#pragma once
#include "tuple.h"
#include <numeric>
#include <cstdint>
#include <ostream>
#include <cmath>

#include "Number.h"

namespace d_rive::detail {

template <typename T1, typename T2>
struct is_same_tpl;

template <template<class...> typename T1, template<class...> typename T2, typename... Args1, typename... Args2>
struct is_same_tpl<T1<Args1...>, T2<Args2...>> {
    constexpr bool static value = (type_index<T1<>> == type_index<T2<>>);
};

template <typename T1, typename T2>
inline constexpr bool is_same_tpl_v = is_same_tpl<T1, T2>::value;


enum class PrintType {
    Cpp,
    WolframAlpha
};

template <typename T = Number<>>
struct Const {
    using N = T;
    using tuple = std::tuple<Const>;

    static_assert(is_same_tpl_v<Number<>, T>);

    constexpr static double value() {
        return N::value();
    }

    template <typename ...Args>
    auto operator()(Args const&... args) const {
        return eval_expr_impl(*this, args...);
    }


    template <typename P>
    constexpr bool operator==(P const& _other) const {
        auto s1 = full_simplify(*this);
        auto s2 = full_simplify(_other);
        return std::is_same_v<decltype(s1), decltype(s2)>;
    }

    static void print(std::ostream& stream, PrintType type) {
        if (type == PrintType::Cpp) {
            stream << Const::value() << "_c";
        } else if (type == PrintType::WolframAlpha) {
            stream << Const::value();
        }
    }

    friend std::ostream& operator<< (std::ostream& stream, Const const&) {
        Const::print(stream, PrintType::Cpp);
        return stream;
    }
};

using ConstZero = Const<>;
using ConstOne  = Const<Number<integer<1>>>;

template <typename T = integer<0>>
struct Var {
    using tuple = std::tuple<Var>;

    static inline constexpr auto n = T::value;

    template <typename ...Args>
    auto operator()(Args const&... args) const {
        return eval_expr_impl(*this, args...);
    }

    static void print(std::ostream& stream, PrintType type) {
        if (type == PrintType::Cpp) {
            stream << "x<" << int(Var::n) << ">";
        } else if (type == PrintType::WolframAlpha) {
//            stream << "x_" << int(Var::n);
            if (int(Var::n) != 0) {
                stream << "c_" <<int(Var::n);
            } else {
                stream << "x";
            }
        }
    }

    template <typename P>
    constexpr bool operator==(P const&) const {
        return std::is_same_v<P, Var>;
    }

    friend std::ostream& operator<< (std::ostream& stream, Var const&) {
        Var::print(stream, PrintType::Cpp);
        return stream;
    }
};

template <typename TP = ConstOne>
struct Ln {
    using tuple = std::tuple<Ln>;
    using P = TP;
    P p;

    template <typename ...Args>
    auto operator()(Args const&... args) const {
        return eval_expr_impl(*this, args...);
    }

    template <typename P>
    constexpr bool operator==(P const& _other) const {
        auto s1 = full_simplify(*this);
        auto s2 = full_simplify(_other);
        return std::is_same_v<decltype(s1), decltype(s2)>;
    }

    static void print(std::ostream& stream, PrintType type) {
        stream << "ln(";
        P::print(stream, type);
        stream << ")";
    }

    friend std::ostream& operator<< (std::ostream& stream, Ln const&) {
        Ln::print(stream, PrintType::Cpp);
        return stream;
    }
};



template <typename TP1 = ConstZero, typename TP2 = ConstOne>
struct Exp {
    using tuple = std::tuple<Exp>;
    using P1 = TP1;
    using P2 = TP2;
    P1 p1;
    P2 p2;

    template <typename P>
    constexpr bool operator==(P const& _other) const {
        auto s1 = full_simplify(*this);
        auto s2 = full_simplify(_other);
        return std::is_same_v<decltype(s1), decltype(s2)>;
    }

    template <typename ...Args>
    auto operator()(Args const&... args) const {
        return eval_expr_impl(*this, args...);
    }

    static void print(std::ostream& stream, PrintType type) {
        stream << "pow(";
        P1::print(stream, type);
        stream << ", ";
        P2::print(stream, type);
        stream << ")";

    }

    friend std::ostream& operator<< (std::ostream& stream, Exp const&) {
        Exp::print(stream, PrintType::Cpp);
        return stream;
    }
};

template <typename ...Ps>
struct Mul {
    using tuple = std::tuple<Ps...>;
    tuple ps;

    template <typename P>
    constexpr bool operator==(P const& _other) const {
        auto s1 = full_simplify(*this);
        auto s2 = full_simplify(_other);
        return std::is_same_v<decltype(s1), decltype(s2)>;
    }

    template <typename ...Args>
    auto operator()(Args const&... args) const {
        return eval_expr_impl(*this, args...);
    }


    static void print(std::ostream& stream, PrintType type) {
        stream << "(";
        if constexpr (sizeof...(Ps) == 0) {
            ConstOne::print(stream, type);
        } else {
            std::apply([&](auto e1, auto... tail) {
                decltype(e1)::print(stream, type);
                [[maybe_unused]] auto f = [&](auto e) {
                    stream << " * ";
                    decltype(e)::print(stream, type);
                };
                (f(tail), ...);
            }, tuple{});
        }
        stream << ")";
    }

    friend std::ostream& operator<< (std::ostream& stream, Mul const&) {
        Mul::print(stream, PrintType::Cpp);
        return stream;
    }
};

template <typename ...Ps>
struct Sum {
    using tuple = std::tuple<Ps...>;
    tuple ps;


    template <typename P>
    constexpr bool operator==(P const& _other) const {
        auto s1 = full_simplify(*this);
        auto s2 = full_simplify(_other);
        return std::is_same_v<decltype(s1), decltype(s2)>;
    }


    template <typename ...Args>
    auto operator()(Args const&... args) const {
        return eval_expr_impl(*this, args...);
    }


    static void print(std::ostream& stream, PrintType type) {
        stream << "(";
        if constexpr (sizeof...(Ps) == 0) {
            ConstZero::print(stream, type);
        } else {
            std::apply([&](auto e1, auto... tail) {
                decltype(e1)::print(stream, type);
                auto f = [&](auto e) {
                    stream << " + ";
                    decltype(e)::print(stream, type);
                };
                (f(tail), ...);
            }, tuple{});
        }
        stream << ")";
    }

    friend std::ostream& operator<< (std::ostream& stream, Sum const&) {
        Sum::print(stream, PrintType::Cpp);
        return stream;
    }
};


template <typename Tuple, template<class...> typename Op>
struct TupleTo;

template <typename ...Ps, template<class...> typename Op>
struct TupleTo<std::tuple<Ps...>, Op> {
    using type = Op<Ps...>;
};
template <template<class...> typename Op, typename Tuple>
using tuple_to_t = typename TupleTo<Tuple, Op>::type;

template <template<class...> typename Op, typename Tuple>
constexpr auto tuple_to(Tuple const&) {
    return tuple_to_t<Op, Tuple>{};
}


// tuple size
template <typename Tuple>
struct size {
    constexpr static size_t value = 1;
};

template<class ...Args>
struct size<Mul<Args...>> {
    constexpr static size_t value = sizeof...(Args);
};
template<class ...Args>
struct size<Sum<Args...>> {
    constexpr static size_t value = sizeof...(Args);
};


template<typename T>
inline constexpr size_t size_v = size<T>::value;

// tuple element type
template <size_t I, typename T>
struct element {
    using type = T;
};

template<size_t I, class ...Args>
struct element<I, Mul<Args...>> {
    using type = typename std::tuple_element_t<I, std::tuple<Args...>>;
};

template<size_t I, class ...Args>
struct element<I, Sum<Args...>> {
    using type = typename std::tuple_element_t<I, std::tuple<Args...>>;
};

template<size_t I, typename T>
using element_t = typename element<I, T>::type;

struct MergeSum {
    template <typename ...Args>
    constexpr auto operator()(Sum<Args...> sum) const {
        return sum.ps;
    }
};

struct MergeMul {
    template <typename ...Args>
    constexpr auto operator()(Mul<Args...> mul) const {
        return mul.ps;
    }
};


template<typename Data, typename L, typename ...Ls>
constexpr auto chain(Data const& data, L l, Ls... ls) {
    auto r = l(data);
    if constexpr(not std::is_same_v<decltype(r), Data>) {
        return r;
    } else {
        return chain(data, ls...);
    }
}

template<typename Data>
constexpr auto chain(Data const& data) {
    return data;
}


template <typename T>
constexpr auto simplify(Const<T> v) {
    return v;
}

template <typename N>
constexpr auto simplify(Var<N>) {
    return Var<N>{};
}

template <typename T>
constexpr auto simplify(Ln<T> v) {
    if constexpr(is_same_tpl_v<T, Const<>>) {
        using N = typename T::N;
        return Const<decltype(ln(N{}))>{};
    } else {
        return Ln<decltype(simplify(v.p))>{};
    }
}


template <typename P1, typename P2>
constexpr auto simplify_impl(Exp<P1, P2> value) {
    auto simplified_p1 = simplify(value.p1);
    auto simplified_p2 = simplify(value.p2);

    auto p1 = simplified_p1;
    auto p2 = simplified_p2;
    return Exp<decltype(p1), decltype(p2)>{};
}

template <typename P1, typename P2>
constexpr auto simplify(Exp<P1, P2> value) {
    (void)value;
    static_assert(not std::is_same_v<P1, ConstZero> or not std::is_same_v<P2, ConstZero>);
    if constexpr(std::is_same_v<P1, ConstZero>) {
        return ConstZero{};
    } else if constexpr(std::is_same_v<P1, ConstOne>) {
        return ConstOne{};
    } else if constexpr(std::is_same_v<P2, ConstZero>) {
        return ConstOne{};
    } else if constexpr(std::is_same_v<P2, ConstOne>) {
        return P1{};
    } else if constexpr(is_same_tpl_v<P1, Const<>> and is_same_tpl_v<P2, Const<>>) {
        using N1 = typename P1::N;
        using N2 = typename P2::N;
        return Const<decltype(pow(N1{}, N2{}))>{};
    } else if constexpr(is_same_tpl_v<P1, Mul<>>){
        return tuple_to<Mul>(tuple_apply_each(value.p1.ps, [](auto e) {
            return std::tuple<Exp<decltype(e), P2>>{};
        }));
    } else if constexpr(is_same_tpl_v<P2, Mul<>> and is_same_tpl_v<P1, Const<>>){
        if constexpr (std::tuple_size_v<typename P2::tuple> > 0){
            using E2   = std::tuple_element_t<0, typename P2::tuple>;
            using Tail2 = tuple_drop_t<1, typename P2::tuple>;

            if constexpr(is_same_tpl_v<E2, Const<>>) {
                return Exp<Exp<P1, E2>, tuple_to_t<Mul, Tail2>>{};
            } else {
                return simplify_impl(value);
            }
        } else {
            return simplify_impl(value);
        }
    } else if constexpr (is_same_tpl_v<P1, Exp<>>) {
        using E1 = typename P1::P1;
        using E2 = typename P1::P2;
        if constexpr (is_same_tpl_v<E2, Const<>> and is_same_tpl_v<P2, Const<>>) {
            return Exp<E1, Mul<E2, P2>>{};
        } else {
            return simplify_impl(value);
        }
    } else {
        return simplify_impl(value);
    }
}


template <typename ...Ps>
constexpr auto simplify(Mul<Ps...> const& value) {
    if constexpr (sizeof...(Ps) == 0) {
        return ConstOne {};
    } else if constexpr (sizeof...(Ps) == 1) {
        return std::get<0>(value.ps);
    } else {
        return tuple_to<Mul>(chain(value.ps,
            [](auto ps) {
                return tuple_apply_each(ps, overloaded {
                    [](ConstOne) { return std::make_tuple(); },
                    [](auto x) { return std::make_tuple(simplify(x)); },
                    MergeMul{},
                });
            },
            [](auto ps) {
                return tuple_type_sort(ps);
            },
            [](auto ps) {
                return tuple_apply_first_pair(ps, overloaded {
                    [](ConstZero x, auto) { return std::make_tuple(x); },
                    [](auto t1, auto t2) {
                        using T1 = decltype(t1);
                        using T2 = decltype(t2);
                        if constexpr(is_same_tpl_v<T1, Const<>> and is_same_tpl_v<T2, Const<>>) {

                            using N1 = typename T1::N;
                            using N2 = typename T2::N;

                            return std::tuple<Const<decltype(N1{} * N2{})>>{};
                        } else if constexpr(is_same_tpl_v<T1, Sum<>>) {
                            return std::make_tuple(tuple_to<Sum>(tuple_apply_each(t1.ps, [&](auto e) {
                                return std::tuple<Mul<decltype(e), T2>>{};
                            })));
                        } else if constexpr(is_same_tpl_v<T1, Exp<>>) {
                            if constexpr (std::is_same_v<typename T1::P1, T2>) {
                                return std::tuple<Exp<T2, Sum<typename T1::P2, ConstOne>>>{};
                            } else if constexpr (is_same_tpl_v<T2, Exp<>>) {
                                if constexpr (std::is_same_v<typename T1::P1, typename T2::P1>) {
                                    return std::tuple<Exp<typename T1::P1, Sum<typename T1::P2, typename T2::P2>>>{};
                                }
                            }
                        } else if constexpr (std::is_same_v<T1, T2>) {
                            return std::tuple<Exp<T1, Const<Number<integer<2>>>>>{};
                        }
                    },
                });
            }
       ));
    }
}

template <typename ...Ps>
constexpr auto simplify(Sum<Ps...> const& value) {
    if constexpr (sizeof...(Ps) == 0) {
        return ConstZero {};
    } else if constexpr (sizeof...(Ps) == 1) {
        return std::get<0>(value.ps);
    } else {
        return tuple_to<Sum>(chain(value.ps,
            [](auto ps) {
                return tuple_apply_each(ps, overloaded {
                    [](ConstZero) { return std::make_tuple(); },
                    [](auto x) { return std::make_tuple(simplify(x)); },
                    MergeSum{},
                });
            },
            [](auto ps) {
                return tuple_type_sort(ps);
            },
            [](auto ps) {
                return tuple_apply_first_pair(ps, overloaded {
                    [](auto t1, auto t2) {
                        using T1 = decltype(t1);
                        using T2 = decltype(t2);
                        if constexpr(std::is_same_v<T1, T2>){
                            return std::tuple<Mul<Const<Number<integer<2>>>, T1>>{};
                        } else if constexpr(size_v<T1> >= 1 and size_v<T2> >= 1) {
                            using E1 = element_t<0, T1>;
                            using E2 = element_t<0, T2>;

                            if constexpr(is_same_tpl_v<E1, Const<>> and is_same_tpl_v<E2, Const<>>) {
                                using Tail1 = tuple_drop_t<1, typename T1::tuple>;
                                using Tail2 = tuple_drop_t<1, typename T2::tuple>;

                                using N1 = typename E1::N;
                                using N2 = typename E2::N;

                                if constexpr(std::is_same_v<Tail1, Tail2>) {
                                    if constexpr(std::tuple_size_v<Tail1> > 0) {
                                        return std::tuple<Mul<Const<decltype(N1{} + N2{})>, tuple_to_t<Mul, Tail1>>>{};
                                    } else {
                                        return std::tuple<Const<decltype(N1{} + N2{})>>{};
                                    }
                                }
                            } else if constexpr(is_same_tpl_v<E1, Const<>>) {
                                using Tail1 = tuple_drop_t<1, typename T1::tuple>;
                                using Tail2 = typename T2::tuple;

                                if constexpr(std::is_same_v<Tail1, Tail2>) {
                                    return std::tuple<Mul<decltype(E1{} + ConstOne{}), tuple_to_t<Mul, Tail1>>>{};
                                }
                            }
                        }
                    },
                 });
            }
        ));
   }
}

template <typename Expr>
constexpr auto full_simplify(Expr const& expr) {
    auto ret = simplify(expr);
    if constexpr(std::is_same_v<decltype(ret), Expr>) {
        return ret;
    } else {
        return full_simplify(ret);
    }
}

template <typename Expr>
constexpr auto list_simplify(Expr const& expr) {
    auto ret = simplify(expr);
    if constexpr(std::is_same_v<decltype(ret), Expr>) {
        return std::make_tuple(ret);
    } else {
        auto r = list_simplify(ret);
        return std::tuple_cat(std::make_tuple(expr), r);
    }
}

template <typename T1, typename T2, typename V>
constexpr auto replace([[maybe_unused]] V var) {
    if constexpr(std::is_same_v<T1, V>) {
        return T2{};
    } else if constexpr(is_same_tpl_v<Exp<>, V>) {
        auto p1 = replace<T1, T2>(var.p1);
        auto p2 = replace<T1, T2>(var.p2);
        return Exp<decltype(p1), decltype(p2)>{};
    } else if constexpr(is_same_tpl_v<Mul<>, V>) {
        return tuple_to<Mul>(std::apply([](auto... e) {
            return std::make_tuple(replace<T1, T2>(e)...);
        }, var.ps));
    } else if constexpr(is_same_tpl_v<Sum<>, V>) {
        return tuple_to<Sum>(std::apply([](auto... e) {
            return std::make_tuple(replace<T1, T2>(e)...);
        }, var.ps));
    } else {
        return V{};
    }
}


template<integer<>::type nr, typename T>
auto set(T) {
    return std::tuple<Var<integer<nr>>, T>{};
}

template <typename Expr, typename Var, typename Const, typename ...Args>
auto eval(Expr e, std::tuple<Var, Const>, Args... args) {
    return full_simplify(eval(replace<Var, Const>(e), args...));
}

template <typename Expr>
auto eval(Expr) {
    return Expr{};
}

template <typename Expr, typename ...Args>
auto eval_expr_impl(Expr e, Args... args) {
    return eval_expr(e, std::make_tuple(args...));
}


template <typename Tuple, typename T>
auto eval_expr(Const<T>, Tuple const&) {
    return Const<T>::value();
}

template <typename Tuple, typename T>
auto eval_expr(Var<T>, Tuple const& tuple) {
    return std::get<T::value>(tuple);
}

template <typename Tuple, typename P1, typename P2>
auto eval_expr(Exp<P1, P2> e, Tuple const& tuple) {
    using std::pow;
    return pow(eval_expr(e.p1, tuple), eval_expr(e.p2, tuple));
}

template <typename Tuple, typename ...Ps>
auto eval_expr(Mul<Ps...> e, Tuple const& tuple) {
    return std::apply([&](auto ...e) {
        return (eval_expr(e, tuple) * ...);
    }, e.ps);
}

template <typename Tuple, typename ...Ps>
auto eval_expr(Sum<Ps...> e, Tuple const& tuple) {
    return std::apply([&](auto ...e) {
        return (eval_expr(e, tuple) + ...);
    }, e.ps);
}

// ------------------- is const
//
template<int nr, typename Expr>
constexpr auto isConst();

template <int nr, typename N>
constexpr auto isConst_impl(Const<N>) {
    return true;
}

template <int nr, integer<>::type n>
constexpr auto isConst_impl(Var<integer<n>>) {
    return n != nr;
}

template <int nr, typename T>
constexpr auto isConst_impl(Ln<T> l) {
    return isConst_impl<nr>(l.p);
}

template <int nr, typename P1, typename P2>
constexpr auto isConst_impl(Exp<P1, P2> value) {
    return isConst_impl<nr>(value.p1) and isConst_impl<nr>(value.p2);
}


template <int nr, typename ...Ps>
constexpr auto isConst_impl(Mul<Ps...> const& e) {
    return std::apply([](auto ...e){
        return (isConst_impl<nr>(e) and ...);
    }, e.ps);
}

template <int nr, typename ...Ps>
constexpr auto isConst_impl(Sum<Ps...> const& e) {
    return std::apply([](auto ...e){
        return (isConst_impl<nr>(e) and ...);
    }, e.ps);
}

template<int nr, typename Expr>
constexpr auto isConst() {
    return isConst_impl<nr>(Expr{});
}



// ------------------- derive

template<int nr=1, typename Expr, typename Head = Var<integer<0>>>
constexpr auto derive(Expr e, Head = {});

template <int nr, typename N>
constexpr auto derive_impl(Const<N>) {
    return ConstZero{};
}

template <int nr, integer<>::type n>
constexpr auto derive_impl(Var<integer<n>>) {
    if constexpr(n == nr) {
        return ConstOne{};
    } else {
        return ConstZero{};
    }
}

template <int nr, typename T>
constexpr auto derive_impl(Ln<T> l) {
    return derive_impl<nr>(l.p) / l.p;
}


template <int nr, typename P1, typename P2>
constexpr auto derive_impl(Exp<P1, P2> value) {
    if constexpr(is_same_tpl_v<P2, Const<>>) {
        using Outer = Sum<P2, Const<Number<integer<-1>>>>;
        auto inner = derive_impl<nr>(value.p1);
       return Mul<P2, Exp<P1, Outer>, decltype(inner)>{};
    } else {
        constexpr auto p1 = derive_impl<nr>(P1{});
        constexpr auto p2 = derive_impl<nr>(P2{});

        return (p2 * Ln<decltype(value.p1)>{} + value.p2 * p1 / value.p1) * pow(value.p1, value.p2);
    }
}


template <int nr>
constexpr auto derive_impl(Mul<> const&) {
    return ConstZero{};
}

template <int nr, typename Head, typename ...Ps>
constexpr auto derive_impl(Mul<Head, Ps...> const&) {
    using P1 = Mul<decltype(derive_impl<nr>(Head{})), Mul<Ps...>>;
    using P2 = Mul<Head, decltype(derive_impl<nr>(Mul<Ps...>{}))>;
    return Sum<P1, P2>{};
}

template <int nr, typename ...Ps>
constexpr auto derive_impl(Sum<Ps...> const&) {
    return Sum<decltype(derive_impl<nr>(Ps{}))...>{};
}

template<int nr, typename Expr, typename Head>
constexpr auto derive(Expr e, Head) {
    auto ret = full_simplify(derive_impl<Head::n>(Expr{}));
    if constexpr(nr > 1) {
        return derive<nr-1>(ret, Head{});
    } else {
        return ret;
    }
}

template<typename Expr>
auto partial_derive(Expr) {
    return std::make_tuple();
}

template<typename Expr, typename Head, typename ...Args>
auto partial_derive(Expr e, Head head, Args... args) {
    return std::tuple_cat(std::make_tuple(derive(e, head)), partial_derive(e, args...));
}

template <unsigned delCt>
constexpr bool isValidDoubleImpl() {
    return delCt == 0 or delCt == 1;
}
template <unsigned delCt, char digit, char... tail>
constexpr bool isValidDoubleImpl() {
    if constexpr(digit != '.' and (digit < '0' or digit > '9')) {
        return false;
    }
    return isValidDoubleImpl<delCt + int(digit == '.'), tail...>();
}

template <char... tail>
constexpr bool isValidDouble() {
    return isValidDoubleImpl<0, tail...>();
}


static_assert(isValidDouble<'1', '.', '2'>());
static_assert(isValidDouble<'.', '2'>());
static_assert(isValidDouble<'.'>());
static_assert(isValidDouble<>());
static_assert(isValidDouble<'1' >());
static_assert(isValidDouble<'1', '.' >());
static_assert(not isValidDouble<'1', '.', '.' >());

template <char... args>
struct char_tuple {
    const static std::string value;
    const static size_t size = sizeof...(args);
};

template <char... args>
const std::string char_tuple<args...>::value{args...};


template <bool hasSplit, char del, typename P1, typename P2, char... args>
struct split;

template <bool hasSplit, char del, char ...args1, char ...args2>
struct split<hasSplit, del, char_tuple<args1...>, char_tuple<args2...>> {
    using P1 = char_tuple<args1...>;
    using P2 = char_tuple<args2...>;
};

template<char del, char ...args1, char ...args2, char head, char ...tail>
struct split<false, del, char_tuple<args1...>, char_tuple<args2...>, head, tail...> {
    using NextSplit = split<false, del, char_tuple<args1..., head>, char_tuple<args2...>, tail...>;
    using P1 = typename NextSplit::P1;
    using P2 = typename NextSplit::P2;
};

template<char del, char ...args1, char ...args2, char ...tail>
struct split<false, del, char_tuple<args1...>, char_tuple<args2...>, del, tail...> {
    using NextSplit = split<true, del, char_tuple<args1...>, char_tuple<args2...>, tail...>;
    using P1 = typename NextSplit::P1;
    using P2 = typename NextSplit::P2;
};


template<char del, char ...args1, char ...args2, char head, char ...tail>
struct split<true, del, char_tuple<args1...>, char_tuple<args2...>, head, tail...> {
    using NextSplit = split<true, del, char_tuple<args1...>, char_tuple<args2..., head>, tail...>;
    using P1 = typename NextSplit::P1;
    using P2 = typename NextSplit::P2;
};

template <typename value, typename P1>
struct stoi;

template <typename Integer>
struct stoi<Integer, char_tuple<>> {
    using type = Integer;
};

template <typename Integer, char head, char... tail>
struct stoi<Integer, char_tuple<head, tail...>> {
    using type = typename stoi<integer<Integer::value * 10 + (head - '0')>, char_tuple<tail...>>::type;
};

template <char... args>
constexpr auto operator "" _c() {
    static_assert(isValidDouble<args...>());
    using Split = split<false, '.', char_tuple<>, char_tuple<>, args...>;
    using I1 = typename stoi<integer<0>, typename Split::P1>::type;
    using I2 = typename stoi<integer<0>, typename Split::P2>::type;
    using Shift = decltype(pow(Number<integer<10>>{}, integer<Split::P2::size>{}));
    auto constexpr shift = Shift::N::value;
    using N = decltype(normalize(Number<integer<I1::value * shift + I2::value>, integer<shift>>{}));
    return Const<N>{};
}

// ------------------- solve
template<int nr, typename Expr, typename Expr2>
constexpr auto solve_impl(Expr, Expr2) {
    static_assert("can't solve this equation");
}

template<int nr, typename Expr2>
constexpr auto solve_impl(Expr2 e2, Var<integer<nr>>) {
    static_assert(isConst<nr, Expr2>());
    return e2;
}

template<int nr, typename Expr2, typename T>
constexpr auto solve_impl(Expr2, Const<T>) {
    static_assert("can't solve this equation");
}
template<int nr, typename Expr2, typename T>
constexpr auto solve_impl(Expr2, Var<T>) {
    static_assert("can't solve this equation");
}

template<int nr, typename Expr2, typename P1, typename P2>
constexpr auto solve_impl(Expr2 e2, Exp<P1, P2> e) {
    static_assert(isConst<nr, P1>() or isConst<nr, P2>());
    if constexpr(isConst<nr, P1>()) {
        return solve_impl<nr>(ln(e2) / ln(e.p1), e.p2);
    } else {
        return solve_impl<nr>(pow(e2, 1_c / e.p2), e.p1);
    }
}


template<int nr, typename Expr2>
constexpr auto solve_impl(Expr2, Mul<>) {
    throw std::runtime_error("can't solve this equation");
}

template<int nr, typename Expr2, typename Head, typename ...Ps>
constexpr auto solve_impl(Expr2 e2, Mul<Head, Ps...>) {
    static_assert(isConst<nr, Expr2>());
    if constexpr (isConst<nr, Head>()) {
        return solve_impl<nr>(e2 / Head{}, Mul<Ps...>{});
    } else if constexpr (sizeof...(Ps) == 0) {
        return solve_impl<nr>(e2, Head{});
    } else if constexpr (isConst<nr, Mul<Ps...>>()) {
        return solve_impl<nr>(e2 / Mul<Ps...>{}, Head{});
    } else {
        throw std::runtime_error("can't solve this equation");
    }
}


template<int nr, typename Expr2>
constexpr auto solve_impl(Expr2, Sum<>) {
    throw std::runtime_error("can't solve this equation");
}

template<int nr, typename Expr2, typename Head, typename ...Ps>
constexpr auto solve_impl(Expr2 e2, Sum<Head, Ps...>) {
    static_assert(isConst<nr, Expr2>());
    if constexpr (isConst<nr, Head>()) {
        return full_simplify(solve_impl<nr>(e2 - Head{}, Sum<Ps...>{}));
    } else if constexpr (isConst<nr, Sum<Ps...>>()) {
        return solve_impl<nr>(e2 - Sum<Ps...>{}, Head{});
    } else {
        throw std::runtime_error("can't solve this equation");
    }
}

template<integer<>::type nr2, typename T2>
constexpr auto solve(T2 r, Var<integer<nr2>>) {
    static_assert(not isConst<nr2, T2>());
    return full_simplify(solve_impl<nr2>(0_c, r));
}


// ------------------- integrate
template<int nr=1, typename Expr, typename Head = Var<integer<0>>>
constexpr auto integrate(Expr e, Head = {});

template <int nr, typename N>
constexpr auto integrate_impl(Const<N>) {
    return Mul<Const<N>, Var<integer<nr>>>{};
}

template <int nr, integer<>::type n>
constexpr auto integrate_impl(Var<integer<n>>) {
    if constexpr(n == nr) {
        return Mul<Const<Number<integer<1>, integer<2>>>, Exp<Var<integer<n>>, Const<Number<integer<2>>>>>{};
    } else {
        return Mul<Var<integer<n>>, Var<integer<nr>>>{};
    }
}

template <int nr, typename T>
constexpr auto integrate_impl(Ln<T> e) {
    using G = Var<integer<nr>>;
    return  G{} * ln(e.p) - integrate_impl<nr>(G{} * derive<nr>(e));
}


template <int nr, typename P1, typename P2>
constexpr auto integrate_impl([[maybe_unused]] Exp<P1, P2> e) {
    static_assert(isConst<nr, P1>() or isConst<nr, P2>());

    if constexpr (isConst<nr, P1>() and isConst<nr, P2>()) {
        return e * Var<integer<nr>>{};
    } else if constexpr (isConst<nr, P2>()) {
        return 1_c/(e.p2+1_c) * pow(e.p1, e.p2+1_c);
    } else if constexpr (is_same_tpl_v<P2, Var<>>) {
        return pow(e.p1, e.p2) / ln(e.p1);
    } else if constexpr (isConst<nr, P1>()) {
        static_assert(not isConst<nr, P2>());
        constexpr auto z = derive(P2{}, Var<integer<nr>>{});
        static_assert(isConst<nr, decltype(z)>(), "can only solve simple integrals");
        constexpr auto s = integrate_impl<-1>(pow(P1{}, Var<integer<-1>>{}) / z);
        return replace<Var<integer<-1>>, P2>(s);
    } else {
        throw std::runtime_error("can't integrate");
    }
}

template <int nr>
constexpr auto integrate_impl(Mul<> const&) {
    return Var<integer<nr>>{};
}

template <int nr, typename Head>
constexpr auto integrate_impl(Mul<Head> const&) {
    return integrate_impl<nr>(Head{});
}

template <int nr, typename Head, typename Next, typename ...Ps>
constexpr auto integrate_impl(Mul<Head, Next, Ps...> const&) {
    if constexpr (isConst<nr, Head>()){
        constexpr auto p2 = integrate_impl<nr>(Mul<Next, Ps...>{});
        return Mul<Head, decltype(p2)>{};
    } else {
        using G = decltype(integrate_impl<nr>(Head{}));
        using H = Mul<Next, Ps...>;
        return G{} * H{} - integrate_impl<nr>(full_simplify(G{} * derive_impl<nr>(H{})));
    }
}

template <int nr, typename ...Ps>
constexpr auto integrate_impl(Sum<Ps...> const&) {
    return Sum<decltype(integrate_impl<nr>(Ps{}))...>{};
}

template<int nr, typename Expr, typename Head>
constexpr auto integrate(Expr, Head) {
    auto ret = full_simplify(integrate_impl<Head::n>(Expr{}));
    if constexpr(nr > 1) {
        return integrate<nr-1>(ret, Head{});
    } else {
        return ret;
    }
}


// ------------------- full_simplify

template <typename T1, typename T2>
constexpr auto operator+(T1 const&, T2 const&) {
    return full_simplify(Sum<T1, T2>{});
}

template <typename T1, typename T2>
constexpr auto operator*(T1 const&, T2 const&) {
    return full_simplify(Mul<T1, T2>{});
}

template <typename T1, typename T2>
constexpr auto operator/(T1 const&, T2 const&) {
    return full_simplify(Mul<T1, Exp<T2, Const<Number<integer<-1>>>>>{});
}

template <typename T1>
constexpr auto operator-(T1 const&) {
    return full_simplify(Mul<Const<Number<integer<-1>>>, T1>{});
}
template <typename T1, typename T2>
constexpr auto operator-(T1 const&, T2 const&) {
    return full_simplify(Sum<T1, Mul<Const<Number<integer<-1>>>, T2>>{});
}

template <typename T1, typename T2>
constexpr auto pow(T1 const&, T2 const&) {
    return full_simplify(Exp<T1, T2>{});
}

template <typename T1>
constexpr auto ln(T1 const&) {
    return full_simplify(Ln<T1>{});
}

template <integer<>::type n, integer<>::type d = 1>
using C = Const<decltype(normalize(Number<integer<n>, integer<d>>{}))>;

template <int n>
using X = Var<integer<n>>;

template <integer<>::type n, integer<>::type d = 1>
constexpr auto c = C<n, d>{};

} // namespace d_rive::detail

namespace d_rive {

template <int n>
constexpr auto x = detail::X<n>{};

//template <char... args>
using detail::operator ""_c;
/*constexpr auto operator "" _c() {
    return detail::operator "" _c<args...>();
}*/

} // namespace d_rive
