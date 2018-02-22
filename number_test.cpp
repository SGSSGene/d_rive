#include "Number.h"

using namespace d_rive::detail;

static_assert(std::is_integral<integer<>::type>::value);
static_assert(std::is_integral<integer<>::lower_type>::value);

static_assert(integer<0>{} == 0);
static_assert(integer<1>{} == 1);
static_assert(integer<2>{} == 2);
static_assert(integer<-1>{} == -1);
static_assert(integer<0>::value == 0);
static_assert(integer<1>::value == 1);
static_assert(integer<2>::value == 2);
static_assert(integer<-1>::value == -1);

static_assert(integer<0>{} != 1);
static_assert(integer<1>{} != 2);
static_assert(integer<2>{} != 3);

static_assert(integer< 0>{} + integer< 0>{} ==  0);
static_assert(integer< 1>{} + integer< 0>{} ==  1);
static_assert(integer<-1>{} + integer< 0>{} == -1);
static_assert(integer< 0>{} + integer< 1>{} ==  1);
static_assert(integer< 1>{} + integer< 1>{} ==  2);
static_assert(integer<-1>{} + integer< 1>{} ==  0);
static_assert(integer< 0>{} + integer<-1>{} == -1);
static_assert(integer< 1>{} + integer<-1>{} ==  0);
static_assert(integer<-1>{} + integer<-1>{} == -2);

static_assert(integer< 0>{} - integer< 0>{} ==  0);
static_assert(integer< 1>{} - integer< 0>{} ==  1);
static_assert(integer<-1>{} - integer< 0>{} == -1);
static_assert(integer< 0>{} - integer< 1>{} == -1);
static_assert(integer< 1>{} - integer< 1>{} ==  0);
static_assert(integer<-1>{} - integer< 1>{} == -2);
static_assert(integer< 0>{} - integer<-1>{} ==  1);
static_assert(integer< 1>{} - integer<-1>{} ==  2);
static_assert(integer<-1>{} - integer<-1>{} ==  0);

static_assert(integer< 0>{} * integer< 0>{} ==  0);
static_assert(integer< 1>{} * integer< 0>{} ==  0);
static_assert(integer<-1>{} * integer< 0>{} ==  0);
static_assert(integer< 0>{} * integer< 1>{} ==  0);
static_assert(integer< 1>{} * integer< 1>{} ==  1);
static_assert(integer<-1>{} * integer< 1>{} == -1);
static_assert(integer< 0>{} * integer<-1>{} ==  0);
static_assert(integer< 1>{} * integer<-1>{} == -1);
static_assert(integer<-1>{} * integer<-1>{} ==  1);

static_assert(integer< 0>{} / integer< 1>{} ==  0);
static_assert(integer< 1>{} / integer< 1>{} ==  1);
static_assert(integer<-1>{} / integer< 1>{} == -1);
static_assert(integer< 0>{} / integer<-1>{} ==  0);
static_assert(integer< 1>{} / integer<-1>{} == -1);
static_assert(integer<-1>{} / integer<-1>{} ==  1);

static_assert(integer< 4>{} / integer< 2>{} ==  2);
static_assert(integer< 8>{} / integer< 2>{} ==  4);
static_assert(integer<-6>{} / integer< 2>{} == -3);

static_assert(sign(integer<0>{}) == 0);
static_assert(sign(integer<1>{}) == 1);
static_assert(sign(integer<-1>{}) == -1);
static_assert(sign(integer<4>{}) == 1);
static_assert(sign(integer<-5>{}) == -1);

static_assert(abs(integer<0>{}) == 0);
static_assert(abs(integer<1>{}) == 1);
static_assert(abs(integer<-1>{}) == 1);
static_assert(abs(integer<5>{}) == 5);
static_assert(abs(integer<-6>{}) == 6);

static_assert(-integer<0>{} == 0);
static_assert(-integer<1>{} == -1);
static_assert(-integer<-1>{} == 1);
static_assert(-integer<5>{} == -5);
static_assert(-integer<-6>{} == 6);

static_assert(max(integer< 0>{}, integer<2>{}) == 2);
static_assert(max(integer< 1>{}, integer<2>{}) == 2);
static_assert(max(integer<-1>{}, integer<2>{}) == 2);
static_assert(max(integer< 5>{}, integer<2>{}) == 5);
static_assert(max(integer<-6>{}, integer<2>{}) == 2);

static_assert(lcm(integer<1>{}, integer<0>{}) == 0);
static_assert(lcm(integer<1>{}, integer<1>{}) == 1);
static_assert(lcm(integer<10>{}, integer<20>{}) == 20);
static_assert(lcm(integer<-10>{}, integer<20>{}) == 20);
static_assert(lcm(integer<10>{}, integer<-20>{}) == 20);
static_assert(lcm(integer<-10>{}, integer<-20>{}) == 20);

static_assert(gcd(integer<1>{}, integer<0>{}) == 1);
static_assert(gcd(integer<1>{}, integer<1>{}) == 1);
static_assert(gcd(integer<10>{}, integer<20>{}) == 10);
static_assert(gcd(integer<-10>{}, integer<20>{}) == 10);
static_assert(gcd(integer<10>{}, integer<-20>{}) == 10);
static_assert(gcd(integer<-10>{}, integer<-20>{}) == 10);

static_assert(Number<>::value() == 0);
static_assert(Number<integer<0>>::value() == 0);
static_assert(Number<integer<1>>::value() == 1);
static_assert(Number<integer<2>>::value() == 2);
static_assert(Number<integer<3>>::value() == 3);
static_assert(Number<integer<3>, integer<2>>::value() == 3./2.);

static_assert(std::is_same_v<decltype(normalize(Number<integer<2>, integer<2>>{})), Number<integer<1>>>);
static_assert(std::is_same_v<decltype(normalize(Number<integer<6>, integer<3>>{})), Number<integer<2>>>);
static_assert(std::is_same_v<decltype(normalize(Number<integer<4>, integer<8>>{})), Number<integer<1>, integer<2>>>);
static_assert(std::is_same_v<decltype(normalize(Number<integer<2>, integer<-2>>{})), Number<integer<-1>>>);
static_assert(std::is_same_v<decltype(normalize(Number<integer<-6>, integer<3>>{})), Number<integer<-2>>>);
static_assert(std::is_same_v<decltype(normalize(Number<integer<-4>, integer<-8>>{})), Number<integer<1>, integer<2>>>);

static_assert((Number<>{} + Number<>{}).value() == 0);
static_assert((Number<integer<1>>{} + Number<>{}).value() == 1);
static_assert((Number<integer<1>>{} + Number<integer<1>>{}).value() == 2);
static_assert((Number<integer<1>>{} + Number<integer<3>>{}).value() == 4);
static_assert((Number<integer<1>>{} + Number<integer<-3>>{}).value() == -2);

static_assert((Number<>{} - Number<>{}).value() == 0);
static_assert((Number<integer<1>>{} - Number<>{}).value() == 1);
static_assert((Number<integer<1>>{} - Number<integer<1>>{}).value() == 0);
static_assert((Number<integer<1>>{} - Number<integer<3>>{}).value() == -2);
static_assert((Number<integer<1>>{} - Number<integer<-3>>{}).value() == 4);

static_assert((Number<>{} * Number<>{}).value() == 0);
static_assert((Number<integer<1>>{} * Number<>{}).value() == 0);
static_assert((Number<integer<1>>{} * Number<integer<1>>{}).value() == 1);
static_assert((Number<integer<1>>{} * Number<integer<3>>{}).value() == 3);
static_assert((Number<integer<1>>{} * Number<integer<-3>>{}).value() == -3);

static_assert((Number<integer<1>>{} / Number<integer<1>>{}).value() == 1);
static_assert((Number<integer<1>>{} / Number<integer<3>>{}).value() == 1./3.);
static_assert((Number<integer<1>>{} / Number<integer<-3>>{}).value() == -1./3.);

static_assert(std::is_same_v<decltype((Number<integer<1>>{} / Number<integer<-1>>{})), Number<integer<-1>>>);
static_assert(std::is_same_v<decltype(inv(Number<integer<-1>>{})), Number<integer<-1>>>);

static_assert((-Number<integer< 0>>{}).value() == 0);
static_assert((-Number<integer<-0>>{}).value() == 0);
static_assert((-Number<integer< 1>>{}).value() ==-1);
static_assert((-Number<integer<-1>>{}).value() == 1);
static_assert((-Number<integer< 3>>{}).value() ==-3);
static_assert((-Number<integer<-5>>{}).value() == 5);

static_assert(pow(Number<integer< 4>>{}, integer<0>{}).value() == 1);
static_assert(pow(Number<integer< 2>>{}, integer<1>{}).value() == 2);
static_assert(pow(Number<integer< 2>>{}, integer<2>{}).value() == 4);
static_assert(pow(Number<integer<-2>>{}, integer<2>{}).value() == 4);
static_assert(pow(Number<integer< 3>>{}, integer<2>{}).value() == 9);
static_assert(pow(Number<integer< 2>>{}, integer<3>{}).value() == 8);
static_assert(pow(Number<integer<-2>>{}, integer<3>{}).value() == -8);

static_assert(n_root(Number<integer< 4>>{}, integer<2>{}).value() == 2);
static_assert(n_root(Number<integer< 9>>{}, integer<2>{}).value() == 3);
static_assert(n_root(Number<integer< 8>>{}, integer<3>{}).value() == 2);
static_assert(n_root(Number<integer<27>>{}, integer<3>{}).value() == 3);

static_assert(pow(Number<integer<27>>{}, Number<integer<2>, integer<3>>{}).value() == 9);
static_assert(pow(Number<integer<8>>{}, Number<integer<2>, integer<3>>{}).value() == 4);
static_assert(pow(Number<integer<9>>{}, Number<integer<3>, integer<2>>{}).value() == 27);
static_assert(pow(Number<integer<4>>{}, Number<integer<3>, integer<2>>{}).value() == 8);

static_assert(ln(Number<integer<1>>{}).value() == 0);
static_assert(std::abs(ln(Number<integer<2>>{}).value() - 0.693147) < 1e-3);
static_assert(std::abs(ln(Number<integer<1>, integer<2>>{}).value() + 0.693147) < 1e-3);

static_assert(log(Number<integer<2>>{}, Number<integer<1>>{}).value() == 0);
static_assert(log(Number<integer<8>>{}, Number<integer<1>>{}).value() == 0);
static_assert(log(Number<integer<10>>{}, Number<integer<1>>{}).value() == 0);
