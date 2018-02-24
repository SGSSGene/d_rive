#include "d_rive.h"

using namespace d_rive;
using namespace d_rive::detail;

static_assert(size_v<Sum<>> == 0);
static_assert(size_v<Sum<double>> == 1);
static_assert(size_v<Sum<int, int>> == 2);

static_assert((12.4_c).value() == 12.4);
static_assert((.5_c).value() == .5);
static_assert((2._c).value() == 2.);

static_assert(c<3> == 3._c);
static_assert(c<3, 5> == 0.6_c);
static_assert(c<10, 5> == 2._c);

static_assert(is_same_tpl_v<Const<>, Const<>>);
static_assert(is_same_tpl_v<Const<>, Const<Number<integer<2>>>>);
static_assert(not is_same_tpl_v<Const<>, Var<>>);
static_assert(is_same_tpl_v<ConstOne, ConstOne>);
static_assert(std::is_same_v<ConstOne, ConstOne>);
static_assert(not is_same_tpl_v<ConstOne, Sum<>>);
static_assert(not is_same_tpl_v<ConstOne, Sum<ConstOne>>);
static_assert(not is_same_tpl_v<ConstZero, Sum<>>);
static_assert(not is_same_tpl_v<ConstZero, Sum<ConstZero>>);

static_assert(std::is_same_v<decltype(full_simplify(c<1>)),      C<1>>);
static_assert(std::is_same_v<decltype(full_simplify(c<0, 2>)),   C<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<0, -2>)),  C<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<4, 4>)),   C<1>>);
static_assert(std::is_same_v<decltype(full_simplify(c<-4, -4>)), C<1>>);
static_assert(std::is_same_v<decltype(full_simplify(c<0>+c<1>)), C<1>>);
static_assert(std::is_same_v<decltype(full_simplify(c<4>+c<6>)), C<10>>);
static_assert(std::is_same_v<decltype(full_simplify(c<0>*c<1>)), C<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<0>*c<0>)), C<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<3>*c<2>)), C<6>>);
static_assert(std::is_same_v<decltype(full_simplify(c<3>*c<2>+c<4>)),   C<10>>);
static_assert(std::is_same_v<decltype(full_simplify(c<3>*(c<2>+c<4>))), C<18>>);

static_assert(std::is_same_v<decltype(full_simplify(c<0>+c<0>)), C<0>>);

static_assert(std::is_same_v<decltype(full_simplify(x<0>)), X<0>>);
static_assert(std::is_same_v<decltype(full_simplify(x<1>)), X<1>>);
static_assert(std::is_same_v<decltype(full_simplify(x<0>+x<1>)), decltype(x<0>+x<1>)>);
static_assert(std::is_same_v<decltype(full_simplify(x<1>+x<0>)), decltype(x<0>+x<1>)>);
static_assert(std::is_same_v<decltype(full_simplify(x<0>+x<1>)), Sum<X<0>, X<1>>>);
static_assert(std::is_same_v<decltype(full_simplify(x<0>+x<1>+x<2>)), Sum<Var<integer<0>>, Var<integer<1>>, Var<integer<2>>>>);
static_assert(std::is_same_v<decltype(full_simplify(x<0>*x<1>)), decltype(x<0>*x<1>)>);
static_assert(std::is_same_v<decltype(full_simplify(x<0>*x<1>)), Mul<Var<integer<0>>, Var<integer<1>>>>);
static_assert(std::is_same_v<decltype(full_simplify(x<1>*x<0>)), Mul<Var<integer<0>>, Var<integer<1>>>>);
static_assert(std::is_same_v<decltype(full_simplify(x<0>*x<1>*x<2>)), Mul<Var<integer<0>>, Var<integer<1>>, Var<integer<2>>>>);

static_assert(std::is_same_v<decltype(full_simplify(x<0>+x<0>)), Mul<C<2>, Var<integer<0>>>>);
static_assert(std::is_same_v<decltype(full_simplify(x<1>+x<1>)), Mul<C<2>, Var<integer<1>>>>);
static_assert(std::is_same_v<decltype(full_simplify(x<1>+x<1>+x<1>)), Mul<C<3>, Var<integer<1>>>>);
static_assert(std::is_same_v<decltype(full_simplify(x<1>+x<1>+x<1>+x<1>)), Mul<C<4>, Var<integer<1>>>>);

static_assert(std::is_same_v<decltype(full_simplify(c<0>+x<0>)), X<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<0>+x<1>)), X<1>>);

static_assert(std::is_same_v<decltype(full_simplify(c<1>*x<0>)), X<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<1>*x<1>)), Var<integer<1>>>);

static_assert(std::is_same_v<decltype(full_simplify(c<0>*x<0>)), C<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<0>*x<1>)), C<0>>);

static_assert(std::is_same_v<decltype(full_simplify(c<0>*(x<0> + x<1>))), C<0>>);
static_assert(std::is_same_v<decltype(full_simplify(c<2>*(x<1> + x<1>))), Mul<C<4>, Var<integer<1>>>>);
static_assert(std::is_same_v<decltype(full_simplify(c<2>*(x<1> + x<2>))), Sum<Mul<C<2>, Var<integer<1>>>, Mul<C<2>, Var<integer<2>>>>>);

static_assert(std::is_same_v<decltype(full_simplify(-x<0>)), Mul<C<-1>, Var<integer<0>>>>);
static_assert(std::is_same_v<decltype(full_simplify(-x<1>)), Mul<C<-1>, Var<integer<1>>>>);
static_assert(std::is_same_v<decltype(full_simplify(-c<1>)), C<-1>>);
static_assert(std::is_same_v<decltype(full_simplify(-c<0>)), C<0>>);

static_assert(std::is_same_v<decltype(full_simplify(c<1>-c<2>)), C<-1>>);
static_assert(std::is_same_v<decltype(full_simplify(c<1>-x<2>)), Sum<C<1>, Mul<C<-1>, Var<integer<2>>>>>);

static_assert(std::is_same_v<decltype(full_simplify(pow(c<0>, x<2>))), C<0>>);
static_assert(std::is_same_v<decltype(full_simplify(pow(c<1>, x<2>))), C<1>>);
static_assert(std::is_same_v<decltype(full_simplify(pow(x<2>, c<0>))), C<1>>);
static_assert(std::is_same_v<decltype(full_simplify(pow(x<2>, c<1>))), Var<integer<2>>>);
static_assert(std::is_same_v<decltype(full_simplify(pow(x<2>, c<2>))), Exp<Var<integer<2>>, C<2>>>);
static_assert(std::is_same_v<decltype(full_simplify(pow(c<2>, x<2>))), Exp<C<2>, Var<integer<2>>>>);

static_assert(std::is_same_v<decltype(full_simplify(pow(c<2>, c<3>))), C<8>>);
static_assert(std::is_same_v<decltype(full_simplify(pow(c<3>, c<2>))), C<9>>);

static_assert(3_c + 1_c == 4_c);
static_assert(-3_c + -1_c == -4_c);
static_assert(3_c + -1_c == 2_c);
static_assert(3_c  - 1_c == 2_c);
static_assert(-3_c  - 1_c == -4_c);


static_assert(4_c + 2_c == 6_c);
static_assert(pow(3_c, 2_c) == 9_c);

static_assert(1_c / -1_c == -1_c);

static_assert(x<0> * x<0> == pow(x<0>, 2_c));
static_assert(x<0> * x<0> * x<0> == pow(x<0>, 3_c));
static_assert(x<0> * x<0> * x<0> * x<0> == pow(x<0>, 4_c));
static_assert(x<0> * pow(x<0>, 2_c) * x<0> == pow(x<0>, 4_c));
static_assert(pow(x<0>, 2_c) * x<0> * x<0> == pow(x<0>, 4_c));
static_assert(x<0> * x<0> * pow(x<0>, 2_c) == pow(x<0>, 4_c));
static_assert(pow(x<0>, 2_c) * pow(x<0>, 2_c) == pow(x<0>, 4_c));
static_assert(pow(x<0>*x<1>, 2_c) == pow(x<0>, 2_c)*pow(x<1>, 2_c));
static_assert(pow(2_c, x<0>) * pow(2_c, x<0>) == pow(2_c, 2_c*x<0>));
static_assert(pow(2_c, x<0>) * pow(2_c, x<1>) == pow(2_c, x<0> + x<1>));
static_assert(pow(pow(x<0>, 2_c), 3_c) == pow(x<0>,  6_c));
static_assert(pow(pow(x<0>, 2_c),-1_c) == pow(x<0>, -2_c));


static_assert(derive(2_c) == 0_c);
static_assert(derive(x<0>) == 1_c);
static_assert(derive(x<0>*x<0>) == 2_c*x<0>);
static_assert(derive(x<0> + x<0>) == 2_c);
static_assert(derive(3_c * x<0> * x<0>) == 6_c * x<0>);
static_assert(derive(3_c * pow(x<0>, 4_c)) == 12_c * pow(x<0>, 3_c));

static_assert(ln(1_c) == 0_c);
static_assert(derive(ln(2_c)) == 0_c);
static_assert(derive(ln(x<0>)) == 1_c / x<0>);
static_assert(derive(ln(x<0>*x<0>)) == 2_c / x<0>);
static_assert(pow(3_c, 2_c * x<0>) == pow(9_c, x<0>));
static_assert(pow(3_c, 2_c * x<0>) == pow(9_c, x<0>));
static_assert(pow(3_c, -1_c) == 1_c/3_c);
static_assert(pow(9_c, -1_c/2_c) == 1_c/3_c);
static_assert(1_c / pow(x<0>, -1_c) == x<0>);

static_assert(derive(sin(x<0>)) == cos(x<0>));
static_assert(derive(sin(x<0>*x<0>)) == cos(x<0>*x<0>) * 2_c*x<0>);
static_assert(derive(cos(x<0>)) == -sin(x<0>));
static_assert(derive(-sin(x<0>)) == -cos(x<0>));
static_assert(derive(-cos(x<0>)) == sin(x<0>));

static_assert( sin(x<0>) == integrate( cos(x<0>)));
static_assert( cos(x<0>) == integrate(-sin(x<0>)));
static_assert(-sin(x<0>) == integrate(-cos(x<0>)));
static_assert(-cos(x<0>) == integrate( sin(x<0>)));
