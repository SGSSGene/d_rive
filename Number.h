#pragma once

#include <limits>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <ostream>
#include <type_traits>

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

    static_assert(std::numeric_limits<lower_type>::min() <= v, "underflow");
    static_assert(std::numeric_limits<lower_type>::max() >= v, "overflow");

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
    return integer<std::lcm(int64_t(v1), int64_t(v2))>{};
}

template<auto v1, auto v2>
constexpr auto gcd(integer<v1>, integer<v2>) {
    return integer<std::gcd(int64_t(v1), int64_t(v2))>{};
}

template <long double v = 0.l>
struct Number {
    static constexpr long double value() {
        return v;
    }
};

template <typename T>
struct is_number : std::false_type {};

template <auto v>
struct is_number<Number<v>> : std::true_type {};


template <typename T>
concept CNumber = is_number<T>::value;

template<auto v1, auto v2>
constexpr auto operator+(Number<v1>, Number<v2>) {
    return Number<v1+v2>{};
}
template<auto v1, auto v2>
constexpr auto operator-(Number<v1>, Number<v2>) {
    return Number<v1-v2>{};
}
template<auto v1, auto v2>
constexpr auto operator*(Number<v1>, Number<v2>) {
    return Number<v1*v2>{};
}
template<auto v1, auto v2>
constexpr auto operator/(Number<v1>, Number<v2>) {
    static_assert(v2 != 0.l, "Can't divide by zero");
    return Number<v1/v2>{};
}
template<auto v>
constexpr auto operator-(Number<v>) {
    return Number<-v>{};
}

constexpr long double pow(long double base, int64_t iexp) {
    if (iexp < 0) {
        return 1.l/pow(base, -iexp);
    }
    if (iexp == 0) {
        return 1.l;
    }
    auto v = pow(base, iexp/2);
    if (iexp % 2 == 0) {
        return v * v;
    }
    return v * v * base;
}

template<auto v, auto n>
constexpr auto pow(Number<v>, integer<n>) {
    return Number<pow(v, n)>{};
}

template<auto n, typename Backtrace = std::tuple<>, auto v1, auto v2>//typename TN1, typename TD1, typename TN2, typename TD2>
constexpr auto n_root_impl(Number<v1> _number, integer<n>, Number<v2> _guess) {
    constexpr auto s1 = Number<(n-1)*v2>{};
    constexpr auto s2 = _number / pow(_guess, integer<n-1>{});
    constexpr auto r = (s1 + s2) * Number<1.l/n>{};

    using R = decltype(r);
    using L = decltype(_guess);

    if constexpr(std::is_same_v<R, L> or tuple_type_count_t<R, Backtrace>::value > 0) {
        return r;
    } else {
        return n_root_impl<n, tuple_cat_t<Backtrace, std::tuple<R>>>(_number, integer<n>{}, r);
    }
}

template<auto n, auto v>
constexpr auto n_root(Number<v> _number, integer<n> _n) {
    return n_root_impl<n>(_number, _n, Number<1.l / (10.l * 2)>{});
}

template<auto v>
constexpr auto inv(Number<v>) {
    return Number<1.l / v>{};
}

template<auto v1, auto v2>
constexpr auto pow(Number<v1> _v1, Number<v2>) {
    //!TODO there must be a better solution...
//    constexpr auto d = 1000000000000000000ull;
    constexpr auto d = 6ll;

    constexpr auto n = int64_t(v2 * d);
    using TD2 = integer<d>;
    using TN2 = integer<n>;
    if constexpr (v2 > 0) {
        return pow(n_root(_v1, TD2{}), TN2{});
    } else {
        return inv(pow(n_root(_v1, TD2{}), -TN2{}));
    }
}

template<uint64_t N = 1, typename Backtrace = std::tuple<>, auto v1, auto v2>
constexpr auto ln_impl(Number<v1> _n, Number<v2> _last) {
    using R = Number<pow((v1-1)/(v1+1), N) * 2.l/N + v2>;
    using L = Number<v1>;
    if constexpr(std::is_same_v<R, L> or tuple_type_count_t<R, Backtrace>::value > 0) {
        (void)_n;
        return R{};
    } else {
        return ln_impl<N+2, tuple_cat_t<Backtrace, std::tuple<R>>>(_n, R{});
    }
}

constexpr auto ln(CNumber auto _n) {
    return ln_impl(_n, Number<>{});
}

constexpr auto log(CNumber auto _base, CNumber auto _n) {
    return ln(_n) / ln(_base);
}

namespace number {

template<auto v1>
constexpr auto sign(Number<v1> _n) {
    if constexpr (v1 > 0) {
        return Number<1.l>{};
    } else if constexpr (v1 < 0) {
        return Number<-1.l>{};
    } else {
        return Number<>{};
    }
}

template<auto v1>
constexpr auto abs(Number<v1> _n) {
    if constexpr (v1 >= 0) return _n;
    else return -_n;
}

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


template<uint64_t f_x, typename Backtrace = std::tuple<>, CNumber X, CNumber NumbSum, CNumber Numb>
constexpr auto sin_impl(X, NumbSum, Numb) {
    constexpr auto newNumb = -Numb{} * X{} * X{} / Number<f_x*1.l>{} / Number<f_x-1.l>{};
    constexpr auto newNumbSum = NumbSum{} + newNumb;

    using R = decltype(newNumbSum);
    using L = X;
    if constexpr(std::is_same_v<R, L> or tuple_type_count_t<R, Backtrace>::value > 0) {
        return newNumbSum;
    } else {
        return sin_impl<f_x+2, tuple_cat_t<Backtrace, std::tuple<R>>>(X{}, newNumbSum, newNumb);
    }
}

constexpr auto sin(CNumber auto x) {
    return sin_impl<3>(x, x, x);
}

constexpr auto cos(CNumber auto x) {
    return sin_impl<2>(x, Number<1.l>{}, Number<1.l>{});
}


inline constexpr auto pi = Number<3.141592653589793238l>{};
} // namespace d_rive::detail
