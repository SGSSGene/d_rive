#pragma once

#include "d_rive.h"

namespace d_rive {

template <typename ix, typename iy, typename iz>
struct Vec3 {
    ix x;
    iy y;
    iz z;

    friend std::ostream& operator<< (std::ostream& stream, Vec3 const& var) {
        stream << "vec3:\n";
        stream << var.x << "\n";
        stream << var.y << "\n";
        stream << var.z << "\n";
        return stream;
    }
};

template<typename P1, typename P2, typename P3>
auto make_vec3(P1 x, P2 y, P3 z) {
    return Vec3<P1, P2, P3>{x, y ,z};
}

template<typename T, int indexStart>
auto make_vec3() {
    return make_vec3(x<indexStart+0>, x<indexStart+1>, x<indexStart+2>);
}


template <typename Vec1, typename Vec2>
auto vec_cross_mul(Vec1 v1, Vec2 v2) {
    auto x = v1.y * v2.z - v1.z * v2.y;
    auto y = v1.z * v2.x - v1.x * v2.z;
    auto z = v1.x * v2.y - v1.y * v2.x;
    return make_vec3(x, y ,z);
}

template <typename Vec1, typename Vec2>
auto vec_add(Vec1 v1, Vec2 v2) {
    auto x = v1.x + v2.x;
    auto y = v1.y + v2.y;
    auto z = v1.z + v2.z;
    return make_vec3(x, y, z);
}


template <typename Vec, typename Scalar>
auto vec_scalar_mul(Scalar s, Vec vec) {
    auto x = s * vec.x;
    auto y = s * vec.y;
    auto z = s * vec.z;
    return make_vec3(x, y ,z);
}


template <typename Vec1, typename Vec2>
auto vec_dot_mul(Vec1 v1, Vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

}
