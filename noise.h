#ifndef _noise_h_
#define _noise_h_

#include<cstdint>
#include <array>

class noise
{
public:
  noise() {}
  virtual ~noise() {}

  virtual double getNoise(const double, const double, int32_t, float p = 0.50f) = 0;
  virtual void reseed(uint64_t) = 0;

private:
};


#endif
