#include <iostream>

#include "d_rive.h"

using namespace d_rive;

static_assert(solve(x<0>, x<0>) == 0_c);
static_assert(solve(2_c + x<0>, x<0>) == -2_c);
static_assert(solve(x<0> + x<1>, x<0>) == -x<1>);
static_assert(solve(x<0> + x<1>, x<1>) == -x<0>);
static_assert(solve(x<0> - x<1>, x<0>) == x<1>);
static_assert(solve(-x<0> + x<1>, x<1>) == x<0>);

static_assert(solve(3_c * x<0>, x<0>) == 0_c);
static_assert(solve(3_c * x<0> -x<1>, x<0>) == x<1>/3_c);
static_assert(solve(x<0>/x<1> -1_c, x<0>) == x<1>);
static_assert(solve(x<1>/x<0> -1_c, x<0>) == x<1>);

static_assert(solve(x<0>*x<0> -4_c, x<0>) == pow(4_c, 0.5_c));

static_assert(integrate(2_c) == 2_c * x<0>);
static_assert(integrate(2_c+2_c) == 4_c * x<0>);
static_assert(integrate(-3_c) == -3_c * x<0>);
static_assert(integrate(0_c) == 0_c);
static_assert(integrate(0.3_c) == 0.3_c * x<0>);

static_assert(integrate(x<0>) == 0.5_c * x<0>*x<0>);
static_assert(integrate(2_c*x<0>) == x<0>*x<0>);

static_assert(integrate(x<0>*x<0>) == 1_c/3_c * x<0>*x<0>*x<0>);

static_assert(integrate(2_c+x<0>) == 2_c * x<0> + 0.5_c * x<0>*x<0>);
static_assert(integrate(x<0>*x<0>+x<0>) == 0.5_c * x<0>*x<0> + 1_c/3_c * x<0>*x<0>*x<0>);

static_assert(integrate(ln(x<0>)) == x<0> * (ln(x<0>) -1_c));
static_assert(integrate(ln(x<0>*x<0>)) == x<0> * (ln(x<0>*x<0>) -2_c));
static_assert(integrate(ln(pow(x<0>,3_c*x<1>))) == x<0> * (ln(pow(x<0>, 3_c*x<1>)) - 3_c*x<1>));
static_assert(integrate(2_c*x<0>*x<1>) == x<0>*x<0>*x<1>);
static_assert(integrate(2_c*x<0>*x<1>, x<1>) == x<0>*x<1>*x<1>);

static_assert(integrate(pow(x<2>, x<1>)) == x<0> * pow(x<2>, x<1>));
static_assert(integrate(pow(x<0>, x<1>)) == pow(x<0>, x<1>+1_c) / (x<1> + 1_c));

static_assert(integrate(pow(x<1>, x<0>*x<2>)) == pow(x<1>, x<0>*x<2>) / (x<2>*ln(x<1>)));
static_assert(integrate(pow(x<1>, 0.5_c*x<0>-4_c)) == 2_c*pow(x<1>, 0.5_c*x<0>-4_c) / ln(x<1>));
