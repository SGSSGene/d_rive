# d_rive
[![forthebadge](https://forthebadge.com/images/badges/you-didnt-ask-for-this.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/uses-badges.svg)](https://forthebadge.com)

Compile time math expressions with c++20.
- standard math addition, subtraction, multiplication, division
- sin, cos, pow, ln, sign, abs, max and min
- derivation
- integration

Check example1.cpp and exampleQuat.cpp for more exciting examples..
To try the examples use a modern compiler and compile the .cpp file `g++ -std=c++20 example1.cpp`.

## Examples

(Assume you have `using namespace d_rive` set)

Lets start with something simple, a constant. Constant have to be written
with the `_c` suffix. that allows us to do everything at compile time. (`f() = 5`)
```
    auto f = 5_c;
    std::cout << f << "\n";
    std::cout << f() << "\n";
```

Lets define a polynomial and print the polynomial (`f(x) = x² + 2x + 3`)
```
    auto f = x<0>*x<0> + 2_c * x<0> + 3_c;
    std::cout << f << "\n";
```
Now lets evaluate it with `5` as `x<0>` (`f(x) = x² + 2x + 3`):
```
    auto f = x<0>*x<0> + 2_c * x<0> + 3_c;
    std::cout << f(5) << "\n";
```
Easy, right? But what if we need the derivative? (`f2 = f'`)
```
    auto f2 = derive(f);
    std::cout << f2 << "\n";
    std::cout << f2(5) << "\n";
```
Derivative of derivates is also possible (`f3 = f2'`)
```
    auto f3 = derive(f2);
    std::cout << f3 << "\n";
```

Integrating? (`F' = f`)
```
    auto F = integrate(f);
    std::cout << F << "\n";
    std::cout << F(5) << "\n";
```
Compile time roots? (`sqrt(2)`)
```
    std::cout << (2_c^0.5_c) << "\n";
```
Multiple variables: (`f(x0, x1, x2) = x0*x1 + x2`)
```
    auto f = x<0>*x<1> + x<2>;
    std::cout << f(1, 2, 3) << "\n";
    std::cout << f << "\n";
    std::cout << derive(f, x<1>) << "\n"; // derives for x<1> instead x<0>
```

Lets go crazy:
```
    auto f = 10_c + ((3_c * x<0>) ^ (0.5_c * x<1> * x<2> - x<1>));
    std::cout << derive(f, x<0>) << "\n";
    std::cout << derive(f, x<1>) << "\n";
    std::cout << derive(f, x<2>) << "\n";
    std::cout << integrate(f, x<0>) << "\n";
    std::cout << integrate(f, x<1>) << "\n";
    std::cout << integrate(f, x<2>) << "\n";
```
