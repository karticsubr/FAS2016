#ifndef __POINT2DH_
#define __POINT2DH_

#include <cmath>

struct Point2D
{
  Point2D(float x0=0, float y0=0) :x(x0), y(y0)
  { }

  Point2D(float x0, float y0, bool toroidal):x(x0), y(y0)
  {
    if (toroidal)
    {
      x = x0 - floor(x0) ;
      y = y0 - floor(y0) ;
    }
  }
  
  float x, y;
};


#endif // __POINT2DH_