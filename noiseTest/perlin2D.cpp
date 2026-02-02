#include "perlin2D.h"
#include <cmath>


perlin2D::perlin2D() : m_perm(nullptr), m_seed(0) 
{
	m_perm = new uint8_t[512];

	for (int ndx = 0; ndx < 256; ndx++)
		m_perm[256 + ndx] = m_perm[ndx] = perm[ndx];
}

perlin2D::perlin2D(uint64_t s) : m_perm(nullptr), m_seed(s)
{
	m_perm = new uint8_t[512];

	for (int ndx = 0; ndx < 256; ndx++)
		m_perm[256 + ndx] = m_perm[ndx] = perm[ndx];

}
	

double perlin2D::noise(double x, double y, double z)
{
	int intX = (int)floor(x) & 255;      // calculate integer part of coordinates....
	int intY = (int)floor(y) & 255;
	int intZ = (int)floor(z) & 255;

	x -= floor(x);                      // calcualte fractional part of coordinates...
	y -= floor(y);
	z -= floor(z);

	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int A = m_perm[intX    ] + intY, AA = m_perm[A] + intZ, AB = m_perm[A + 1] + intZ;
	int B = m_perm[intX + 1] + intY, BA = m_perm[B] + intZ, BB = m_perm[B + 1] + intZ;

	return lerp(w, lerp(v, lerp(u, grad(m_perm[AA], x, y, z), grad(m_perm[BA], x - 1, y, z)),
		lerp(u, grad(m_perm[AB], x, y - 1, z), grad(m_perm[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(m_perm[AA + 1], x, y, z - 1), grad(m_perm[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(m_perm[AB + 1], x, y - 1, z - 1), grad(m_perm[BB + 1], x - 1, y - 1, z - 1))));
}


perlin2D::~perlin2D()
{
	delete[] m_perm;
}

// smoothing function to insure continueity at end points : 6t^5 -15t^4 + 10t^3
double perlin2D::fade(double t)
{
	return (t * t * t * (t * (t * 6 - 15) + 10));
}

// performing linear interpolation between a and b with t the distance along the vector
double perlin2D::lerp(double t, double a, double b)
{ 
	return (a + t * (b - a));
}

double perlin2D::grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}



