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

static_assert(sin(Number<integer<0>>{}).value() == 0);
static_assert(std::abs(sin(Number<integer< 0>, integer<10>>{}).value() - 0.000000) < 1e-3);
static_assert(std::abs(sin(Number<integer< 1>, integer<10>>{}).value() - 0.099833) < 1e-3);
static_assert(std::abs(sin(Number<integer< 2>, integer<10>>{}).value() - 0.198670) < 1e-3);
static_assert(std::abs(sin(Number<integer< 3>, integer<10>>{}).value() - 0.295520) < 1e-3);
static_assert(std::abs(sin(Number<integer< 4>, integer<10>>{}).value() - 0.389420) < 1e-3);
static_assert(std::abs(sin(Number<integer< 5>, integer<10>>{}).value() - 0.479430) < 1e-3);
static_assert(std::abs(sin(Number<integer< 6>, integer<10>>{}).value() - 0.564640) < 1e-3);
static_assert(std::abs(sin(Number<integer< 7>, integer<10>>{}).value() - 0.644220) < 1e-3);
static_assert(std::abs(sin(Number<integer< 8>, integer<10>>{}).value() - 0.717360) < 1e-3);
static_assert(std::abs(sin(Number<integer< 9>, integer<10>>{}).value() - 0.783330) < 1e-3);
static_assert(std::abs(sin(Number<integer<10>, integer<10>>{}).value() -  0.841471) < 1e-3);
static_assert(std::abs(sin(Number<integer<11>, integer<10>>{}).value() -  0.891207) < 1e-3);
static_assert(std::abs(sin(Number<integer<12>, integer<10>>{}).value() -  0.932039) < 1e-3);
static_assert(std::abs(sin(Number<integer<13>, integer<10>>{}).value() -  0.963558) < 1e-3);
static_assert(std::abs(sin(Number<integer<14>, integer<10>>{}).value() -  0.985450) < 1e-3);
static_assert(std::abs(sin(Number<integer<15>, integer<10>>{}).value() -  0.997495) < 1e-3);
static_assert(std::abs(sin(Number<integer<16>, integer<10>>{}).value() -  0.999574) < 1e-3);
static_assert(std::abs(sin(Number<integer<17>, integer<10>>{}).value() -  0.991665) < 1e-3);
static_assert(std::abs(sin(Number<integer<18>, integer<10>>{}).value() -  0.973848) < 1e-3);
static_assert(std::abs(sin(Number<integer<19>, integer<10>>{}).value() -  0.946300) < 1e-3);
static_assert(std::abs(sin(Number<integer<20>, integer<10>>{}).value() -  0.909297) < 1e-3);
static_assert(std::abs(sin(Number<integer<21>, integer<10>>{}).value() -  0.863209) < 1e-3);
static_assert(std::abs(sin(Number<integer<22>, integer<10>>{}).value() -  0.808496) < 1e-3);
static_assert(std::abs(sin(Number<integer<23>, integer<10>>{}).value() -  0.745705) < 1e-3);
static_assert(std::abs(sin(Number<integer<24>, integer<10>>{}).value() -  0.675463) < 1e-3);
static_assert(std::abs(sin(Number<integer<25>, integer<10>>{}).value() -  0.598472) < 1e-3);
static_assert(std::abs(sin(Number<integer<26>, integer<10>>{}).value() -  0.515501) < 1e-3);
static_assert(std::abs(sin(Number<integer<27>, integer<10>>{}).value() -  0.427380) < 1e-3);
static_assert(std::abs(sin(Number<integer<28>, integer<10>>{}).value() -  0.334988) < 1e-3);
static_assert(std::abs(sin(Number<integer<29>, integer<10>>{}).value() -  0.239249) < 1e-3);
static_assert(std::abs(sin(Number<integer<30>, integer<10>>{}).value() -  0.141120) < 1e-3);
static_assert(std::abs(sin(Number<integer<31>, integer<10>>{}).value() -  0.041581) < 1e-3);
static_assert(std::abs(sin(Number<integer<32>, integer<10>>{}).value() - -0.058374) < 1e-3);
static_assert(std::abs(sin(Number<integer<33>, integer<10>>{}).value() - -0.157746) < 1e-3);
static_assert(std::abs(sin(Number<integer<34>, integer<10>>{}).value() - -0.255541) < 1e-3);
static_assert(std::abs(sin(Number<integer<35>, integer<10>>{}).value() - -0.350783) < 1e-3);
static_assert(std::abs(sin(Number<integer<36>, integer<10>>{}).value() - -0.442520) < 1e-3);
static_assert(std::abs(sin(Number<integer<37>, integer<10>>{}).value() - -0.529836) < 1e-3);
static_assert(std::abs(sin(Number<integer<38>, integer<10>>{}).value() - -0.611858) < 1e-3);
static_assert(std::abs(sin(Number<integer<39>, integer<10>>{}).value() - -0.687766) < 1e-3);
static_assert(std::abs(sin(Number<integer<40>, integer<10>>{}).value() - -0.756802) < 1e-3);
static_assert(std::abs(sin(Number<integer<41>, integer<10>>{}).value() - -0.818277) < 1e-3);
static_assert(std::abs(sin(Number<integer<42>, integer<10>>{}).value() - -0.871576) < 1e-3);
static_assert(std::abs(sin(Number<integer<43>, integer<10>>{}).value() - -0.916166) < 1e-3);
static_assert(std::abs(sin(Number<integer<44>, integer<10>>{}).value() - -0.951602) < 1e-3);
static_assert(std::abs(sin(Number<integer<45>, integer<10>>{}).value() - -0.977530) < 1e-3);
static_assert(std::abs(sin(Number<integer<46>, integer<10>>{}).value() - -0.993691) < 1e-3);
static_assert(std::abs(sin(Number<integer<47>, integer<10>>{}).value() - -0.999923) < 1e-3);
static_assert(std::abs(sin(Number<integer<48>, integer<10>>{}).value() - -0.996165) < 1e-3);
static_assert(std::abs(sin(Number<integer<49>, integer<10>>{}).value() - -0.982453) < 1e-3);
static_assert(std::abs(sin(Number<integer<50>, integer<10>>{}).value() - -0.958924) < 1e-3);
static_assert(std::abs(sin(Number<integer<51>, integer<10>>{}).value() - -0.925815) < 1e-3);
static_assert(std::abs(sin(Number<integer<52>, integer<10>>{}).value() - -0.883455) < 1e-3);
static_assert(std::abs(sin(Number<integer<53>, integer<10>>{}).value() - -0.832267) < 1e-3);
static_assert(std::abs(sin(Number<integer<54>, integer<10>>{}).value() - -0.772764) < 1e-3);
static_assert(std::abs(sin(Number<integer<55>, integer<10>>{}).value() - -0.705540) < 1e-3);
static_assert(std::abs(sin(Number<integer<56>, integer<10>>{}).value() - -0.631267) < 1e-3);
static_assert(std::abs(sin(Number<integer<57>, integer<10>>{}).value() - -0.550686) < 1e-3);
static_assert(std::abs(sin(Number<integer<58>, integer<10>>{}).value() - -0.464602) < 1e-3);
static_assert(std::abs(sin(Number<integer<59>, integer<10>>{}).value() - -0.373877) < 1e-3);
static_assert(std::abs(sin(Number<integer<60>, integer<10>>{}).value() - -0.279415) < 1e-3);
static_assert(std::abs(sin(Number<integer<61>, integer<10>>{}).value() - -0.182163) < 1e-3);
static_assert(std::abs(sin(Number<integer<62>, integer<10>>{}).value() - -0.083089) < 1e-3);
static_assert(std::abs(sin(Number<integer<63>, integer<10>>{}).value() -  0.016814) < 1e-3);
static_assert(std::abs(sin(Number<integer<64>, integer<10>>{}).value() -  0.116549) < 1e-3);
static_assert(std::abs(sin(Number<integer<65>, integer<10>>{}).value() -  0.215120) < 1e-3);
static_assert(std::abs(sin(Number<integer<66>, integer<10>>{}).value() -  0.311541) < 1e-3);
static_assert(std::abs(sin(Number<integer<67>, integer<10>>{}).value() -  0.404850) < 1e-3);
static_assert(std::abs(sin(Number<integer<68>, integer<10>>{}).value() -  0.494113) < 1e-3);
static_assert(std::abs(sin(Number<integer<69>, integer<10>>{}).value() -  0.578440) < 1e-3);

static_assert(cos(Number<integer<0>>{}).value() == 1.);
static_assert(std::abs(cos(Number<integer< 0>, integer<10>>{}).value() -  1.000000) < 1e-3);
static_assert(std::abs(cos(Number<integer< 1>, integer<10>>{}).value() -  0.995004) < 1e-3);
static_assert(std::abs(cos(Number<integer< 2>, integer<10>>{}).value() -  0.980067) < 1e-3);
static_assert(std::abs(cos(Number<integer< 3>, integer<10>>{}).value() -  0.955336) < 1e-3);
static_assert(std::abs(cos(Number<integer< 4>, integer<10>>{}).value() -  0.921061) < 1e-3);
static_assert(std::abs(cos(Number<integer< 5>, integer<10>>{}).value() -  0.877583) < 1e-3);
static_assert(std::abs(cos(Number<integer< 6>, integer<10>>{}).value() -  0.825336) < 1e-3);
static_assert(std::abs(cos(Number<integer< 7>, integer<10>>{}).value() -  0.764842) < 1e-3);
static_assert(std::abs(cos(Number<integer< 8>, integer<10>>{}).value() -  0.696707) < 1e-3);
static_assert(std::abs(cos(Number<integer< 9>, integer<10>>{}).value() -  0.621610) < 1e-3);
static_assert(std::abs(cos(Number<integer<10>, integer<10>>{}).value() -  0.540302) < 1e-3);
static_assert(std::abs(cos(Number<integer<11>, integer<10>>{}).value() -  0.453596) < 1e-3);
static_assert(std::abs(cos(Number<integer<12>, integer<10>>{}).value() -  0.362358) < 1e-3);
static_assert(std::abs(cos(Number<integer<13>, integer<10>>{}).value() -  0.267499) < 1e-3);
static_assert(std::abs(cos(Number<integer<14>, integer<10>>{}).value() -  0.169967) < 1e-3);
static_assert(std::abs(cos(Number<integer<15>, integer<10>>{}).value() -  0.070737) < 1e-3);
static_assert(std::abs(cos(Number<integer<16>, integer<10>>{}).value() - -0.029200) < 1e-3);
static_assert(std::abs(cos(Number<integer<17>, integer<10>>{}).value() - -0.128844) < 1e-3);
static_assert(std::abs(cos(Number<integer<18>, integer<10>>{}).value() - -0.227202) < 1e-3);
static_assert(std::abs(cos(Number<integer<19>, integer<10>>{}).value() - -0.323290) < 1e-3);
static_assert(std::abs(cos(Number<integer<20>, integer<10>>{}).value() - -0.416147) < 1e-3);
static_assert(std::abs(cos(Number<integer<21>, integer<10>>{}).value() - -0.504846) < 1e-3);
static_assert(std::abs(cos(Number<integer<22>, integer<10>>{}).value() - -0.588501) < 1e-3);
static_assert(std::abs(cos(Number<integer<23>, integer<10>>{}).value() - -0.666276) < 1e-3);
static_assert(std::abs(cos(Number<integer<24>, integer<10>>{}).value() - -0.737394) < 1e-3);
static_assert(std::abs(cos(Number<integer<25>, integer<10>>{}).value() - -0.801144) < 1e-3);
static_assert(std::abs(cos(Number<integer<26>, integer<10>>{}).value() - -0.856889) < 1e-3);
static_assert(std::abs(cos(Number<integer<27>, integer<10>>{}).value() - -0.904072) < 1e-3);
static_assert(std::abs(cos(Number<integer<28>, integer<10>>{}).value() - -0.942222) < 1e-3);
static_assert(std::abs(cos(Number<integer<29>, integer<10>>{}).value() - -0.970958) < 1e-3);
static_assert(std::abs(cos(Number<integer<30>, integer<10>>{}).value() - -0.989992) < 1e-3);
static_assert(std::abs(cos(Number<integer<31>, integer<10>>{}).value() - -0.999135) < 1e-3);
static_assert(std::abs(cos(Number<integer<32>, integer<10>>{}).value() - -0.998295) < 1e-3);
static_assert(std::abs(cos(Number<integer<33>, integer<10>>{}).value() - -0.987480) < 1e-3);
static_assert(std::abs(cos(Number<integer<34>, integer<10>>{}).value() - -0.966798) < 1e-3);
static_assert(std::abs(cos(Number<integer<35>, integer<10>>{}).value() - -0.936457) < 1e-3);
static_assert(std::abs(cos(Number<integer<36>, integer<10>>{}).value() - -0.896758) < 1e-3);
static_assert(std::abs(cos(Number<integer<37>, integer<10>>{}).value() - -0.848100) < 1e-3);
static_assert(std::abs(cos(Number<integer<38>, integer<10>>{}).value() - -0.790968) < 1e-3);
static_assert(std::abs(cos(Number<integer<39>, integer<10>>{}).value() - -0.725932) < 1e-3);
static_assert(std::abs(cos(Number<integer<40>, integer<10>>{}).value() - -0.653644) < 1e-3);
static_assert(std::abs(cos(Number<integer<41>, integer<10>>{}).value() - -0.574824) < 1e-3);
static_assert(std::abs(cos(Number<integer<42>, integer<10>>{}).value() - -0.490261) < 1e-3);
static_assert(std::abs(cos(Number<integer<43>, integer<10>>{}).value() - -0.400799) < 1e-3);
static_assert(std::abs(cos(Number<integer<44>, integer<10>>{}).value() - -0.307333) < 1e-3);
static_assert(std::abs(cos(Number<integer<45>, integer<10>>{}).value() - -0.210796) < 1e-3);
static_assert(std::abs(cos(Number<integer<46>, integer<10>>{}).value() - -0.112153) < 1e-3);
static_assert(std::abs(cos(Number<integer<47>, integer<10>>{}).value() - -0.012389) < 1e-3);
static_assert(std::abs(cos(Number<integer<48>, integer<10>>{}).value() -  0.087499) < 1e-3);
static_assert(std::abs(cos(Number<integer<49>, integer<10>>{}).value() -  0.186512) < 1e-3);
static_assert(std::abs(cos(Number<integer<50>, integer<10>>{}).value() -  0.283662) < 1e-3);
static_assert(std::abs(cos(Number<integer<51>, integer<10>>{}).value() -  0.377978) < 1e-3);
static_assert(std::abs(cos(Number<integer<52>, integer<10>>{}).value() -  0.468517) < 1e-3);
static_assert(std::abs(cos(Number<integer<53>, integer<10>>{}).value() -  0.554374) < 1e-3);
static_assert(std::abs(cos(Number<integer<54>, integer<10>>{}).value() -  0.634693) < 1e-3);
static_assert(std::abs(cos(Number<integer<55>, integer<10>>{}).value() -  0.708670) < 1e-3);
static_assert(std::abs(cos(Number<integer<56>, integer<10>>{}).value() -  0.775566) < 1e-3);
static_assert(std::abs(cos(Number<integer<57>, integer<10>>{}).value() -  0.834713) < 1e-3);
static_assert(std::abs(cos(Number<integer<58>, integer<10>>{}).value() -  0.885520) < 1e-3);
static_assert(std::abs(cos(Number<integer<59>, integer<10>>{}).value() -  0.927478) < 1e-3);
static_assert(std::abs(cos(Number<integer<60>, integer<10>>{}).value() -  0.960170) < 1e-3);
static_assert(std::abs(cos(Number<integer<61>, integer<10>>{}).value() -  0.983268) < 1e-3);
static_assert(std::abs(cos(Number<integer<62>, integer<10>>{}).value() -  0.996542) < 1e-3);
static_assert(std::abs(cos(Number<integer<63>, integer<10>>{}).value() -  0.999859) < 1e-3);
static_assert(std::abs(cos(Number<integer<64>, integer<10>>{}).value() -  0.993185) < 1e-3);
static_assert(std::abs(cos(Number<integer<65>, integer<10>>{}).value() -  0.976588) < 1e-3);
static_assert(std::abs(cos(Number<integer<66>, integer<10>>{}).value() -  0.950233) < 1e-3);
static_assert(std::abs(cos(Number<integer<67>, integer<10>>{}).value() -  0.914383) < 1e-3);
static_assert(std::abs(cos(Number<integer<68>, integer<10>>{}).value() -  0.869397) < 1e-3);
static_assert(std::abs(cos(Number<integer<69>, integer<10>>{}).value() -  0.815725) < 1e-3);

static_assert(number::abs(Number<integer<10>>{}).value() == 10);
static_assert(number::abs(Number<integer<-5>>{}).value() == 5);
static_assert(number::abs(Number<integer<0>>{}).value() == 0);

static_assert(number::sign(Number<integer<10>>{}).value() == 1);
static_assert(number::sign(Number<integer<-5>>{}).value() == -1);
static_assert(number::sign(Number<integer<0>>{}).value() == 0);

static_assert(number::min(Number<integer<10>>{}).value() == 10);
static_assert(number::min(Number<integer<-5>>{}).value() == -5);
static_assert(number::min(Number<integer<0>>{}).value() == 0);

static_assert(number::min(Number<integer<10>>{}, Number<integer<11>>{}).value() == 10);
static_assert(number::min(Number<integer<-5>>{}, Number<integer<11>>{}).value() == -5);
static_assert(number::min(Number<integer< 0>>{}, Number<integer<11>>{}).value() == 0);

static_assert(number::min(Number<integer<-3>>{}, Number<integer<10>>{}, Number<integer<11>>{}).value() == -3);
static_assert(number::min(Number<integer<-3>>{}, Number<integer<-5>>{}, Number<integer<11>>{}).value() == -5);
static_assert(number::min(Number<integer<-3>>{}, Number<integer< 0>>{}, Number<integer<11>>{}).value() == -3);

static_assert(number::max(Number<integer<10>>{}).value() == 10);
static_assert(number::max(Number<integer<-5>>{}).value() == -5);
static_assert(number::max(Number<integer< 0>>{}).value() == 0);

static_assert(number::max(Number<integer<10>>{}, Number<integer<11>>{}).value() == 11);
static_assert(number::max(Number<integer<-5>>{}, Number<integer<11>>{}).value() == 11);
static_assert(number::max(Number<integer< 0>>{}, Number<integer<11>>{}).value() == 11);

static_assert(number::max(Number<integer<-3>>{}, Number<integer<10>>{}, Number<integer<4>>{}).value() == 10);
static_assert(number::max(Number<integer<-3>>{}, Number<integer<-5>>{}, Number<integer<4>>{}).value() == 4);
static_assert(number::max(Number<integer<-3>>{}, Number<integer< 0>>{}, Number<integer<4>>{}).value() == 4);




