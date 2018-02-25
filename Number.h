#pragma once

#include <limits>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <ostream>

#include "tuple.h"

template<>
struct std::is_integral<__int128_t> : std::true_type {};

namespace d_rive::detail {

template<__int128_t v=0>
struct integer {
    using type = __int128_t;
    using lower_type = int64_t;

    inline constexpr static type lower_type_max = std::numeric_limits<lower_type>::max();
    inline constexpr static type value = v;

    constexpr bool operator==(type _v) const {
        return _v == v;
    }
    constexpr bool operator!=(type _v) const {
        return _v != v;
    }

};
template<auto v1, auto v2>
constexpr auto operator+(integer<v1>, integer<v2>) {
    return integer<v1 + v2>{};
}

template<auto v1, auto v2>
constexpr auto operator-(integer<v1>, integer<v2>) {
    return integer<v1 - v2>{};
}

template<auto v1, auto v2>
constexpr auto operator*(integer<v1>, integer<v2>) {
    return integer<v1 * v2>{};
}

template<auto v1, auto v2>
constexpr auto operator/(integer<v1>, integer<v2>) {
    static_assert(v2 != 0);
    using std::gcd;
    static_assert(gcd(v1, v2) == v2 or gcd(v1, v2) == -v2);
    return integer<v1 / v2>{};
}

template<auto v1, auto v2>
constexpr auto unsafe_div(integer<v1>, integer<v2>) {
    static_assert(v2 != 0);
    using std::gcd;
    return integer<v1 / v2>{};
}


template<auto v>
constexpr auto sign(integer<v>) {
    if constexpr(v == 0) {
        return integer<0>{};
    } else if constexpr(v > 0) {
        return integer<1>{};
    } else {
        return integer<-1>{};
    }
}

template<auto v>
constexpr auto abs(integer<v> _v) {
    return sign(_v) * _v;
}

template<auto v>
constexpr auto operator-(integer<v>) {
    return integer<-v>{};
}

template<auto v1, auto v2>
constexpr auto max(integer<v1>, integer<v2>) {
    using std::max;
    return integer<max(v1, v2)>{};
}

template<auto v1, auto v2>
constexpr auto lcm(integer<v1>, integer<v2>) {
    using std::lcm;
    return integer<lcm(v1, v2)>{};
}

template<auto v1, auto v2>
constexpr auto gcd(integer<v1>, integer<v2>) {
    using std::gcd;
    return integer<gcd(v1, v2)>{};
}

template <typename TN = integer<0>, typename TD = integer<1>>
struct Number {
    static_assert(TD::value != 0);

    using N = TN;
    using D = TD;

    static constexpr double value() {
        return double(N::value) / double(D::value);
    }
};

template<typename TN1, typename TD1>
constexpr auto normalize(Number<TN1, TD1>) {
    constexpr auto _gcd = gcd(TN1{}, TD1{});
    constexpr auto s    = sign(TN1{}) * sign(TD1{});

    using N = decltype(abs(TN1{}/_gcd) * s);
    using D = decltype(abs(TD1{}/_gcd));

    constexpr auto kurz_N = unsafe_div(abs(N{}), integer<integer<>::lower_type_max>{}) + integer<1>{};
    constexpr auto kurz_D = unsafe_div(abs(D{}), integer<integer<>::lower_type_max>{}) + integer<1>{};

    constexpr auto in_limits = max(kurz_N, kurz_D);

    return Number<decltype(unsafe_div(N{}, in_limits)), decltype(unsafe_div(D{}, in_limits))>{};
}

template<typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto operator+(Number<TN1, TD1>, Number<TN2, TD2>) {
    using D = decltype(lcm(TD1{}, TD2{}));
    using N = decltype(TN1{} * (D{}/TD1{}) + TN2{} * (D{}/TD2{}));
    return normalize(Number<N, D>{});
}

template<typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto operator-(Number<TN1, TD1>, Number<TN2, TD2>) {
    using D = decltype(lcm(TD1{}, TD2{}));
    using N = decltype(TN1{} * (D{}/TD1{}) - TN2{} * (D{}/TD2{}));
    return normalize(Number<N, D>{});
}

template<typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto operator*(Number<TN1, TD1>, Number<TN2, TD2>) {
    using N = decltype(TN1{} * TN2{});
    using D = decltype(TD1{} * TD2{});
    return normalize(Number<N, D>{});
}

template<typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto operator/(Number<TN1, TD1>, Number<TN2, TD2>) {
    using N = decltype(TN1{} * TD2{});
    using D = decltype(TD1{} * TN2{});
    return normalize(Number<N, D>{});
}

template<typename TN1, typename TD1>
constexpr auto operator-(Number<TN1, TD1>) {
    return Number<decltype(-TN1{}), TD1>{};
}

template<typename TN1, typename TD1, auto n>
constexpr auto pow(Number<TN1, TD1> _number, integer<n>) {
    static_assert(n != 0 or TN1{} != 0);
    static_assert(n >= 0);
    if constexpr (n == 0) {
        (void)_number;
        return Number<integer<1>>{};
    } else if constexpr (n == 1) {
        return _number;
    } else {
        return _number * pow(_number, integer<n-1>{});
    }
}

template<auto n, typename Backtrace = std::tuple<>, typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto n_root_impl(Number<TN1, TD1> _number, integer<n> _n, Number<TN2, TD2> _guess) {
    constexpr auto s1 = normalize(Number<integer<n - 1>>{} * _guess);
    constexpr auto s2 = normalize(_number / pow(_guess, integer<n-1>{}));
    constexpr auto r = normalize((s1 + s2) * Number<integer<1>, integer<n>>{});

    using R = decltype(r);
    using L = decltype(_guess);

    if constexpr(std::is_same_v<R, L> or tuple_type_count_t<R, Backtrace>::value > 0) {
        (void)_n;
        return r;
    } else {
        return n_root_impl<n, tuple_cat_t<Backtrace, std::tuple<R>>>(_number, _n, r);
    }
}

template<auto n, typename TN1, typename TD1>
constexpr auto n_root(Number<TN1, TD1> _number, integer<n> _n) {
    return n_root_impl<n>(_number, _n, normalize(Number<integer<1>, decltype(TD1{}*integer<2>{})>{}));
}

template<typename TN1, typename TD1>
constexpr auto inv(Number<TN1, TD1>) {
    return normalize(Number<TD1, TN1>{});
}

template<typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto pow(Number<TN1, TD1> _v1, Number<TN2, TD2>) {
    if constexpr(TN2::value > 0) {
        return pow(n_root(_v1, TD2{}), TN2{});
    } else {
        return inv(pow(n_root(_v1, TD2{}), -TN2{}));
    }
}

template<typename N = integer<1>, typename Backtrace = std::tuple<>, typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto ln_impl(Number<TN1, TD1> _n, Number<TN2, TD2> _last) {
    constexpr auto t1 = Number<TN1, TD1>{} - Number<integer<1>>{};
    constexpr auto t2 = Number<TN1, TD1>{} + Number<integer<1>>{};

    constexpr auto r = pow(t1/t2, N{}) * Number<integer<2>, N>{} + _last;

    using R = decltype(r);
    using L = Number<TN1, TD1>;
    if constexpr(std::is_same_v<R, L> or tuple_type_count_t<R, Backtrace>::value > 0) {
        (void)_n;
        return r;
    } else {
        using Next = decltype(N{} + integer<2>{});
        return ln_impl<Next, tuple_cat_t<Backtrace, std::tuple<R>>>(_n, r);
    }
}

template<typename TN1, typename TD1>
constexpr auto ln(Number<TN1, TD1> _n) {
    return ln_impl(_n, Number<>{});
}

template<typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto log(Number<TN2, TD2> _base, Number<TN1, TD1> _n) {
    return ln(_n) / ln(_base);
}

template<typename TN1, typename TD1>
constexpr auto sign(Number<TN1, TD1> _n) {
    if constexpr (TN1::value > 0) {
        return Number<integer<1>>{};
    } else if constexpr (TN1::value < 0) {
        return Number<integer<-1>>{};
    } else {
        return Number<>{};
    }
}

template<typename TN1, typename TD1>
constexpr auto abs(Number<TN1, TD1> _n) {
    return Number<decltype(abs(TN1{})), TD1>{};
}

namespace number {
template<typename Head, typename ...N>
constexpr auto min(Head, N...) {
    if constexpr(sizeof...(N) == 0) {
        return Head{};
    } else {
        constexpr auto e = min(N{}...);
        constexpr auto s = sign(Head{} - e);
        if constexpr (s.value() <= 0) {
            return Head{};
        } else {
            return e;
        }
    }
}

template<typename Head, typename ...N>
constexpr auto max(Head, N...) {
    if constexpr(sizeof...(N) == 0) {
        return Head{};
    } else {
        constexpr auto e = max(N{}...);
        constexpr auto s = sign(Head{} - e);
        if constexpr (s.value() >= 0) {
            return Head{};
        } else {
            return e;
        }
    }
}



}




template<int f_x, typename Backtrace = std::tuple<>, typename X, typename NumbSum, typename Numb>
constexpr auto sin_impl(X, NumbSum, Numb) {
    constexpr auto newNumb = normalize(-Numb{} * X{} * X{} / Number<integer<f_x>>{} / Number<integer<f_x-1>>{});
    constexpr auto newNumbSum = normalize(NumbSum{} + newNumb);
    constexpr auto r = newNumbSum;

    using R = decltype(r);
    using L = X;
    if constexpr(std::is_same_v<R, L> or tuple_type_count_t<R, Backtrace>::value > 0) {
        return r;
    } else {
        return sin_impl<f_x+2, tuple_cat_t<Backtrace, std::tuple<R>>>(X{}, newNumbSum, newNumb);
    }
}

template<typename TN1, typename TD1>
constexpr auto sin(Number<TN1, TD1> x) {
    return sin_impl<3>(x, x, x);
}

template<int f_x, typename Backtrace = std::tuple<>, typename X, typename NumbSum, typename Numb>
constexpr auto cos_impl(X, NumbSum, Numb) {
    constexpr auto newNumb = normalize(-Numb{} * X{} * X{} / Number<integer<f_x>>{} / Number<integer<f_x-1>>{});
    constexpr auto newNumbSum = normalize(NumbSum{} + newNumb);
    constexpr auto r = newNumbSum;

    using R = decltype(r);
    using L = X;
    if constexpr(std::is_same_v<R, L> or tuple_type_count_t<R, Backtrace>::value > 0) {
        return r;
    } else {
        return cos_impl<f_x+2, tuple_cat_t<Backtrace, std::tuple<R>>>(X{}, newNumbSum, newNumb);
    }
}

template<typename TN1, typename TD1>
constexpr auto cos(Number<TN1, TD1> x) {
    return sin_impl<2>(x, Number<integer<1>>{}, Number<integer<1>>{});
}


inline constexpr auto pi = Number<integer<3141592653589793238>,
                                  integer<1000000000000000000>>{};

} // namespace d_rive::detail
