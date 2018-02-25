#pragma once
#include "tuple.h"
#include "Number.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <ostream>
#include <sstream>

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

/* Every EntityBase should implement
 *
 * simplify_impl
 * to_string_impl
 * some operator for usage
 * eval_expr
 * derive_impl
 * integrate
 * isConst_impl
 */

template<typename T, typename ...Ps>
struct EntityBase {
    using tuple = std::tuple<Ps...>;
    template <typename ...Args>
    constexpr auto operator()(Args const&... args) const {
        return eval_expr_impl(T{}, args...);
    }

    template <typename P>
    constexpr bool operator==(P const&) const {
        auto s1 = full_simplify(T{});
        auto s2 = full_simplify(P{});
        return std::is_same_v<decltype(s1), decltype(s2)>;
    }

    friend auto operator<< (std::ostream& stream, EntityBase) -> std::ostream& {
        return stream << to_string(T{}, PrintType::Cpp);
    }
};

template <typename T = Number<>>
struct Const : EntityBase<Const<T>, Const<T>> {
    using N = T;

    static_assert(is_same_tpl_v<Number<>, T>);

    constexpr static auto value() {
        return N::value();
    }
};

using ConstZero = Const<>;
using ConstOne  = Const<Number<integer<1>>>;

template <typename T = integer<0>>
struct Var : EntityBase<Var<T>, Var<T>> {
    using N = T;
};

template <typename TP = ConstOne>
struct Ln : EntityBase<Ln<TP>, Ln<TP>> {
    using P = TP;
};

template <typename TP = ConstOne>
struct Sin : EntityBase<Sin<TP>, Sin<TP>> {
    using P = TP;
};

template <typename TP = ConstOne>
struct Cos : EntityBase<Cos<TP>, Cos<TP>> {
    using P = TP;
};

template <typename TP = ConstOne>
struct Sign : EntityBase<Sign<TP>, Sign<TP>> {
    using P = TP;
};

template <typename TP = ConstOne>
struct Abs : EntityBase<Abs<TP>, Abs<TP>> {
    using P = TP;
};



template <typename TP1 = ConstZero, typename TP2 = ConstOne>
struct Exp : EntityBase<Exp<TP1, TP2>, Exp<TP1, TP2>> {
    using P1 = TP1;
    using P2 = TP2;
};

template <typename ...Ps>
struct Mul : EntityBase<Mul<Ps...>, Ps...> {};

template <typename ...Ps>
struct Sum : EntityBase<Sum<Ps...>, Ps...> {};

template <typename ...Ps>
struct Min : EntityBase<Min<Ps...>, Ps...> {};

template <typename ...Ps>
struct Max : EntityBase<Max<Ps...>, Ps...> {};



// ------------------- Parsing, _c option

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

// ------------------- to_string

template <typename T>
auto to_string_impl(Const<T>, PrintType type) {
    std::string valueAsStr = [&] () {
        using TT = decltype(Const<T>::value());
        if constexpr(std::is_same_v<TT, double> or std::is_same_v<TT, float>) {
            std::stringstream ss;
            ss << Const<T>::value();
            return ss.str();
        } else {
            using std::to_string;
            return to_string(Const<T>::value());
        }
    }();

    if (type == PrintType::Cpp) {
        return valueAsStr + "_c";
    } else if (type == PrintType::WolframAlpha) {
        return valueAsStr;
    }
    throw std::runtime_error("Unknown print type");
}

template <typename T>
auto to_string_impl(Var<T>, PrintType type) -> std::string {
    using std::to_string;
    constexpr auto n = int(Var<T>::N::value);
    if (type == PrintType::Cpp) {
        return "x<" + to_string(n) + ">";
    } else if (type == PrintType::WolframAlpha) {
        if (n != 0) {
            return "c_" + to_string(n);
        }
        return "x";
    }
    throw std::runtime_error("Unknown print type");
}


template <typename TP>
auto to_string_impl(Ln<TP>, PrintType type) {
    return "ln(" + to_string(TP{}, type) + ")";
}
template <typename TP>
auto to_string_impl(Sin<TP>, PrintType type) {
    return "sin(" + to_string(TP{}, type) + ")";
}
template <typename TP>
auto to_string_impl(Cos<TP>, PrintType type) {
    return "cos(" + to_string(TP{}, type) + ")";
}

template <typename TP>
auto to_string_impl(Sign<TP>, PrintType type) {
    return "sign(" + to_string(TP{}, type) + ")";
}

template <typename TP>
auto to_string_impl(Abs<TP>, PrintType type) {
    return "abs(" + to_string(TP{}, type) + ")";
}

template <typename TP1, typename TP2>
auto to_string_impl(Exp<TP1, TP2>, PrintType type) {
    return "(" + to_string(TP1{}, type) + "^" + to_string(TP2{},type) + ")";
}

template <typename ...Ps>
auto to_string_multi_impl(std::string_view op, std::tuple<Ps...>, PrintType type) {
    return "(" + std::apply([&](auto e1, auto... tail) {
        auto s = to_string(e1, type);
        [[maybe_unused]] auto f = [&](auto e) {
            return std::string(op) + to_string(e, type);
        };
        return (s + ... + f(tail));
    }, std::tuple<Ps...>{}) +")";
}

template <typename ...Ps>
auto to_string_impl(Mul<Ps...>, PrintType type) {
    if constexpr (sizeof...(Ps) == 0) {
        return to_string_impl(1_c, type);
    } else {
        return to_string_multi_impl(" * ", std::tuple<Ps...>{}, type);
    }
}

template <typename ...Ps>
auto to_string_impl(Sum<Ps...>, PrintType type) {
    if constexpr (sizeof...(Ps) == 0) {
        return to_string_impl(0_c, type);
    } else {
        return to_string_multi_impl(" * ", std::tuple<Ps...>{}, type);
    }
}

template <typename ...Ps>
auto to_string_impl(Min<Ps...>, PrintType type) {
    if constexpr (sizeof...(Ps) == 0) {
        return to_string_impl(0_c, type);
    } else {
        return "min" + to_string_multi_impl(", ", std::tuple<Ps...>{}, type);
    }
}

template <typename ...Ps>
auto to_string_impl(Max<Ps...>, PrintType type) {
    if constexpr (sizeof...(Ps) == 0) {
        return to_string_impl(0_c, type);
    } else {
        return "max" + to_string_multi_impl(", ", std::tuple<Ps...>{}, type);
    }
}



template <typename Expr>
auto to_string(Expr e, PrintType type = PrintType::Cpp) {
    return to_string_impl(e, type);
}


// -------------- some helper functions for simplify

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
        return typename Sum<Args...>::tuple{};
    }
};

struct MergeMul {
    template <typename ...Args>
    constexpr auto operator()(Mul<Args...> mul) const {
        return typename Mul<Args...>::tuple{};
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

// ------------------- simplify

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
        return Ln<decltype(simplify(T{}))>{};
    }
}

template <typename T>
constexpr auto simplify(Sin<T>) {
    if constexpr(is_same_tpl_v<T, Const<>>) {
        using N = typename T::N;
        return Const<decltype(sin(N{}))>{};
    } else {
        return Sin<decltype(simplify(T{}))>{};
    }
}

template <typename T>
constexpr auto simplify(Cos<T>) {
    if constexpr(is_same_tpl_v<T, Const<>>) {
        using N = typename T::N;
        return Const<decltype(cos(N{}))>{};
    } else {
        return Cos<decltype(simplify(T{}))>{};
    }
}

template <typename T>
constexpr auto simplify(Sign<T>) {
    if constexpr(is_same_tpl_v<T, Const<>>) {
        using N = typename T::N;
        return Const<decltype(number::sign(N{}))>{};
    } else {
        return Sign<decltype(simplify(T{}))>{};
    }
}

template <typename T>
constexpr auto simplify(Abs<T>) {
    if constexpr(is_same_tpl_v<T, Const<>>) {
        using N = typename T::N;
        return Const<decltype(number::abs(N{}))>{};
    } else {
        return Abs<decltype(simplify(T{}))>{};
    }
}

template <typename P1, typename P2>
constexpr auto simplify_impl(Exp<P1, P2> value) {
    auto simplified_p1 = simplify(P1{});
    auto simplified_p2 = simplify(P2{});

    auto p1 = simplified_p1;
    auto p2 = simplified_p2;
    return Exp<decltype(p1), decltype(p2)>{};
}

template <typename P1, typename P2>
constexpr auto simplify([[maybe_unused]] Exp<P1, P2> value) {
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
        return tuple_to<Mul>(tuple_apply_each(typename P1::tuple{}, [](auto e) {
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
        return std::get<0>(typename Mul<Ps...>::tuple{});
    } else {
        return tuple_to<Mul>(chain(typename Mul<Ps...>::tuple{},
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
                            return std::make_tuple(tuple_to<Sum>(tuple_apply_each(typename T1::tuple{}, [&](auto e) {
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
        return std::get<0>(typename Sum<Ps...>::tuple{});
    } else {
        return tuple_to<Sum>(chain(typename Sum<Ps...>::tuple{},
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

template <typename ...Ps>
constexpr auto simplify(Min<Ps...> const& value) {
    static_assert(sizeof...(Ps) > 0);
    if constexpr (sizeof...(Ps) == 1) {
        return std::get<0>(typename Min<Ps...>::tuple{});
    } else {
        auto tuple = tuple_type_sort(std::tuple<decltype(simplify(Ps{}))...>{});
        return tuple_to<Min>(tuple_apply_first_pair(tuple, overloaded {
            [](auto t1, auto t2) {
                if constexpr (std::is_same_v<decltype(t1), decltype(t2)>) {
                    return std::tuple<decltype(t1)>{};
                } else if constexpr (is_same_tpl_v<decltype(t1), Const<>> and is_same_tpl_v<decltype(t2), Const<>>) {
                    if constexpr (decltype(t1)::value() < decltype(t2)::value()) {
                        return std::tuple<decltype(t1)>{};
                    } else {
                        return std::tuple<decltype(t2)>{};
                    }
                }
            }
        }));
    }
}

template <typename ...Ps>
constexpr auto simplify(Max<Ps...> const& value) {
    static_assert(sizeof...(Ps) > 0);
    if constexpr (sizeof...(Ps) == 1) {
        return std::get<0>(typename Max<Ps...>::tuple{});
    } else {
        auto tuple = tuple_type_sort(std::tuple<decltype(simplify(Ps{}))...>{});
        return tuple_to<Max>(tuple_apply_first_pair(tuple, overloaded {
            [](auto t1, auto t2) {
                if constexpr (std::is_same_v<decltype(t1), decltype(t2)>) {
                    return std::tuple<decltype(t1)>{};
                } else if constexpr (is_same_tpl_v<decltype(t1), Const<>> and is_same_tpl_v<decltype(t2), Const<>>) {
                    if constexpr (decltype(t1)::value() > decltype(t2)::value()) {
                        return std::tuple<decltype(t1)>{};
                    } else {
                        return std::tuple<decltype(t2)>{};
                    }
                }
            }
        }));
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
        auto p1 = replace<T1, T2>(typename V::P1{});
        auto p2 = replace<T1, T2>(typename V::P2{});
        return Exp<decltype(p1), decltype(p2)>{};
    } else if constexpr(is_same_tpl_v<Ln<>, V>) {
        auto p = replace<T1, T2>(typename V::P{});
        return Ln<decltype(p)>{};
    } else if constexpr(is_same_tpl_v<Sin<>, V>) {
        auto p = replace<T1, T2>(typename V::P{});
        return Sin<decltype(p)>{};
    } else if constexpr(is_same_tpl_v<Cos<>, V>) {
        auto p = replace<T1, T2>(typename V::P{});
        return Cos<decltype(p)>{};
    } else if constexpr(is_same_tpl_v<Mul<>, V>) {
        return tuple_to<Mul>(std::apply([](auto... e) {
            return std::make_tuple(replace<T1, T2>(e)...);
        }, typename V::tuple{}));
    } else if constexpr(is_same_tpl_v<Sum<>, V>) {
        return tuple_to<Sum>(std::apply([](auto... e) {
            return std::make_tuple(replace<T1, T2>(e)...);
        }, typename V::tuple{}));
    } else {
        return V{};
    }
}

// ------------------- operator

template <typename T1, typename T2, typename ...Ps1, typename... Ps2>
constexpr auto operator+(EntityBase<T1, Ps1...> const&, EntityBase<T2, Ps2...> const&) {
    return full_simplify(Sum<T1, T2>{});
}

template <typename T1, typename T2, typename ...Ps1, typename ...Ps2>
constexpr auto operator*(EntityBase<T1, Ps1...> const&, EntityBase<T2, Ps2...> const&) {
    return full_simplify(Mul<T1, T2>{});
}

template <typename T1, typename T2, typename ...Ps1, typename ...Ps2>
constexpr auto operator/(EntityBase<T1, Ps1...> const&, EntityBase<T2, Ps2...> const&) {
    return full_simplify(Mul<T1, Exp<T2, Const<Number<integer<-1>>>>>{});
}

template <typename T1, typename ...Ps>
constexpr auto operator-(EntityBase<T1, Ps...> const&) {
    return full_simplify(Mul<Const<Number<integer<-1>>>, T1>{});
}
template <typename T1, typename T2, typename ...Ps1, typename ...Ps2>
constexpr auto operator-(EntityBase<T1, Ps1...> const&, EntityBase<T2, Ps2...> const&) {
    return full_simplify(Sum<T1, Mul<Const<Number<integer<-1>>>, T2>>{});
}

template <typename T1, typename T2, typename ...Ps1, typename ...Ps2>
constexpr auto pow(EntityBase<T1, Ps1...> const&, EntityBase<T2, Ps2...> const&) {
    return full_simplify(Exp<T1, T2>{});
}
template <typename T1, typename T2, typename ...Ps1, typename ...Ps2>
constexpr auto operator^(EntityBase<T1, Ps1...> const p1, EntityBase<T2, Ps2...> const& p2) {
    return pow(p1, p2);
}


template <typename T1, typename ...Ps>
constexpr auto ln(EntityBase<T1, Ps...> const&) {
    return full_simplify(Ln<T1>{});
}

template <typename T1, typename ...Ps>
constexpr auto sin(EntityBase<T1, Ps...> const&) {
    return full_simplify(Sin<T1>{});
}

template <typename T1, typename ...Ps>
constexpr auto cos(EntityBase<T1, Ps...> const&) {
    return full_simplify(Cos<T1>{});
}
template <typename T1, typename ...Ps>
constexpr auto sign(EntityBase<T1, Ps...> const&) {
    return full_simplify(Sign<T1>{});
}
template <typename T1, typename ...Ps>
constexpr auto abs(EntityBase<T1, Ps...> const&) {
    return full_simplify(Abs<T1>{});
}

template <typename ...Ts>
constexpr auto min(Ts const&...) {
    return full_simplify(Min<Ts...>{});
}

template <typename ...Ts>
constexpr auto max(Ts const&...) {
    return full_simplify(Max<Ts...>{});
}







// ------------------- eval expressions

template<integer<>::type nr, typename T>
auto set(T) {
    return std::tuple<Var<integer<nr>>, T>{};
}

template <typename Expr, typename Var, typename Const, typename ...Args>
constexpr auto eval(Expr e, std::tuple<Var, Const>, Args... args) {
    return full_simplify(eval(replace<Var, Const>(e), args...));
}

template <typename Expr>
constexpr auto eval(Expr) {
    return Expr{};
}

template <typename Expr, typename ...Args>
constexpr auto eval_expr_impl(Expr e, Args... args) {
    return eval_expr(e, std::make_tuple(args...));
}

template <typename Tuple, typename T>
constexpr auto eval_expr(Const<T>, Tuple const&) {
    return Const<T>::value();
}

template <typename Tuple, typename T>
constexpr auto eval_expr(Var<T>, Tuple const& tuple) {
    return std::get<T::value>(tuple);
}

template <typename Tuple, typename T>
constexpr auto eval_expr(Ln<T>, Tuple const& tuple) {
    using std::log;
    return log(eval_expr(T{}, tuple));
}

template <typename Tuple, typename T>
constexpr auto eval_expr(Sin<T>, Tuple const& tuple) {
    using std::sin;
    return sin(eval_expr(T{}, tuple));
}
template <typename Tuple, typename T>
constexpr auto eval_expr(Cos<T>, Tuple const& tuple) {
    using std::cos;
    return cos(eval_expr(T{}, tuple));
}

template <typename Tuple, typename T>
constexpr auto eval_expr(Sign<T>, Tuple const& tuple) {
    auto r = eval_expr(T{}, tuple);
    if (r > 0) {
        return 1.;
    } else if (r < 0) {
        return -1.;
    } else {
        return 0.;
    }
}


template <typename Tuple, typename T>
constexpr auto eval_expr(Abs<T> e, Tuple const& tuple) {
    using std::abs;
    return abs(eval_expr(T{}, tuple));
}


template <typename Tuple, typename P1, typename P2>
constexpr auto eval_expr(Exp<P1, P2> e, Tuple const& tuple) {
    using std::pow;
    return pow(eval_expr(P1{}, tuple), eval_expr(P2{}, tuple));
}

template <typename Tuple, typename ...Ps>
constexpr auto eval_expr(Mul<Ps...> e, Tuple const& tuple) {
    return std::apply([&](auto ...e) {
        return (eval_expr(e, tuple) * ...);
    }, typename Mul<Ps...>::tuple{});
}

template <typename Tuple, typename ...Ps>
constexpr auto eval_expr(Sum<Ps...> e, Tuple const& tuple) {
    return std::apply([&](auto ...e) {
        return (eval_expr(e, tuple) + ...);
    }, typename Mul<Ps...>::tuple{});
}

template <typename Tuple, typename ...Ps>
constexpr auto eval_expr(Min<Ps...> e, Tuple const& tuple) {
    using std::min;
    return min({eval_expr(Ps{}, tuple)...});
}
template <typename Tuple, typename ...Ps>
constexpr auto eval_expr(Max<Ps...> e, Tuple const& tuple) {
    using std::max;
    return max({eval_expr(Ps{}, tuple)...});
}



// ------------------- is const

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
    return isConst_impl<nr>(T{});
}
template <int nr, typename T>
constexpr auto isConst_impl(Sin<T> l) {
    return isConst_impl<nr>(T{});
}
template <int nr, typename T>
constexpr auto isConst_impl(Cos<T> l) {
    return isConst_impl<nr>(T{});
}

template <int nr, typename T>
constexpr auto isConst_impl(Sign<T> l) {
    return isConst_impl<nr>(T{});
}
template <int nr, typename T>
constexpr auto isConst_impl(Abs<T> l) {
    return isConst_impl<nr>(T{});
}



template <int nr, typename P1, typename P2>
constexpr auto isConst_impl(Exp<P1, P2> value) {
    return isConst_impl<nr>(P1{}) and isConst_impl<nr>(P2{});
}


template <int nr, typename ...Ps>
constexpr auto isConst_impl(Mul<Ps...> const& e) {
    return std::apply([](auto ...e){
        return (isConst_impl<nr>(e) and ...);
    }, typename Mul<Ps...>::tuple{});
}

template <int nr, typename ...Ps>
constexpr auto isConst_impl(Sum<Ps...> const& e) {
    return std::apply([](auto ...e){
        return (isConst_impl<nr>(e) and ...);
    }, typename Sum<Ps...>::tuple{});
}
template <int nr, typename ...Ps>
constexpr auto isConst_impl(Min<Ps...> const& e) {
    return std::apply([](auto ...e){
        return (isConst_impl<nr>(e) and ...);
    }, typename Min<Ps...>::tuple{});
}
template <int nr, typename ...Ps>
constexpr auto isConst_impl(Max<Ps...> const& e) {
    return std::apply([](auto ...e){
        return (isConst_impl<nr>(e) and ...);
    }, typename Max<Ps...>::tuple{});
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
    return 0_c;
}

template <int nr, integer<>::type n>
constexpr auto derive_impl(Var<integer<n>>) {
    if constexpr(n == nr) {
        return 1_c;
    } else {
        return 0_c;
    }
}

template <int nr, typename T>
constexpr auto derive_impl(Ln<T> l) {
    return derive_impl<nr>(T{}) / T{};
}

template <int nr, typename T>
constexpr auto derive_impl(Sin<T>) {
    return cos(T{}) * derive_impl<nr>(T{});
}
template <int nr, typename T>
constexpr auto derive_impl(Cos<T>) {
    return -sin(T{}) * derive_impl<nr>(T{});
}

template <int nr, typename T>
constexpr auto derive_impl(Sign<T>) {
    return 0_c;
}

template <int nr, typename T>
constexpr auto derive_impl(Abs<T>) {
    return abs(derive_impl<nr>(T{}));
}


template <int nr, typename P1, typename P2>
constexpr auto derive_impl(Exp<P1, P2> value) {
    if constexpr(is_same_tpl_v<P2, Const<>>) {
        auto outer = P2{} - 1_c;
        auto inner = derive_impl<nr>(P1{});
        return P2{} * pow(P1{}, outer) * inner;
    } else {
        constexpr auto p1 = derive_impl<nr>(P1{});
        constexpr auto p2 = derive_impl<nr>(P2{});

        return (p2 * Ln<P1>{} + P2{} * p1 / P1{}) * pow(P1{}, P2{});
    }
}

template<int nr, typename Expr, typename Head>
constexpr auto derive(Expr e, Head) {
    auto ret = full_simplify(derive_impl<Head::N::value>(Expr{}));
    if constexpr(nr > 1) {
        return derive<nr-1>(ret, Head{});
    } else {
        return ret;
    }
}

template <int nr>
constexpr auto derive_impl(Mul<> const&) {
    return 0_c;
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

template <int nr, typename ...Ps>
constexpr auto derive_impl(Min<Ps...> const&) {
    return Min<decltype(derive_impl<nr>(Ps{}))...>{};
}

template <int nr, typename ...Ps>
constexpr auto derive_impl(Max<Ps...> const&) {
    return Max<decltype(derive_impl<nr>(Ps{}))...>{};
}

// ------------------- partial derivative

template<typename Expr>
auto partial_derive(Expr) {
    return std::make_tuple();
}

template<typename Expr, typename Head, typename ...Args>
auto partial_derive(Expr e, Head head, Args... args) {
    return std::tuple_cat(std::make_tuple(derive(e, head)), partial_derive(e, args...));
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
        return solve_impl<nr>(ln(e2) / ln(P1{}), P2{});
    } else {
        return solve_impl<nr>(pow(e2, 1_c / P2{}), P1{});
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
    return  G{} * ln(T{}) - integrate_impl<nr>(G{} * derive<nr>(e));
}

template <int nr, typename T>
constexpr auto integrate_impl(Sin<T>) {
    auto z = derive(T{}, Var<integer<nr>>{});
    static_assert(isConst<nr, decltype(z)>(), "can't integrate this function");

    return Mul<Const<Number<integer<-1>>>, Cos<T>, Exp<decltype(z), Const<Number<integer<-1>>>>>{};
}
template <int nr, typename T>
constexpr auto integrate_impl(Cos<T>) {
    auto z = derive(T{}, Var<integer<nr>>{});
    static_assert(isConst<nr, decltype(z)>(), "can't integrate this function");

    return Mul<Sin<T>, Exp<decltype(z), Const<Number<integer<-1>>> >>{};
}

template <int nr, typename T>
constexpr auto integrate_impl(Sign<T>) {
    constexpr auto r = sign(T{}) * Var<integer<nr>>{};
    static_assert(std::is_same_v<decltype(full_simplify(derive_impl<nr>(r))), Sign<T>>, "can't integrate function");
    return r;
}

template <int nr, typename T>
constexpr auto integrate_impl(Abs<T>) {
    return abs(integrate_impl<nr>(T{}));
}

template <int nr, typename P1, typename P2>
constexpr auto integrate_impl([[maybe_unused]] Exp<P1, P2> e) {
    static_assert(isConst<nr, P1>() or isConst<nr, P2>());

    if constexpr (isConst<nr, P1>() and isConst<nr, P2>()) {
        return e * Var<integer<nr>>{};
    } else if constexpr (isConst<nr, P2>()) {
        return 1_c/(P2{}+1_c) * pow(P1{}, P2{}+1_c);
    } else if constexpr (is_same_tpl_v<P2, Var<>>) {
        return pow(P1{}, P2{}) / ln(P1{});
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

template <int nr, typename ...Ps>
constexpr auto integrate_impl(Min<Ps...> const&) {
    return Min<decltype(integrate_impl<nr>(Ps{}))...>{};
}

template <int nr, typename ...Ps>
constexpr auto integrate_impl(Max<Ps...> const&) {
    return Max<decltype(integrate_impl<nr>(Ps{}))...>{};
}



template<int nr, typename Expr, typename Head>
constexpr auto integrate(Expr, Head) {
    auto ret = full_simplify(integrate_impl<Head::N::value>(Expr{}));
    if constexpr(nr > 1) {
        return integrate<nr-1>(ret, Head{});
    } else {
        return ret;
    }
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

using detail::operator ""_c;
using detail::derive;

} // namespace d_rive
