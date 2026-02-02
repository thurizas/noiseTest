//----------------------------------------------------------------------------------------
//
//	siv::PerlinNoise
//	Perlin noise library for modern C++
//
//	Copyright (C) 2013-2021 Ryo Suzuki <reputeless@gmail.com>
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//
//  Modified from the original:
//  (1) split in interface (*.h) and implememtnation (*.cpp) files.
//  (2) back-ported to C++11 standard, C++20 stuff so it could compile on older compilers
//  (3) removed namespaces and cleaned up includes
//
//----------------------------------------------------------------------------------------

/****************************************************************************************
 * Based on Perlin's 2002 reference implementation 
 *  (3.14, 42, 7) => 0.13691995878400012


*/

#ifndef _perlinNoise_h_
#define _perlinNoise_h_

#include "noise.h"
# include <cstdint>
# include <array>
# include <random>



# define MAJOR			3        // Library major version
# define MINOR			0        // Library minor version
# define REV			0        // Library revision version

// Library version
# define VERSION		((MAJOR * 100 * 100) + MINOR * 100) + (REV))

static const double defY=0.12345;
static const double defZ=0.34567;


class BasicPerlinNoise : public noise
{
public:
	BasicPerlinNoise();
	~BasicPerlinNoise();

	void reseed(uint64_t seed = 0);

	const std::array<std::uint8_t, 256>& serialize();
	void deserialize(const std::array<std::uint8_t, 256>& state);

	double getNoise(double x, double y, std::int32_t octaves, float persistence = 0.5f);



	///////////////////////////////////////
	//	Noise (The result is in the range [-1, 1])
	float noise1D(float x);
	float noise2D(float x, float y);
	float noise3D(float x, float y, float z);

	///////////////////////////////////////
	//	Octave noise (The result can be out of the range [-1, 1])

	float octave1D(float x, std::int32_t octaves, float persistence = 0.5f);
	float octave2D(float x, float y, std::int32_t octaves, float persistence = 0.5f);
	float octave3D(float x, float y, float z, std::int32_t octaves, float persistence = 0.5f);

	
private:

	std::array<std::uint8_t, 256>  m_permutation;
	std::mt19937*                  m_defaultEngine = nullptr;
};

#endif