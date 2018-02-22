// g++ -std=c++17 example1.cpp -O3 -o example1

#include <iostream>

#include "d_rive.h"

using namespace d_rive;

int main()
{
    {
        std::cout << "\nexample: expression definition\n";

        // defines a statement, x<0> are free variables
        auto expr = 3_c * x<0>*x<0> + 2_c * x<0> + 1_c + 3_c;

        // how to print a statement
        std::cout << "expression:\n";
        std::cout << expr << "\n";

        std::cout << "simplified expression:\n";
        std::cout << full_simplify(expr) << "\n";

        std::cout << "step by step simplified expression:\n";
        std::apply([](auto...e) {
            auto f = [](auto e) {
                std::cout << e << "\n";
            };
            (f(e), ...);
        }, list_simplify(expr));

    }

    {
        std::cout << "\nexample: expression evaluation\n";

        // how to evaluate a statement
        auto f = 3_c * x<0>*x<0> + 2_c * x<0> + 1_c + 3_c;

        // set<0>(10.) means the variable x<0> is set to 10.
        std::cout << f << "\n"; // prints "324_c"
        std::cout << f(10.) << "\n"; // prints "324"
    }

    {
        std::cout << "\nexample: expression derivation\n";

        // show derivative of the expression
        auto expr = 3_c * x<0>*x<0> + 2_c * x<0> + 1_c + 3_c;
//        auto expr = x<0> * x<0>;

        std::cout << expr << "\n"; // prints "( ( 3_c * pow<2>(x<0>) ) + ( 2_c * x<0> ) + 4_c)"

        // derive d1
        auto d1 = derive(expr, x<0>);
        std::cout << d1 << "\n"; // prints "(2_c + (6_c * x<0>))";

        // derive d2 by deriving from d1
        auto d2a = derive(d1, x<0>);
        std::cout << d2a << "\n"; // prints "6_c";

        // derive d2 by deriving directly from expr
        auto d2b = derive(derive(expr, x<0>));
        std::cout << d2b << "\n"; // prints "6_c";

        // derive d3 by deriving directly from expr
        auto d3 = derive(derive(derive(expr, x<0>)));
        std::cout << d3 << "\n"; // prints "0_c";

        // derive d4 by deriving directly from expr
        auto d4 = derive(derive(derive(derive(expr, x<0>))));
        std::cout << d4 << "\n"; // prints "0_c";*/
    }
}
