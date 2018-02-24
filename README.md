# d_rive
[![forthebadge](https://forthebadge.com/images/badges/you-didnt-ask-for-this.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/uses-badges.svg)](https://forthebadge.com)

Compile time math expressions with c++17.
- standard math addition, subtraction, multiplication, division
- sin, cos, pow, and ln
- derivation
- integration

Check example1.cpp and exampleQuat.cpp for more exciting examples

## Examples

(Assume you have `using namespace d_rive` set)

Lets start with something simple, a constant. Constant have to be written
with the `_c` suffix. that allows us to do everything at compile time.
```
    auto f = 5_c;
    std::cout << f << "\n";
    std::cout << f() << "\n";
```

Lets define a polynomial and print the polynomial
```
    auto f = x<0>*x<0> + 2_c * x<0> + 3_c;
    std::cout << f << "\n";
```
Now lets evaluate it with `5` as `x<0>`:
```
    auto f = x<0>*x<0> + 2_c * x<0> + 3_c;
    std::cout << f(5) << "\n";
```
Easy, right? But what if we need the derivative?
```
    auto f2 = derive(f);
    std::cout << f2 << "\n";
    std::cout << f2(5) << "\n";
```
Integrating?
```
    auto F = integrate(f);
    std::cout << F << "\n";
    std::cout << F(5) << "\n";
```
Compile time roots?
```
    std::cout << pow(2_c, 0.5_c) << "\n";
```
Multiple variables:
```
    auto f = x<0>*x<1> + x<2>;
    std::cout << f(1, 2, 3) << "\n";
    std::cout << f << "\n";
    std::cout << derive(f, x<1>) << "\n"; // derives for x<1> instead x<0>
```

Lets go crazy:
```
    auto f = 10_c + pow(3_c * x<0>, 0.5_c * x<1> * x<2> - x<1>);
    std::cout << derive(f, x<0>) << "\n";
    std::cout << derive(f, x<1>) << "\n";
    std::cout << derive(f, x<2>) << "\n";
    std::cout << integrate(f, x<0>) << "\n";
    std::cout << integrate(f, x<1>) << "\n";
    std::cout << integrate(f, x<2>) << "\n";
```
