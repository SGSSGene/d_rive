// g++ -std=c++17 exampleQuat.cpp -O3 -o exampleQuat

#include <iostream>

#include "vec3.h"
#include "quat.h"

#include "d_rive.h"

using namespace d_rive;



int main()
{
    // create quaternion with variables x<0>...x<3>
    auto q = make_quat(x<0>, make_vec3(x<1>, x<2>, x<3>));

    // create a point with variables x<10>...x<12>, setting real part to zero to allow quaternion multiplication
    auto p = make_quat(0_c, make_vec3(x<10>, x<11>, x<12>));

    std::cout << "q: " << q << "\n";
    std::cout << "p: " << p << "\n";

    // create quat expression
    auto expr = quat_mul(quat_mul(q, p), quat_conj(q));

    std::cout << full_simplify(expr) << "\n";

    // 1. do 4 partial derivation of variables each with 0...3
    //   -> produce a 4x expr (as tuple) in every a different expr is derived
    // 2. evaluate every expr with the given variables values
    auto pd = partial_derive(expr, x<0>, x<1>, x<2>, x<3>);
    std::cout << "w:\n" << std::get<0>(pd) << "\n";
    std::cout << "x:\n" << std::get<1>(pd) << "\n";
    std::cout << "y:\n" << std::get<2>(pd) << "\n";
    std::cout << "z:\n" << std::get<3>(pd) << "\n";

/*    auto r  = eval(pd, set(1_c), set<1>(0_c), set<2>(0_c), set<3>(0_c));
    std::cout << "w:\n" << std::get<0>(r) << "\n";
    std::cout << "x:\n" << std::get<1>(r) << "\n";
    std::cout << "y:\n" << std::get<2>(r) << "\n";
    std::cout << "z:\n" << std::get<3>(r) << "\n";*/

    return 0;
}
