#include "utility.h"
#include "constants.h"

#include "perlinNoise.h"

int fastFloor(double x)
{
	return (x > 0 ? (int)x : (int)(x - 1));
}

double dot(const int* g, double x, double y)
{
	return (g[0] * x + g[1] * y);
}


std::uint64_t Random(const std::uint64_t max)
{
	std::random_device rd;
	std::mt19937       gen(rd());
	std::uniform_int_distribution<>  udist(0, max);
	return udist(gen);
}


// fade function to minimize contribution close to end poin
float Fade(const float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}


// linear interpolation function
float Lerp(const float a, const float b, const float t)
{
	return (a + (b - a) * t);
}


float Grad(const std::uint8_t hash, const float x, const float y, const float z)
{
	const std::uint8_t h = hash & 15;
	const float u = h < 8 ? x : y;
	const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


// remaps the input value to the interval [0, 1)
float Remap_01(const float x)
{
	return (x * float(0.5) + float(0.5));
}


// clamps that input value to the interval [-1, 1]
float Clamp_11(const float x)
{
	if (x < -1.0) return -1.0;
	else if (x > 1.0) return 1.0;
	else return x;
}


// clamps and remaps input value to the interval [0,1]
float RemapClamp_01(const float x)
{
	if (x <= float(-1.0))
	{
		return float(0.0);
	}
	else if (float(1.0) <= x)
	{
		return float(1.0);
	}

	return (x * float(0.5) + float(0.5));
}


float Octave1D(BasicPerlinNoise& noise, float x, const std::int32_t octaves, const float persistence)
{
	float result = 0;
	float amplitude = 1;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += (noise.noise1D(x) * amplitude);
		x *= 2;
		amplitude *= persistence;
	}

	return result;
}

float Octave2D(BasicPerlinNoise& noise, float x, float y, const std::int32_t octaves, const float persistence)
{

	float result = 0;
	float amplitude = 1;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += (noise.noise2D(x, y) * amplitude);
		x *= 2;
		y *= 2;
		amplitude *= persistence;
	}

	return result;
}

float Octave3D(BasicPerlinNoise& noise, float x, float y, float z, const std::int32_t octaves, const float persistence)
{
	float result = 0;
	float amplitude = 1;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += (noise.noise3D(x, y, z) * amplitude);
		x *= 2;
		y *= 2;
		z *= 2;
		amplitude *= persistence;
	}

	return result;
}

float MaxAmplitude(const std::int32_t octaves, const float persistence)
{
	float result = 0;
	float amplitude = 1;

	for (std::int32_t i = 0; i < octaves; ++i)
	{
		result += amplitude;
		amplitude *= persistence;
	}

	return result;
}