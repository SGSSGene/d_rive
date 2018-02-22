#pragma once

namespace d_rive {

template <typename iw, typename ix, typename iy, typename iz>
struct Quat {
    iw real;
    Vec3<ix, iy, iz> img;

    friend std::ostream& operator<< (std::ostream& stream, Quat const& var) {
        stream << var.real << "\n";
        stream << var.img.x << "\n";
        stream << var.img.y << "\n";
        stream << var.img.z << "\n";
        return stream;
    }
};

template <typename iw, typename ix, typename iy, typename iz>
constexpr auto simplify(Quat<iw, ix, iy, iz>) {
    auto w = simplify(iw{});
    auto x = simplify(ix{});
    auto y = simplify(iy{});
    auto z = simplify(iz{});
    return make_quat(w, make_vec3(x, y, z));
}


template <int nr, typename iw, typename ix, typename iy, typename iz>
constexpr auto derive_impl(Quat<iw, ix, iy, iz>) {
    auto w = derive_impl<nr>(iw{});
    auto x = derive_impl<nr>(ix{});
    auto y = derive_impl<nr>(iy{});
    auto z = derive_impl<nr>(iz{});
    return make_quat(w, make_vec3(x, y, z));
}

template<typename P1, typename P2, typename P3, typename P4>
auto make_quat(P1 w, Vec3<P2, P3, P4> v) {
    return Quat<P1, P2, P3, P4>{w, v};
}
template<typename T, int indexStart>
auto make_quat() {
    return make_quat(x<indexStart, T>, make_vec3<T, indexStart+1>());
}


template <typename Q1, typename Q2>
auto quat_mul(Q1 q1, Q2 q2) {
    auto real = q1.real * q2.real - vec_dot_mul(q1.img, q2.img);
    auto img  = vec_add(vec_add(vec_scalar_mul(q1.real, q2.img), vec_scalar_mul(q2.real, q1.img)), vec_cross_mul(q1.img, q2.img));
    return make_quat(real, img);
}

template <typename Q1>
auto quat_conj(Q1 q1) {
    return make_quat(q1.real, make_vec3(-q1.img.x, -q1.img.y, -q1.img.z));
}

}
