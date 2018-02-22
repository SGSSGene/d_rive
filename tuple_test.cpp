#include "tuple.h"

using namespace d_rive::detail;

// check typeidx_v
static_assert(type_index<double> == type_index<double>);
static_assert(type_index<double> != type_index<float>);
static_assert(type_index<double> != type_index<int>);
static_assert(type_index<double> != type_index<std::string>);
static_assert(type_index<double> <= type_index<int>);
static_assert(type_index<double> <  type_index<int>);



// check compare
static_assert(compare(0, 0) == 0);
static_assert(compare(1, 0) > 0);
static_assert(compare(10, 0) > 0);
static_assert(compare(1, 20) < 0);
static_assert(compare(10, 21) < 0);



// check tuple_compare
static_assert(tuple_compare(std::make_tuple(),  std::make_tuple()) == 0);

static_assert(tuple_compare(std::make_tuple(),  std::make_tuple())  == 0);
static_assert(tuple_compare(std::make_tuple(1), std::make_tuple(1)) == 0);
static_assert(tuple_compare(std::make_tuple(1, 1), std::make_tuple(1, 1)) == 0);
static_assert(tuple_compare(std::make_tuple(2, 1), std::make_tuple(2, 1)) == 0);

static_assert(tuple_compare(std::make_tuple(),  std::make_tuple(1)) < 0);
static_assert(tuple_compare(std::make_tuple(1), std::make_tuple(2)) < 0);
static_assert(tuple_compare(std::make_tuple(1), std::make_tuple(1, 1)) < 0);
static_assert(tuple_compare(std::make_tuple(1, 1), std::make_tuple(1, 2)) < 0);
static_assert(tuple_compare(std::make_tuple(2, 1), std::make_tuple(2, 2)) < 0);

static_assert(tuple_compare(std::make_tuple(1), std::make_tuple()) > 0);
static_assert(tuple_compare(std::make_tuple(2), std::make_tuple(1)) > 0);
static_assert(tuple_compare(std::make_tuple(1, 1), std::make_tuple(1)) > 0);
static_assert(tuple_compare(std::make_tuple(2, 1), std::make_tuple(1, 2)) > 0);


// check tuple_compare_eq
static_assert(tuple_compare_eq(std::make_tuple(),  std::make_tuple()));

static_assert(tuple_compare_eq(std::make_tuple(),  std::make_tuple()));
static_assert(tuple_compare_eq(std::make_tuple(1), std::make_tuple(1)));
static_assert(tuple_compare_eq(std::make_tuple(1, 1), std::make_tuple(1, 1)));
static_assert(tuple_compare_eq(std::make_tuple(2, 1), std::make_tuple(2, 1)));

static_assert(tuple_compare_eq(std::make_tuple(),  std::make_tuple(1)) == false);
static_assert(tuple_compare_eq(std::make_tuple(1), std::make_tuple()) == false);
static_assert(tuple_compare_eq(std::make_tuple(1), std::make_tuple(2)) == false);
static_assert(tuple_compare_eq(std::make_tuple(1), std::make_tuple(1, 1)) == false);
static_assert(tuple_compare_eq(std::make_tuple(1, 1), std::make_tuple(1)) == false);
static_assert(tuple_compare_eq(std::make_tuple(1, 1), std::make_tuple(1, 2)) == false);
static_assert(tuple_compare_eq(std::make_tuple(2, 1), std::make_tuple(1, 2)) == false);
static_assert(tuple_compare_eq(std::make_tuple(2, 1), std::make_tuple(2, 2)) == false);


// check access to index
static_assert(get_index_v<1, indices<5, 4, 3>> == 4);
static_assert(get_index_v<0, indices<5, 4, 3>> == 5);
static_assert(get_index_v<2, indices<5, 4, 3>> == 3);


// check add_indices
static_assert(std::is_same_v<indices<>, add_indices<0, indices<>>>);
static_assert(std::is_same_v<indices<>, add_indices<2, indices<>>>);

static_assert(std::is_same_v<indices<2, 3, 4>, add_indices<2, indices<0, 1, 2>>>);


// check generate indices
static_assert(std::is_same_v<indices<>, gen_indices<0, 0>>);
static_assert(std::is_same_v<indices<>, gen_indices<2, 0>>);
static_assert(std::is_same_v<indices<0, 1, 2>, gen_indices<0, 3>>);
static_assert(std::is_same_v<indices<2, 3, 4>, gen_indices<2, 3>>);


// check cat_indices
static_assert(std::is_same_v<indices<>, cat_indices<>>);
static_assert(std::is_same_v<indices<>, cat_indices<indices<>>>);
static_assert(std::is_same_v<indices<>, cat_indices<indices<>, indices<>>>);

static_assert(std::is_same_v<indices<0>, cat_indices<indices<0>>>);
static_assert(std::is_same_v<indices<0, 1>, cat_indices<indices<0, 1>>>);
static_assert(std::is_same_v<indices<0, 1, 2, 3>, cat_indices<indices<0, 1>, indices<2, 3>>>);
static_assert(std::is_same_v<indices<0, 1, 2, 3, 4, 5>, cat_indices<indices<0, 1>, indices<2, 3>, indices<4, 5>>>);


// check remove_index
static_assert(std::is_same_v<indices<>, remove_index<0, indices<>>>);
static_assert(std::is_same_v<indices<>, remove_index<0, indices<0>>>);
static_assert(std::is_same_v<indices<>, remove_index<0, indices<0, 0, 0>>>);
static_assert(std::is_same_v<indices<1>, remove_index<0, indices<1>>>);

static_assert(std::is_same_v<indices<1, 2>, remove_index<0, indices<0, 1, 2>>>);
static_assert(std::is_same_v<indices<0, 2>, remove_index<1, indices<0, 1, 2>>>);
static_assert(std::is_same_v<indices<0, 1>, remove_index<2, indices<0, 1, 2>>>);


// check remove_indices
static_assert(std::is_same_v<indices<>, remove_indices<indices<>, indices<>>>);
static_assert(std::is_same_v<indices<>, remove_indices<indices<0>, indices<0>>>);
static_assert(std::is_same_v<indices<>, remove_indices<indices<0, 1, 2>, indices<>>>);
static_assert(std::is_same_v<indices<>, remove_indices<indices<0, 0>, indices<>>>);
static_assert(std::is_same_v<indices<>, remove_indices<indices<0, 1, 2>, indices<0>>>);
static_assert(std::is_same_v<indices<>, remove_indices<indices<0, 0>, indices<0>>>);

static_assert(std::is_same_v<indices<0>, remove_indices<indices<1, 2>, indices<0, 1, 2>>>);
static_assert(std::is_same_v<indices<1>, remove_indices<indices<0, 2>, indices<0, 1, 2>>>);
static_assert(std::is_same_v<indices<2>, remove_indices<indices<0, 1>, indices<0, 1, 2>>>);



// check tuple_to_indices_t
// converts a tuple<index...> to integral_sequence<size_t, ...>
static_assert(std::is_same_v<tuple_to_indices_t<std::tuple<>>,                                 indices<>>);
static_assert(std::is_same_v<tuple_to_indices_t<std::tuple<d_rive::detail::index<0>>>,                  indices<0>>);
static_assert(std::is_same_v<tuple_to_indices_t<std::tuple<d_rive::detail::index<1>>>,                  indices<1>>);
static_assert(std::is_same_v<tuple_to_indices_t<std::tuple<d_rive::detail::index<1>, d_rive::detail::index<2>>>, indices<1, 2>>);


// check indices_to_tuple_t
static_assert(std::is_same_v<indices_to_tuple_t<indices<>>,     std::tuple<>>);
static_assert(std::is_same_v<indices_to_tuple_t<indices<0>>,    std::tuple<d_rive::detail::index<0>>>);
static_assert(std::is_same_v<indices_to_tuple_t<indices<1>>,    std::tuple<d_rive::detail::index<1>>>);
static_assert(std::is_same_v<indices_to_tuple_t<indices<1, 2>>, std::tuple<d_rive::detail::index<1>, d_rive::detail::index<2>>>);


// check tuple_map_indices_t
// reorders the entry of a tuple
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<>, indices<>>, std::tuple<>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<int>, indices<>>, std::tuple<>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<int, double>, indices<>>, std::tuple<>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<int>, indices<0>>, std::tuple<int>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<int>, indices<0, 0>>, std::tuple<int, int>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<double, int>, indices<0>>, std::tuple<double>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<int, double>, indices<0, 0>>, std::tuple<int, int>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<double, int>, indices<1>>, std::tuple<int>>);
static_assert(std::is_same_v<tuple_map_indices_t<std::tuple<int, double>, indices<1, 0>>, std::tuple<double, int>>);


// check tuple_map
// reorders the entry of a tuple
static_assert(tuple_map<indices<0, 1, 2>>(std::make_tuple(0, 1, 2)) == std::make_tuple(0, 1, 2));
static_assert(tuple_map<indices<1, 1, 2>>(std::make_tuple(0, 1, 2)) == std::make_tuple(1, 1, 2));
static_assert(tuple_map<indices<1, 2, 1>>(std::make_tuple(0, 1, 2)) == std::make_tuple(1, 2, 1));
constexpr char s1[] = "1";
static_assert(tuple_map<indices<0, 1, 2>>(std::make_tuple(0.1, s1, 2.f)) == std::make_tuple(0.1, s1,  2.f));
static_assert(tuple_map<indices<1, 1, 2>>(std::make_tuple(0.1, s1, 2.f)) == std::make_tuple(s1,  s1,  2.f));
static_assert(tuple_map<indices<1, 2, 1>>(std::make_tuple(0.1, s1, 2.f)) == std::make_tuple(s1,  2.f, s1));


// check tuple_cat_t
static_assert(std::is_same_v<tuple_cat_t<>, std::tuple<>>);
static_assert(std::is_same_v<tuple_cat_t<std::tuple<>>, std::tuple<>>);
static_assert(std::is_same_v<tuple_cat_t<std::tuple<>, std::tuple<>>, std::tuple<>>);
static_assert(std::is_same_v<tuple_cat_t<std::tuple<int>>, std::tuple<int>>);
static_assert(std::is_same_v<tuple_cat_t<std::tuple<int>, std::tuple<>>, std::tuple<int>>);
static_assert(std::is_same_v<tuple_cat_t<std::tuple<>, std::tuple<int>>, std::tuple<int>>);
static_assert(std::is_same_v<tuple_cat_t<std::tuple<>, std::tuple<int>, std::tuple<>>, std::tuple<int>>);
static_assert(std::is_same_v<tuple_cat_t<std::tuple<double, double, double>, std::tuple<int, double>, std::tuple<char>>, std::tuple<double, double, double, int, double, char>>);



static_assert(tuple_head<0>(std::make_tuple()) == std::make_tuple());
static_assert(tuple_head<0>(std::make_tuple(0)) == std::make_tuple());
static_assert(tuple_head<1>(std::make_tuple(0)) == std::make_tuple(0));
static_assert(tuple_head<1>(std::make_tuple(3, 2, 1)) == std::make_tuple(3));
static_assert(tuple_head<2>(std::make_tuple(2, 1)) == std::make_tuple(2, 1));



static_assert(tuple_tail<0>(std::make_tuple()) == std::make_tuple());
static_assert(tuple_tail<0>(std::make_tuple(0)) == std::make_tuple());
static_assert(tuple_tail<1>(std::make_tuple(0)) == std::make_tuple(0));
static_assert(tuple_tail<1>(std::make_tuple(3, 2, 1)) == std::make_tuple(1));
static_assert(tuple_tail<2>(std::make_tuple(2, 1)) == std::make_tuple(2, 1));



static_assert(tuple_drop<0>(std::make_tuple()) == std::make_tuple());
static_assert(tuple_drop<0>(std::make_tuple(0)) == std::make_tuple(0));
static_assert(tuple_drop<1>(std::make_tuple(0)) == std::make_tuple());
static_assert(tuple_drop<1>(std::make_tuple(3, 2, 1)) == std::make_tuple(2, 1));
static_assert(tuple_drop<2>(std::make_tuple(2, 1)) == std::make_tuple());



static_assert(tuple_remove<0>(std::make_tuple(0)) == std::make_tuple());
static_assert(tuple_remove<0>(std::make_tuple(3, 2, 1)) == std::make_tuple(2, 1));
static_assert(tuple_remove<1>(std::make_tuple(3, 2, 1)) == std::make_tuple(3, 1));
static_assert(tuple_remove<2>(std::make_tuple(3, 2, 1)) == std::make_tuple(3, 2));


static_assert(tuple_remove<indices<>>(std::make_tuple()) == std::make_tuple());
static_assert(tuple_remove<indices<>>(std::make_tuple(0)) == std::make_tuple(0));
static_assert(tuple_remove<indices<0>>(std::make_tuple(0)) == std::make_tuple());
static_assert(tuple_remove<indices<0>>(std::make_tuple(3, 2, 1)) == std::make_tuple(2, 1));
static_assert(tuple_remove<indices<1>>(std::make_tuple(3, 2, 1)) == std::make_tuple(3, 1));
static_assert(tuple_remove<indices<2>>(std::make_tuple(3, 2, 1)) == std::make_tuple(3, 2));
static_assert(tuple_remove<indices<0, 1>>(std::make_tuple(3, 2, 1)) == std::make_tuple(1));
static_assert(tuple_remove<indices<0, 2>>(std::make_tuple(3, 2, 1)) == std::make_tuple(2));
static_assert(tuple_remove<indices<1, 2>>(std::make_tuple(3, 2, 1)) == std::make_tuple(3));


// find smallest index by type
static_assert(tuple_smallest_element_index<std::tuple<int>> == 0);
static_assert(tuple_smallest_element_index<std::tuple<int, int>> == 0);

//!TODO currently assuming datatypes are ordered by name
static_assert(tuple_smallest_element_index<std::tuple<int, double>> == 1);
static_assert(tuple_smallest_element_index<std::tuple<double, int>> == 0);
static_assert(tuple_smallest_element_index<std::tuple<double, int, double>> == 0);
static_assert(tuple_smallest_element_index<std::tuple<int, double, double>> == 1);


// sort tuple by type and retrieve indices
static_assert(std::is_same_v<tuple_sort_indices<std::tuple<>>, indices<>>);
static_assert(std::is_same_v<tuple_sort_indices<std::tuple<int>>, indices<0>>);
static_assert(std::is_same_v<tuple_sort_indices<std::tuple<int, int>>, indices<0, 1>>);

//!TODO currently assuming datatypes are ordered by name
static_assert(std::is_same_v<tuple_sort_indices<std::tuple<double, int>>, indices<0, 1>>);
static_assert(std::is_same_v<tuple_sort_indices<std::tuple<int, int>>, indices<0, 1>>);
static_assert(std::is_same_v<tuple_sort_indices<std::tuple<int, double>>, indices<1, 0>>);


// test type sort
static_assert(tuple_type_sort(std::make_tuple()) == std::make_tuple());

static_assert(tuple_type_sort(std::make_tuple(0, 1, 2)) == std::make_tuple(0, 1, 2));
static_assert(tuple_type_sort(std::make_tuple(2, 1, 0)) == std::make_tuple(2, 1, 0));

//!TODO currently assuming datatypes are ordered by name
static_assert(tuple_type_sort(std::make_tuple(0, 1.)) == std::make_tuple(1., 0));
static_assert(tuple_type_sort(std::make_tuple(1., 0)) == std::make_tuple(1., 0));


// test tuple_permute
static_assert(tuple_permute(std::make_tuple(), std::make_tuple())
    == std::make_tuple());
static_assert(tuple_permute(std::make_tuple(1, 2, 3), std::make_tuple())
    == std::make_tuple());
static_assert(tuple_permute(std::make_tuple(), std::make_tuple(1, 2, 3))
    == std::make_tuple());
static_assert(tuple_permute(std::make_tuple(1), std::make_tuple(1, 2, 3))
    == std::make_tuple(std::make_tuple(1, 1),
                       std::make_tuple(1, 2),
                       std::make_tuple(1, 3)));

static_assert(tuple_permute(std::make_tuple(1, 2), std::make_tuple(5, 6))
    == std::make_tuple(std::make_tuple(1, 5),
                       std::make_tuple(1, 6),
                       std::make_tuple(2, 5),
                       std::make_tuple(2, 6)));



//test tuple_apply_each

// increase every element by 2
static_assert(tuple_apply_each(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](auto x) { return std::make_tuple(x*2); }
}) == std::make_tuple(2, 4., 6, 8.));

// increase every int by 2 and double by 3
static_assert(tuple_apply_each(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](int x)    { return std::make_tuple(x*2); },
    [](double x) { return std::make_tuple(x*3.); }
}) == std::make_tuple(2, 6., 6, 12.));

// drop all elements
static_assert(tuple_apply_each(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](auto) {},
}) == std::make_tuple());


// drop all doubles
static_assert(tuple_apply_each(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](int x) { return std::make_tuple(x*2); },
    [](double) {},
}) == std::make_tuple(2, 6));


// pass elements through
static_assert(tuple_apply_each(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](int x) { return std::make_tuple(x*2); },
    [](auto x) { return std::make_tuple(x); },
}) == std::make_tuple(2, 2., 6, 4.));

// drop all ints with constexpr
static_assert(tuple_apply_each(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](auto x) {
        using T = decltype(x);
        if constexpr(std::is_same_v<T, double>) {
            return std::make_tuple(x*2.);
        }
    },
}) == std::make_tuple(4., 8.));

// drop all ints with constexpr
static_assert(tuple_apply_each(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](auto x) {
        using T = decltype(x);
        if constexpr(std::is_same_v<T, double>) {
            return std::make_tuple(x*2.);
        }
    },
}) == std::make_tuple(4., 8.));

// check type is being droped if not available
static_assert(tuple_apply_each(std::make_tuple(std::true_type{}, std::false_type{}), overloaded {
    [](std::true_type x) { return std::make_tuple(x); },
}) == std::make_tuple(std::true_type{}));



// apply change only to first double
static_assert(tuple_apply_first(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](auto x) {
        using T = decltype(x);
        if constexpr(std::is_same_v<T, double>) {
            return std::make_tuple(x*5.);
        }
    },
}) == std::make_tuple(1, 3, 4., 10.));

// apply no match
static_assert(tuple_apply_first(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](auto) {}
}) == std::make_tuple(1, 2., 3, 4.));

static_assert(tuple_apply_first(std::make_tuple(), overloaded {
    [](auto) {}
}) == std::make_tuple());

// check type is being droped if not available
static_assert(tuple_apply_first(std::make_tuple(std::false_type{}), overloaded {
    [](std::true_type x) { return std::make_tuple(x); },
}) == std::make_tuple(std::false_type{}));




// apply change to first double int combi
static_assert(tuple_apply_first_pair(std::make_tuple(1, 2., 3, 4.), overloaded {
    [](auto x1, auto x2) {
        using T1 = decltype(x1);
        using T2 = decltype(x2);

        if constexpr(std::is_same_v<T1, double> and std::is_same_v<T2, int>) {
            return std::make_tuple(x1 + x2);
        }
    },
}) == std::make_tuple(3, 4., 3.));

static_assert(tuple_apply_first_pair(std::make_tuple(), overloaded {
    [](auto x1, auto x2) {
        using T1 = decltype(x1);
        using T2 = decltype(x2);

        if constexpr(std::is_same_v<T1, double> and std::is_same_v<T2, int>) {
            return std::make_tuple(x1 + x2);
        }
    },
}) == std::make_tuple());

static_assert(tuple_apply_first_pair(std::make_tuple(1), overloaded {
    [](auto x1, auto x2) {
        using T1 = decltype(x1);
        using T2 = decltype(x2);

        if constexpr(std::is_same_v<T1, double> and std::is_same_v<T2, int>) {
            return std::make_tuple(x1 + x2);
        }
    },
}) == std::make_tuple(1));

static_assert(tuple_apply_first_pair(std::make_tuple(std::false_type{}, std::false_type{}), overloaded {
    [](std::false_type x, std::true_type y) { return std::make_tuple(x, y); },
}) == std::make_tuple(std::false_type{}, std::false_type{}));
