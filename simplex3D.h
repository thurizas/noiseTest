#pragma once

#include <cstdint>

#include <array>
class simplex3D
{
public:
    simplex3D();
    simplex3D(int64_t seed);

    void reseed(int64_t);
    double getNoise(double x, double y, double z) const;

private:
    const double m_stretch3d;
    const double m_squish3d;

    const double m_norm3d;

    const long m_defaultSeed;

    std::array<short, 256> m_perm;
    std::array<short, 256> m_permGradIndex3d;
    std::array<char, 72> m_gradients3d;
    double extrapolate(int xsb, int ysb, int zsb, double dx, double dy, double dz) const;
};
