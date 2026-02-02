#ifndef _simplex2d_h_
#define _simplex2d_h_

#include "noise.h"

#include <cstdint>
#include <array>


class simplex2D : public noise
{
public:
    simplex2D();
    simplex2D(uint64_t seed);

    void reseed(uint64_t);
    //double eval(const double x, const double y) const;
    double getNoise(const double x, const double y, int32_t octaves = 0, float persistance = 0);

private:
    const double m_stretch2d;
    const double m_squish2d;

    const double m_norm2d;

    const long m_defaultSeed;

    std::array<short, 256> m_perm;
    std::array<char, 16>   m_gradients2d;
    double extrapolate(int xsb, int ysb, double dx, double dy) const;
};

#endif
