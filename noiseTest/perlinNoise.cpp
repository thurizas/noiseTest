#include "perlinNoise.h"
#include "utility.h"

#include <algorithm>                     // for std::iota
#include <numeric>                       // for std::iota

//static float Fade(const float t);
//static float Lerp(const float a, const float b, const float t);
//static float Grad(const std::uint8_t hash, const float x, const float y, const float z);
//static float Remap_01(const float x);
//static float Clamp_11(const float x);
//static float RemapClamp_01(const float x);
//static float Octave1D(BasicPerlinNoise& noise, float x, const std::int32_t octaves, const float persistence);
//static float Octave2D(BasicPerlinNoise& noise, float x, float y, const std::int32_t octaves, const float persistence);
//static float Octave3D(BasicPerlinNoise& noise, float x, float y, float z, const std::int32_t octaves, const float persistence);
//static float MaxAmplitude(const std::int32_t octaves, const float persistence);

BasicPerlinNoise::BasicPerlinNoise() : m_permutation{ 151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225, 140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23,
													 190,   6, 148, 247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,  57, 177,  33,  88, 237, 149,  56,  87, 174,  20,
													 125, 136, 171, 168,  68, 175,  74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,  60, 211, 133, 230, 220, 105,  92,
													  41,  55,  46, 245,  40, 244, 102, 143,  54,  65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169, 200, 196, 135, 130,
													 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,  52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212, 207,
													 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213, 119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43,
													 172,   9, 129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104, 218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,
													  12,  191,179, 162, 241,  81,  51, 145, 235, 249,  14, 239, 107,   49,192, 214,  31, 181, 199, 106, 157, 184,  84, 204, 176, 115, 121,  50,  45,
													 127,    4,150, 254, 138, 236, 205,  93, 222, 114,  67,  29,  24,   72,243, 141, 128, 195,  78,  66, 215,  61, 156,180 }
{ 
	std::random_device rd;
	m_defaultEngine = new std::mt19937(rd());
}

BasicPerlinNoise::~BasicPerlinNoise()
{
	if (nullptr != m_defaultEngine)
		delete m_defaultEngine;
}


void BasicPerlinNoise::reseed(uint64_t seed)
{
	std::iota(m_permutation.begin(), m_permutation.end(), uint8_t{ 0 });
	std::uniform_int_distribution<int>    dist(0, 255);

	// now reshuffle the permutation array...
	std::array<std::uint8_t, 256>::iterator  iter = m_permutation.begin();

	for(int ndx = 0; ndx < m_permutation.size(); ndx++)
	{
		int loc = dist(*m_defaultEngine);     // generate a random number in range 0....255
		
		// TODO: swap those elements...
		int temp = m_permutation[ndx];
		m_permutation[ndx] = m_permutation[loc];
		m_permutation[loc] = temp;
	}
}

double BasicPerlinNoise::getNoise(double x, double y, int32_t octaves, float persistence)
{
	return octave2D(x, y, octaves, persistence);
}


const std::array<std::uint8_t, 256>& BasicPerlinNoise::serialize() 
{
	return m_permutation;
}


void BasicPerlinNoise::deserialize(const std::array<std::uint8_t, 256>& state)
{
	m_permutation = state;
}



float BasicPerlinNoise::noise1D(const float x)
{
	return noise3D(x, static_cast<float>(defY), static_cast<float>(defZ));
}


float BasicPerlinNoise::noise2D(const float x, const float y)
{
	return noise3D(x, y, static_cast<float>(defZ));
}


float BasicPerlinNoise::noise3D(const float x, const float y, const float z)
{
	const float _x = std::floor(x);
	const float _y = std::floor(y);
	const float _z = std::floor(z);

	const std::int32_t ix = static_cast<std::int32_t>(_x) & 255;
	const std::int32_t iy = static_cast<std::int32_t>(_y) & 255;
	const std::int32_t iz = static_cast<std::int32_t>(_z) & 255;

	const float fx = (x - _x);
	const float fy = (y - _y);
	const float fz = (z - _z);

	const float u = Fade(fx);
	const float v = Fade(fy);
	const float w = Fade(fz);

	const std::uint8_t A = (m_permutation[ix & 255] + iy) & 255;
	const std::uint8_t B = (m_permutation[(ix + 1) & 255] + iy) & 255;

	const std::uint8_t AA = (m_permutation[A] + iz) & 255;
	const std::uint8_t AB = (m_permutation[(A + 1) & 255] + iz) & 255;

	const std::uint8_t BA = (m_permutation[B] + iz) & 255;
	const std::uint8_t BB = (m_permutation[(B + 1) & 255] + iz) & 255;

	const float p0 = Grad(m_permutation[AA], fx, fy, fz);
	const float p1 = Grad(m_permutation[BA], fx - 1, fy, fz);
	const float p2 = Grad(m_permutation[AB], fx, fy - 1, fz);
	const float p3 = Grad(m_permutation[BB], fx - 1, fy - 1, fz);
	const float p4 = Grad(m_permutation[(AA + 1) & 255], fx, fy, fz - 1);
	const float p5 = Grad(m_permutation[(BA + 1) & 255], fx - 1, fy, fz - 1);
	const float p6 = Grad(m_permutation[(AB + 1) & 255], fx, fy - 1, fz - 1);
	const float p7 = Grad(m_permutation[(BB + 1) & 255], fx - 1, fy - 1, fz - 1);

	const float q0 = Lerp(p0, p1, u);
	const float q1 = Lerp(p2, p3, u);
	const float q2 = Lerp(p4, p5, u);
	const float q3 = Lerp(p6, p7, u);

	const float r0 = Lerp(q0, q1, v);
	const float r1 = Lerp(q2, q3, v);

	return Lerp(r0, r1, w);
}


float BasicPerlinNoise::octave1D(const float x, const std::int32_t octaves, const float persistence)
{
	return Octave1D(*this, x, octaves, persistence);
}

float BasicPerlinNoise::octave2D(const float x, const float y, const std::int32_t octaves, const float persistence)
{
	return Octave2D(*this, x, y, octaves, persistence);
}

float BasicPerlinNoise::octave3D(const float x, const float y, const float z, const std::int32_t octaves, const float persistence)
{
	return Octave3D(*this, x, y, z, octaves, persistence);
}









