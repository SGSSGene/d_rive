#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <string_view>
#include <variant>

namespace d_rive::detail {

// compile time type information
template <typename T>
constexpr auto type_name() -> std::string_view
{
    char const* p = __PRETTY_FUNCTION__;
    while (*p++ != '=');
    for (; *p == ' '; ++p);
    char const* p2 = p;
    int count = 1;
    for (;;++p2)
    {
        switch (*p2)
        {
        case '[':
            ++count;
            break;
        case ']':
            --count;
            if (!count)
                return {p, std::size_t(p2 - p)};
        }
    }
    return {};
}
template <typename T>
constexpr auto type_index = type_name<T>();

// tuple compare

template <typename TTypes, typename UTypes>
constexpr int compare(TTypes const& lhs, UTypes const& rhs) {
    return lhs - rhs;
}

template <size_t start = 0, template<class...> typename TTuple, template<class...> typename UTuple, typename... TTypes, typename... UTypes>
constexpr int tuple_compare(TTuple<TTypes...> const& lhs, UTuple<UTypes...> const& rhs) {
    if constexpr (start < sizeof...(TTypes) and start < sizeof...(UTypes)) {
        auto result = compare(std::get<start>(lhs), std::get<start>(rhs));
        if (result != 0) {
            return result;
        }
        return tuple_compare<start+1>(lhs, rhs);
    } else {
        return int(sizeof...(TTypes)) - int(sizeof...(UTypes));
    }
}

template <template<class...> typename TTuple, template<class...> typename UTuple, typename... TTypes, typename... UTypes>
constexpr bool tuple_compare_eq(TTuple<TTypes...> const& lhs, UTuple<UTypes...> const& rhs) {
    return tuple_compare(lhs, rhs) == 0;
}


// tuple cat type

template<typename ...Args>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<Args>()...));


// tuple index

template<size_t index>
struct tuple_index {
    static constexpr auto op() {
        if constexpr(index == 0) {
            return std::tuple<std::integral_constant<size_t, index>>{};
        } else {
            using Head = std::integral_constant<size_t, index>;
            using Tail = typename tuple_index<index-1>::type;

            return std::tuple_cat(Tail{}, std::tuple<Head>{});
        }
    }
    using type = decltype(op());
};

template<size_t index>
using tuple_index_t = typename tuple_index<index>::type;


// define index and indices

template <auto value>
using index = std::integral_constant<size_t, value>;

template <auto ...values>
using indices = std::integer_sequence<size_t, values...>;

// access value in indices at
template <int index, typename T>
struct get_index;

template <size_t head, auto ...values>
struct get_index<0, indices<head, values...>> {
    static constexpr auto value = head;
};

template <int index, size_t head, auto ...values>
struct get_index<index, indices<head, values...>> {
    static constexpr auto value = get_index<index-1, indices<values...>>::value;
};
/*template <size_t index, auto head, auto ...values>
constexpr auto get_index(indices<head, values...>) {
    if constexpr(index == 0) {
        return head;
    } else {
        return get_index<index-1>(std::declval<indices<values...>>());
    }
}

template <size_t index, typename T>
constexpr size_t get_index_v = get_index<index>(std::declval<T>());*/

template <size_t index, typename T>
constexpr size_t get_index_v = get_index<index, T>::value;


// add value to indices
template <size_t a, typename T>
struct add_indices_impl;

template <size_t a, size_t ...I>
struct add_indices_impl<a, indices<I...>> {
    using type = indices<(I+a)...>;
};

template <size_t a, typename T>
using add_indices = typename add_indices_impl<a, T>::type;


// generate indices
template <size_t start, size_t count>
using gen_indices = add_indices<start, std::make_index_sequence<count>>;

/*template <size_t start, size_t count>
using gen_indices = add_indices<start, std::make_index_sequence<count>>;*/

// cat indices
template <typename ...Args>
struct cat_indices_impl;

template <>
struct cat_indices_impl<> {
    using type = indices<>;
};

template <typename Arg>
struct cat_indices_impl<Arg> {
    using type = Arg;
};

template <size_t ...I, size_t ...J>
struct cat_indices_impl<indices<I...>, indices<J...>> {
    using type = indices<I..., J...>;
};

template <typename E1, typename E2, typename Next, typename ...Args>
struct cat_indices_impl<E1, E2, Next, Args...> {
    using type = typename cat_indices_impl<typename cat_indices_impl<E1, E2>::type, Next, Args...>::type;
};


template <typename ...Args>
using cat_indices = typename cat_indices_impl<Args...>::type;



// remove index
template <typename U, size_t I>
struct remove_index_impl;

template <size_t I>
struct remove_index_impl<indices<>, I> {
    using type = indices<>;
};

template <size_t uI, size_t ...tail, size_t I>
struct remove_index_impl<indices<uI, tail...>, I> {
    using type = cat_indices<indices<uI>, typename remove_index_impl<indices<tail...>, I>::type>;
};

template <size_t ...tail, size_t I>
struct remove_index_impl<indices<I, tail...>, I> {
    using type = typename remove_index_impl<indices<tail...>, I>::type;
};

template <size_t I, typename T>
using remove_index = typename remove_index_impl<T, I>::type;

// remove indices
template <typename U, typename A>
struct remove_indices_impl;

template <typename A>
struct remove_indices_impl<indices<>, A> {
    using type = A;
};

template <size_t I, size_t ...uI, typename A>
struct remove_indices_impl<indices<I, uI...>, A> {
    using type = remove_index<I, typename remove_indices_impl<indices<uI...>, A>::type>;
};

template <typename A, typename U>
using remove_indices = typename remove_indices_impl<A, U>::type;


// tuple to indices

template <typename Tuple>
struct tuple_to_indices;

template <template<class...> typename Tuple, typename... Args>
struct tuple_to_indices<Tuple<Args...>> {
    using type = indices<Args::value...>;
};

template <typename Tuple>
using tuple_to_indices_t = typename tuple_to_indices<Tuple>::type;


// indices to tuple
template <typename Indices>
struct indices_to_tuple;

template <size_t ...I>
struct indices_to_tuple<indices<I...>> {
    using type = std::tuple<std::integral_constant<size_t, I>...>;
};

template <typename Indices>
using indices_to_tuple_t = typename indices_to_tuple<Indices>::type;

// tuple type map

template <typename TupleIn, typename T>
struct tuple_map_indices_impl;

template <typename TupleIn, size_t ...I>
struct tuple_map_indices_impl<TupleIn, indices<I...>> {
    static_assert(((I < std::tuple_size_v<TupleIn>) and ... and true), "indices are out of range");

    using type = std::tuple<std::tuple_element_t<I, TupleIn>...>;
};

template <typename Tuple, typename Map>
using tuple_map_indices_t = typename tuple_map_indices_impl<Tuple, Map>::type;


// tuple map

template <typename TupleIn, typename Map>
struct tuple_map_impl;

template <typename TupleIn, size_t ...I>
struct tuple_map_impl<TupleIn, indices<I...>> {
    static_assert(((I < std::tuple_size_v<TupleIn>) and ... and true), "indices are out of range");

    constexpr static auto op(TupleIn const& _in) {
        using TupleOut = tuple_map_indices_t<TupleIn, indices<I...>>;

        return TupleOut {std::get<I>(_in)...};
    }
};

template <typename Map, typename TupleIn>
constexpr auto tuple_map(TupleIn const& _in) {
    return tuple_map_impl<TupleIn, Map>::op(_in);
}


// tuple head/tail/drop/remove

template <size_t count=1, typename TupleIn>
constexpr auto tuple_head(TupleIn const& _in) {
    static_assert(count <= std::tuple_size_v<TupleIn>, "tuple_head request out of range");
    return tuple_map<gen_indices<0, count>>(_in);
}

template <size_t count=1, typename TupleIn>
constexpr auto tuple_tail(TupleIn const& _in) {
    constexpr auto s = std::tuple_size_v<TupleIn>;
    static_assert(count <= s, "tuple_tail request out of range");
    return tuple_map<gen_indices<s-count, count>>(_in);
}

template <size_t count=1, typename TupleIn>
constexpr auto tuple_drop(TupleIn const& _in) {
    constexpr auto s = std::tuple_size_v<TupleIn>;
    static_assert(count <= s, "tuple_drop request out of range");
    return tuple_map<gen_indices<count, s-count>>(_in);
}

template <size_t count, typename TupleIn>
using tuple_drop_t = decltype(tuple_drop<count>(std::declval<TupleIn>()));

template <typename Type, typename Tuple, size_t ...I>
constexpr auto tuple_type_count(indices<I...>) {
    return (int(std::is_same_v<Type, std::tuple_element_t<I, Tuple>>) + ... + 0);
}

template <typename T, typename Tuple>
using tuple_type_count_t = index<tuple_type_count<T, Tuple>(gen_indices<0, std::tuple_size_v<Tuple>>{})>;


template <size_t index, typename TupleIn>
constexpr auto tuple_remove(TupleIn const& _in) {
    static_assert(index <= std::tuple_size_v<TupleIn>, "tuple_remove request out of range");
    return std::tuple_cat(tuple_head<index>(_in), tuple_drop<index+1>(_in));
}

template <typename indices, typename TupleIn>
constexpr auto tuple_remove(TupleIn const& _in) {
    using allIndices = gen_indices<0, std::tuple_size_v<TupleIn>>;
    using idx = remove_indices<indices, allIndices>;
    return tuple_map<idx>(_in);
}

template <typename indices, typename TupleIn>
using tuple_remove_t = decltype(tuple_remove<indices>(std::declval<TupleIn>()));

// tuple select smallest type

template <size_t i, size_t j, typename Tuple, size_t l>
struct tuple_smallest_element_t_impl {
    static_assert(std::tuple_size_v<Tuple> > 0, "tuple must be larger than zero");

    using T1 = typename tuple_smallest_element_t_impl<i, j+1, Tuple, l>::type;
    using T2 = typename tuple_smallest_element_t_impl<j, j+1, Tuple, l>::type;

    using ti = std::tuple_element_t<i, Tuple>;
    using tj = std::tuple_element_t<j, Tuple>;

    static constexpr bool cond = type_index<ti> <= type_index<tj>;
    using type = std::conditional_t<cond, T1, T2>;
};

template <size_t i, typename Tuple, size_t l>
struct tuple_smallest_element_t_impl<i,  l, Tuple, l> {
    static_assert(std::tuple_size_v<Tuple> > 0, "tuple must be larger than zero");

    using type = index<i>;
};


template <typename Tuple>
constexpr size_t tuple_smallest_element_index = tuple_smallest_element_t_impl<0, 0, Tuple, std::tuple_size_v<Tuple>>::type::value;


// tuple sort
template <typename Tuple, typename Index>
struct tuple_sort_indices_impl {
    static constexpr auto smallestIndex = tuple_smallest_element_index<Tuple>;
    using TupleRemovedIndex = tuple_remove_t<indices<smallestIndex>, Tuple>;
    using RestIndex         = remove_index<get_index_v<smallestIndex, Index>, Index>;
    using RestSorted        = typename tuple_sort_indices_impl<TupleRemovedIndex, RestIndex>::type;
    using type = cat_indices<indices<get_index_v<smallestIndex, Index>>, RestSorted>;
};

template <template<class...> typename T>
struct tuple_sort_indices_impl<T<>, indices<>> {
    using type = indices<>;
};

template <typename T>
using tuple_sort_indices = typename tuple_sort_indices_impl<T, gen_indices<0, std::tuple_size_v<T>>>::type;


template <typename T>
constexpr auto tuple_type_sort(T const& _in) {
    return tuple_map<tuple_sort_indices<T>>(_in);
}

// permute tuples
template <size_t i, size_t j, typename T1, typename T2>
constexpr auto tuple_permute_combine_impl(T1 const& t1, T2 const& t2) {
    return std::make_tuple(std::get<i>(t1), std::get<j>(t2));
}

template <size_t i, typename T1, typename T2, auto ...I>
constexpr auto tuple_permute_impl2(T1 const& t1, T2 const& t2, indices<I...>) {
    return std::make_tuple(tuple_permute_combine_impl<i, I>(t1, t2)...);
}

template <typename T1, typename T2, auto ...I>
constexpr auto tuple_permute_impl(T1 const& t1, T2 const& t2, indices<I...>) {
    return std::tuple_cat(tuple_permute_impl2<I>(t1, t2, gen_indices<0, std::tuple_size_v<T2>>{})...);

}
template <typename T1, typename T2>
constexpr auto tuple_permute(T1 const& t1, T2 const& t2) {
    return std::tuple_cat(tuple_permute_impl(t1, t2, gen_indices<0, std::tuple_size_v<T1>>{}));
}

template <typename T1, typename T2>
using tuple_permute_t = decltype(tuple_permute(std::declval<T1>(), std::declval<T2>()));



// tuple apply each
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <typename Tuple>
struct isTuple : std::false_type {};

template <typename ...Args>
struct isTuple<std::tuple<Args...>> : std::true_type {};

template <typename Tuple>
struct isVoid : std::false_type {};

template <>
struct isVoid<void> : std::true_type {};

namespace detail{
    template<typename>
    using sfinae_true = std::true_type;

    template<class T, class ...Args>
    static auto hasOverload(int) -> sfinae_true<decltype(std::declval<T>()(std::declval<Args>()...))>;

    template<class, class...>
    static auto hasOverload(long) -> std::false_type;
} // detail::

template <class T, class ...Args>
using hasOverloaded = decltype(detail::hasOverload<T, Args...>(0));

namespace detail {
    template <class T, class ...Args>
    constexpr bool hasOverloadedAndNotVoid() {
        if constexpr (not hasOverloaded<T, Args...>::value) {
            return false;
        } else if (isVoid<decltype(std::declval<T>()(std::declval<Args>()...))>::value) {
            return false;
        } else {
            return true;
        }
    }
}
template <class T, class ...Args>
using hasOverloadedAndNotVoid = std::conditional_t<detail::hasOverloadedAndNotVoid<T, Args...>(), std::true_type, std::false_type>;

template <typename Single, typename Overloaded>
constexpr auto tuple_apply_each_impl_single(Single const& t, Overloaded&& overloaded) {
    using hO = hasOverloaded<Overloaded, Single>;
    if constexpr(not hO::value){
        return std::make_tuple();
    } else {
        using R = decltype(overloaded(t));

        static_assert(isTuple<R>::value or isVoid<R>::value, "return type must be tuple or void");

        if constexpr(isTuple<R>::value) {
            return overloaded(t);
        } else {
            overloaded(t);
            return std::make_tuple();
        }
    }
}

template <typename Tuple, typename Overloaded, auto ...I>
constexpr auto tuple_apply_each_impl(Tuple const& _in, Overloaded&& overloaded, indices<I...>) {
    return std::tuple_cat(tuple_apply_each_impl_single(std::get<I>(_in), overloaded)...);
}

template <typename Tuple, typename Overloaded>
constexpr auto tuple_apply_each(Tuple const& _in, Overloaded&& overloaded) {
    return tuple_apply_each_impl(_in, overloaded, gen_indices<0, std::tuple_size_v<Tuple>>{});
}

// tuple apply first that doesn't return void

template <size_t I, typename Tuple, typename Overloaded>
struct tuple_apply_first_impl_compare {
    using Element = std::tuple_element_t<I, Tuple>;
    static constexpr bool value = not hasOverloadedAndNotVoid<Overloaded, Element>::value;
};

template <typename Tuple, typename Overloaded, auto ...I>
constexpr auto tuple_apply_first_impl(Tuple const& _in, Overloaded&& overloaded, indices<I...>) {
    if constexpr(std::tuple_size_v<Tuple> == 0) {
        return _in;
    } else {
        using type  = std::tuple<std::conditional_t<tuple_apply_first_impl_compare<I, Tuple, Overloaded>::value, int, double>...>;

        constexpr size_t index = tuple_smallest_element_index<type>;
        if constexpr (std::is_same_v<std::tuple_element_t<index, type>, int>) {
            return _in;
        } else {
            using P = decltype(std::get<index>(_in));
            static_assert(not isVoid<decltype(overloaded(std::get<index>(_in)))>::value);
            static_assert(hasOverloaded<Overloaded, P>::value);
            static_assert(hasOverloadedAndNotVoid<Overloaded, P>::value);
            using Map   = remove_index<index, gen_indices<0, std::tuple_size_v<Tuple>>>;
            return std::tuple_cat(tuple_map<Map>(_in), overloaded(std::get<index>(_in)));

        }
    }
}

template <typename Tuple, typename Overloaded>
constexpr auto tuple_apply_first(Tuple const& _in, Overloaded&& overloaded) {
    return tuple_apply_first_impl(_in, overloaded, gen_indices<0, std::tuple_size_v<Tuple>>{});
}

template <typename Tuple, typename Overloaded, typename Pairs>
struct tuple_apply_first_pair_impl {
    using Pair = std::false_type;
};

template <typename Tuple, typename Overloaded, typename Head, typename ...Args>
struct tuple_apply_first_pair_impl<Tuple, Overloaded, std::tuple<Head, Args...>> {
    using I1 = std::tuple_element_t<0, Head>;
    using I2 = std::tuple_element_t<1, Head>;
    using E1 = std::tuple_element_t<I1::value, Tuple>;
    using E2 = std::tuple_element_t<I2::value, Tuple>;

    static constexpr bool value = hasOverloadedAndNotVoid<Overloaded, E1, E2>::value and I1::value != I2::value;

    using Pair = std::conditional_t<value, Head, typename tuple_apply_first_pair_impl<Tuple, Overloaded, std::tuple<Args...>>::Pair>;
};

template <typename Tuple, typename Overloaded>
constexpr auto tuple_apply_first_pair(Tuple const& _in, Overloaded&& overloaded) {
    if constexpr(std::tuple_size_v<Tuple> == 0) {
        return _in;
    } else {

        using Ids = gen_indices<0, std::tuple_size_v<Tuple>>;
        using Pairs = tuple_permute_t<indices_to_tuple_t<Ids>, indices_to_tuple_t<Ids>>;

        using Pair = typename tuple_apply_first_pair_impl<Tuple, Overloaded, Pairs>::Pair;
        if constexpr (std::is_same_v<Pair, std::false_type>) {
            return _in;
        } else {
            using I1 = std::tuple_element_t<0, Pair>;
            using I2 = std::tuple_element_t<1, Pair>;
            using Map = remove_indices<indices<I1::value, I2::value>, gen_indices<0, std::tuple_size_v<Tuple>>>;
            return std::tuple_cat(tuple_map<Map>(_in), overloaded(std::get<I1::value>(_in), std::get<I2::value>(_in)));
        }
    }
}

} // namespace d_rive::detail
