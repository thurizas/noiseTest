#ifndef _utility_h_
#define _utility_h_

#include "perlinNoise.h"

#include <cstdint>

static uint32_t perm[512];

int fastFloor(double);
double dot(const int*, double, double);
void makePerm();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// utility functions

std::uint64_t Random(const std::uint64_t max);


// fade function to minimize contribution close to end poin
float Fade(const float t);

// linear interpolation function
float Lerp(const float a, const float b, const float t);


float Grad(const std::uint8_t hash, const float x, const float y, const float z);


// remaps the input value to the interval [0, 1)
float Remap_01(const float x);

// clamps that input value to the interval [-1, 1]
float Clamp_11(const float x);

// clamps and remaps input value to the interval [0,1]
float RemapClamp_01(const float x);


float Octave1D(BasicPerlinNoise& noise, float x, const std::int32_t octaves, const float persistence);
float Octave2D(BasicPerlinNoise& noise, float x, float y, const std::int32_t octaves, const float persistence);
float Octave3D(BasicPerlinNoise& noise, float x, float y, float z, const std::int32_t octaves, const float persistence);
float MaxAmplitude(const std::int32_t octaves, const float persistence);

#endif
