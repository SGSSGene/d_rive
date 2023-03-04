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
static_assert(Number<0.l>::value() == 0);
static_assert(Number<1.l>::value() == 1);
static_assert(Number<2.l>::value() == 2);
static_assert(Number<3.l>::value() == 3);
static_assert(Number<3.l/2>::value() == 3./2.);

static_assert((Number<>{} + Number<>{}).value() == 0);
static_assert((Number<1.l>{} + Number<>{}).value() == 1);
static_assert((Number<1.l>{} + Number<1.l>{}).value() == 2);
static_assert((Number<1.l>{} + Number<3.l>{}).value() == 4);
static_assert((Number<1.l>{} + Number<-3.l>{}).value() == -2);

static_assert((Number<>{} - Number<>{}).value() == 0);
static_assert((Number<1.l>{} - Number<>{}).value() == 1);
static_assert((Number<1.l>{} - Number<1.l>{}).value() == 0);
static_assert((Number<1.l>{} - Number<3.l>{}).value() == -2);
static_assert((Number<1.l>{} - Number<-3.l>{}).value() == 4);

static_assert((Number<>{} * Number<>{}).value() == 0);
static_assert((Number<1.l>{} * Number<>{}).value() == 0);
static_assert((Number<1.l>{} * Number<1.l>{}).value() == 1);
static_assert((Number<1.l>{} * Number<3.l>{}).value() == 3);
static_assert((Number<1.l>{} * Number<-3.l>{}).value() == -3);

static_assert((Number<1.l>{} / Number<1.l>{}).value() == 1);
static_assert((Number<1.l>{} / Number<3.l>{}).value() == 1.l/3.);
static_assert((Number<1.l>{} / Number<-3.l>{}).value() == -1.l/3.);

static_assert((-Number< 0.l>{}).value() == 0);
static_assert((-Number<-0.l>{}).value() == 0);
static_assert((-Number< 1.l>{}).value() ==-1);
static_assert((-Number<-1.l>{}).value() == 1);
static_assert((-Number< 3.l>{}).value() ==-3);
static_assert((-Number<-5.l>{}).value() == 5);

static_assert(pow(Number< 4.l>{}, integer<0>{}).value() == 1);
static_assert(pow(Number< 2.l>{}, integer<1>{}).value() == 2);
static_assert(pow(Number< 2.l>{}, integer<2>{}).value() == 4);
static_assert(pow(Number<-2.l>{}, integer<2>{}).value() == 4);
static_assert(pow(Number< 3.l>{}, integer<2>{}).value() == 9);
static_assert(pow(Number< 2.l>{}, integer<3>{}).value() == 8);
static_assert(pow(Number<-2.l>{}, integer<3>{}).value() == -8);

static_assert(n_root(Number< 4.l>{}, integer<2>{}).value() == 2);
static_assert(n_root(Number< 9.l>{}, integer<2>{}).value() == 3);
static_assert(n_root(Number< 8.l>{}, integer<3>{}).value() == 2);
static_assert(n_root(Number<27.l>{}, integer<3>{}).value() == 3);

static_assert(std::abs(pow(Number<27.l>{}, Number<2.l/3>{}).value() -  9) < 1e-9);
static_assert(std::abs(pow(Number< 8.l>{}, Number<2.l/3>{}).value() -  4) < 1e-9);
static_assert(std::abs(pow(Number< 9.l>{}, Number<3.l/2>{}).value() - 27) < 1e-9);
static_assert(std::abs(pow(Number< 4.l>{}, Number<3.l/2>{}).value() -  8) < 1e-9);

static_assert(ln(Number<1.l>{}).value() == 0);
static_assert(std::abs(ln(Number<2.l>{}).value() - 0.693147) < 1e-3);
static_assert(std::abs(ln(Number<1.l/2>{}).value() + 0.693147) < 1e-3);

static_assert(log(Number< 2.l>{}, Number<1.l>{}).value() == 0);
static_assert(log(Number< 8.l>{}, Number<1.l>{}).value() == 0);
static_assert(log(Number<10.l>{}, Number<1.l>{}).value() == 0);

static_assert(sin(Number<0.l>{}).value() == 0);
static_assert(std::abs(sin(Number< .0l>{}).value() - 0.000000) < 1e-3);
static_assert(std::abs(sin(Number< .1l>{}).value() - 0.099833) < 1e-3);
static_assert(std::abs(sin(Number< .2l>{}).value() - 0.198670) < 1e-3);
static_assert(std::abs(sin(Number< .3l>{}).value() - 0.295520) < 1e-3);
static_assert(std::abs(sin(Number< .4l>{}).value() - 0.389420) < 1e-3);
static_assert(std::abs(sin(Number< .5l>{}).value() - 0.479430) < 1e-3);
static_assert(std::abs(sin(Number< .6l>{}).value() - 0.564640) < 1e-3);
static_assert(std::abs(sin(Number< .7l>{}).value() - 0.644220) < 1e-3);
static_assert(std::abs(sin(Number< .8l>{}).value() - 0.717360) < 1e-3);
static_assert(std::abs(sin(Number< .9l>{}).value() - 0.783330) < 1e-3);
static_assert(std::abs(sin(Number<1.0l>{}).value() -  0.841471) < 1e-3);
static_assert(std::abs(sin(Number<1.1l>{}).value() -  0.891207) < 1e-3);
static_assert(std::abs(sin(Number<1.2l>{}).value() -  0.932039) < 1e-3);
static_assert(std::abs(sin(Number<1.3l>{}).value() -  0.963558) < 1e-3);
static_assert(std::abs(sin(Number<1.4l>{}).value() -  0.985450) < 1e-3);
static_assert(std::abs(sin(Number<1.5l>{}).value() -  0.997495) < 1e-3);
static_assert(std::abs(sin(Number<1.6l>{}).value() -  0.999574) < 1e-3);
static_assert(std::abs(sin(Number<1.7l>{}).value() -  0.991665) < 1e-3);
static_assert(std::abs(sin(Number<1.8l>{}).value() -  0.973848) < 1e-3);
static_assert(std::abs(sin(Number<1.9l>{}).value() -  0.946300) < 1e-3);
static_assert(std::abs(sin(Number<2.0l>{}).value() -  0.909297) < 1e-3);
static_assert(std::abs(sin(Number<2.1l>{}).value() -  0.863209) < 1e-3);
static_assert(std::abs(sin(Number<2.2l>{}).value() -  0.808496) < 1e-3);
static_assert(std::abs(sin(Number<2.3l>{}).value() -  0.745705) < 1e-3);
static_assert(std::abs(sin(Number<2.4l>{}).value() -  0.675463) < 1e-3);
static_assert(std::abs(sin(Number<2.5l>{}).value() -  0.598472) < 1e-3);
static_assert(std::abs(sin(Number<2.6l>{}).value() -  0.515501) < 1e-3);
static_assert(std::abs(sin(Number<2.7l>{}).value() -  0.427380) < 1e-3);
static_assert(std::abs(sin(Number<2.8l>{}).value() -  0.334988) < 1e-3);
static_assert(std::abs(sin(Number<2.9l>{}).value() -  0.239249) < 1e-3);
static_assert(std::abs(sin(Number<3.0l>{}).value() -  0.141120) < 1e-3);
static_assert(std::abs(sin(Number<3.1l>{}).value() -  0.041581) < 1e-3);
static_assert(std::abs(sin(Number<3.2l>{}).value() - -0.058374) < 1e-3);
static_assert(std::abs(sin(Number<3.3l>{}).value() - -0.157746) < 1e-3);
static_assert(std::abs(sin(Number<3.4l>{}).value() - -0.255541) < 1e-3);
static_assert(std::abs(sin(Number<3.5l>{}).value() - -0.350783) < 1e-3);
static_assert(std::abs(sin(Number<3.6l>{}).value() - -0.442520) < 1e-3);
static_assert(std::abs(sin(Number<3.7l>{}).value() - -0.529836) < 1e-3);
static_assert(std::abs(sin(Number<3.8l>{}).value() - -0.611858) < 1e-3);
static_assert(std::abs(sin(Number<3.9l>{}).value() - -0.687766) < 1e-3);
static_assert(std::abs(sin(Number<4.0l>{}).value() - -0.756802) < 1e-3);
static_assert(std::abs(sin(Number<4.1l>{}).value() - -0.818277) < 1e-3);
static_assert(std::abs(sin(Number<4.2l>{}).value() - -0.871576) < 1e-3);
static_assert(std::abs(sin(Number<4.3l>{}).value() - -0.916166) < 1e-3);
static_assert(std::abs(sin(Number<4.4l>{}).value() - -0.951602) < 1e-3);
static_assert(std::abs(sin(Number<4.5l>{}).value() - -0.977530) < 1e-3);
static_assert(std::abs(sin(Number<4.6l>{}).value() - -0.993691) < 1e-3);
static_assert(std::abs(sin(Number<4.7l>{}).value() - -0.999923) < 1e-3);
static_assert(std::abs(sin(Number<4.8l>{}).value() - -0.996165) < 1e-3);
static_assert(std::abs(sin(Number<4.9l>{}).value() - -0.982453) < 1e-3);
static_assert(std::abs(sin(Number<5.0l>{}).value() - -0.958924) < 1e-3);
static_assert(std::abs(sin(Number<5.1l>{}).value() - -0.925815) < 1e-3);
static_assert(std::abs(sin(Number<5.2l>{}).value() - -0.883455) < 1e-3);
static_assert(std::abs(sin(Number<5.3l>{}).value() - -0.832267) < 1e-3);
static_assert(std::abs(sin(Number<5.4l>{}).value() - -0.772764) < 1e-3);
static_assert(std::abs(sin(Number<5.5l>{}).value() - -0.705540) < 1e-3);
static_assert(std::abs(sin(Number<5.6l>{}).value() - -0.631267) < 1e-3);
static_assert(std::abs(sin(Number<5.7l>{}).value() - -0.550686) < 1e-3);
static_assert(std::abs(sin(Number<5.8l>{}).value() - -0.464602) < 1e-3);
static_assert(std::abs(sin(Number<5.9l>{}).value() - -0.373877) < 1e-3);
static_assert(std::abs(sin(Number<6.0l>{}).value() - -0.279415) < 1e-3);
static_assert(std::abs(sin(Number<6.1l>{}).value() - -0.182163) < 1e-3);
static_assert(std::abs(sin(Number<6.2l>{}).value() - -0.083089) < 1e-3);
static_assert(std::abs(sin(Number<6.3l>{}).value() -  0.016814) < 1e-3);
static_assert(std::abs(sin(Number<6.4l>{}).value() -  0.116549) < 1e-3);
static_assert(std::abs(sin(Number<6.5l>{}).value() -  0.215120) < 1e-3);
static_assert(std::abs(sin(Number<6.6l>{}).value() -  0.311541) < 1e-3);
static_assert(std::abs(sin(Number<6.7l>{}).value() -  0.404850) < 1e-3);
static_assert(std::abs(sin(Number<6.8l>{}).value() -  0.494113) < 1e-3);
static_assert(std::abs(sin(Number<6.9l>{}).value() -  0.578440) < 1e-3);

static_assert(cos(Number<0.l>{}).value() == 1.);
static_assert(std::abs(cos(Number< .0l>{}).value() -  1.000000) < 1e-3);
static_assert(std::abs(cos(Number< .1l>{}).value() -  0.995004) < 1e-3);
static_assert(std::abs(cos(Number< .2l>{}).value() -  0.980067) < 1e-3);
static_assert(std::abs(cos(Number< .3l>{}).value() -  0.955336) < 1e-3);
static_assert(std::abs(cos(Number< .4l>{}).value() -  0.921061) < 1e-3);
static_assert(std::abs(cos(Number< .5l>{}).value() -  0.877583) < 1e-3);
static_assert(std::abs(cos(Number< .6l>{}).value() -  0.825336) < 1e-3);
static_assert(std::abs(cos(Number< .7l>{}).value() -  0.764842) < 1e-3);
static_assert(std::abs(cos(Number< .8l>{}).value() -  0.696707) < 1e-3);
static_assert(std::abs(cos(Number< .9l>{}).value() -  0.621610) < 1e-3);
static_assert(std::abs(cos(Number<1.0l>{}).value() -  0.540302) < 1e-3);
static_assert(std::abs(cos(Number<1.1l>{}).value() -  0.453596) < 1e-3);
static_assert(std::abs(cos(Number<1.2l>{}).value() -  0.362358) < 1e-3);
static_assert(std::abs(cos(Number<1.3l>{}).value() -  0.267499) < 1e-3);
static_assert(std::abs(cos(Number<1.4l>{}).value() -  0.169967) < 1e-3);
static_assert(std::abs(cos(Number<1.5l>{}).value() -  0.070737) < 1e-3);
static_assert(std::abs(cos(Number<1.6l>{}).value() - -0.029200) < 1e-3);
static_assert(std::abs(cos(Number<1.7l>{}).value() - -0.128844) < 1e-3);
static_assert(std::abs(cos(Number<1.8l>{}).value() - -0.227202) < 1e-3);
static_assert(std::abs(cos(Number<1.9l>{}).value() - -0.323290) < 1e-3);
static_assert(std::abs(cos(Number<2.0l>{}).value() - -0.416147) < 1e-3);
static_assert(std::abs(cos(Number<2.1l>{}).value() - -0.504846) < 1e-3);
static_assert(std::abs(cos(Number<2.2l>{}).value() - -0.588501) < 1e-3);
static_assert(std::abs(cos(Number<2.3l>{}).value() - -0.666276) < 1e-3);
static_assert(std::abs(cos(Number<2.4l>{}).value() - -0.737394) < 1e-3);
static_assert(std::abs(cos(Number<2.5l>{}).value() - -0.801144) < 1e-3);
static_assert(std::abs(cos(Number<2.6l>{}).value() - -0.856889) < 1e-3);
static_assert(std::abs(cos(Number<2.7l>{}).value() - -0.904072) < 1e-3);
static_assert(std::abs(cos(Number<2.8l>{}).value() - -0.942222) < 1e-3);
static_assert(std::abs(cos(Number<2.9l>{}).value() - -0.970958) < 1e-3);
static_assert(std::abs(cos(Number<3.0l>{}).value() - -0.989992) < 1e-3);
static_assert(std::abs(cos(Number<3.1l>{}).value() - -0.999135) < 1e-3);
static_assert(std::abs(cos(Number<3.2l>{}).value() - -0.998295) < 1e-3);
static_assert(std::abs(cos(Number<3.3l>{}).value() - -0.987480) < 1e-3);
static_assert(std::abs(cos(Number<3.4l>{}).value() - -0.966798) < 1e-3);
static_assert(std::abs(cos(Number<3.5l>{}).value() - -0.936457) < 1e-3);
static_assert(std::abs(cos(Number<3.6l>{}).value() - -0.896758) < 1e-3);
static_assert(std::abs(cos(Number<3.7l>{}).value() - -0.848100) < 1e-3);
static_assert(std::abs(cos(Number<3.8l>{}).value() - -0.790968) < 1e-3);
static_assert(std::abs(cos(Number<3.9l>{}).value() - -0.725932) < 1e-3);
static_assert(std::abs(cos(Number<4.0l>{}).value() - -0.653644) < 1e-3);
static_assert(std::abs(cos(Number<4.1l>{}).value() - -0.574824) < 1e-3);
static_assert(std::abs(cos(Number<4.2l>{}).value() - -0.490261) < 1e-3);
static_assert(std::abs(cos(Number<4.3l>{}).value() - -0.400799) < 1e-3);
static_assert(std::abs(cos(Number<4.4l>{}).value() - -0.307333) < 1e-3);
static_assert(std::abs(cos(Number<4.5l>{}).value() - -0.210796) < 1e-3);
static_assert(std::abs(cos(Number<4.6l>{}).value() - -0.112153) < 1e-3);
static_assert(std::abs(cos(Number<4.7l>{}).value() - -0.012389) < 1e-3);
static_assert(std::abs(cos(Number<4.8l>{}).value() -  0.087499) < 1e-3);
static_assert(std::abs(cos(Number<4.9l>{}).value() -  0.186512) < 1e-3);
static_assert(std::abs(cos(Number<5.0l>{}).value() -  0.283662) < 1e-3);
static_assert(std::abs(cos(Number<5.1l>{}).value() -  0.377978) < 1e-3);
static_assert(std::abs(cos(Number<5.2l>{}).value() -  0.468517) < 1e-3);
static_assert(std::abs(cos(Number<5.3l>{}).value() -  0.554374) < 1e-3);
static_assert(std::abs(cos(Number<5.4l>{}).value() -  0.634693) < 1e-3);
static_assert(std::abs(cos(Number<5.5l>{}).value() -  0.708670) < 1e-3);
static_assert(std::abs(cos(Number<5.6l>{}).value() -  0.775566) < 1e-3);
static_assert(std::abs(cos(Number<5.7l>{}).value() -  0.834713) < 1e-3);
static_assert(std::abs(cos(Number<5.8l>{}).value() -  0.885520) < 1e-3);
static_assert(std::abs(cos(Number<5.9l>{}).value() -  0.927478) < 1e-3);
static_assert(std::abs(cos(Number<6.0l>{}).value() -  0.960170) < 1e-3);
static_assert(std::abs(cos(Number<6.1l>{}).value() -  0.983268) < 1e-3);
static_assert(std::abs(cos(Number<6.2l>{}).value() -  0.996542) < 1e-3);
static_assert(std::abs(cos(Number<6.3l>{}).value() -  0.999859) < 1e-3);
static_assert(std::abs(cos(Number<6.4l>{}).value() -  0.993185) < 1e-3);
static_assert(std::abs(cos(Number<6.5l>{}).value() -  0.976588) < 1e-3);
static_assert(std::abs(cos(Number<6.6l>{}).value() -  0.950233) < 1e-3);
static_assert(std::abs(cos(Number<6.7l>{}).value() -  0.914383) < 1e-3);
static_assert(std::abs(cos(Number<6.8l>{}).value() -  0.869397) < 1e-3);
static_assert(std::abs(cos(Number<6.9l>{}).value() -  0.815725) < 1e-3);

static_assert(number::abs(Number< 10.l>{}).value() == 10);
static_assert(number::abs(Number<- 5.l>{}).value() ==  5);
static_assert(number::abs(Number<  0.l>{}).value() ==  0);

static_assert(number::sign(Number< 10.l>{}).value() ==  1);
static_assert(number::sign(Number<- 5.l>{}).value() == -1);
static_assert(number::sign(Number<  0.l>{}).value() ==  0);

static_assert(number::min(Number< 10.l>{}).value() ==  10);
static_assert(number::min(Number<- 5.l>{}).value() == - 5);
static_assert(number::min(Number<  0.l>{}).value() ==   0);

static_assert(number::min(Number< 10.l>{}, Number<11.l>{}).value() == 10);
static_assert(number::min(Number<- 5.l>{}, Number<11.l>{}).value() == -5);
static_assert(number::min(Number<  0.l>{}, Number<11.l>{}).value() == 0);

static_assert(number::min(Number<-3.l>{}, Number< 10.l>{}, Number<11.l>{}).value() == -3);
static_assert(number::min(Number<-3.l>{}, Number<- 5.l>{}, Number<11.l>{}).value() == -5);
static_assert(number::min(Number<-3.l>{}, Number<- 0.l>{}, Number<11.l>{}).value() == -3);

static_assert(number::max(Number< 10.l>{}).value() ==  10);
static_assert(number::max(Number<- 5.l>{}).value() == - 5);
static_assert(number::max(Number<  0.l>{}).value() ==   0);

static_assert(number::max(Number< 10.l>{}, Number<11.l>{}).value() == 11);
static_assert(number::max(Number<- 5.l>{}, Number<11.l>{}).value() == 11);
static_assert(number::max(Number<  0.l>{}, Number<11.l>{}).value() == 11);

static_assert(number::max(Number<-3.l>{}, Number< 10.l>{}, Number<4.l>{}).value() == 10);
static_assert(number::max(Number<-3.l>{}, Number<- 5.l>{}, Number<4.l>{}).value() ==  4);
static_assert(number::max(Number<-3.l>{}, Number<  0.l>{}, Number<4.l>{}).value() ==  4);
