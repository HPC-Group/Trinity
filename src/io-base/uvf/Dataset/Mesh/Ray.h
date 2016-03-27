#pragma once

#include "silverbullet/math/Vectors.h"

class Ray
{
  public:
    Core::Math::Vec3d start;
    Core::Math::Vec3d direction;

    Ray(void) : start(), direction() { }
    Ray(const Core::Math::Vec3d& s, const Core::Math::Vec3d& d):start(s),direction(d) {}
    Core::Math::Vec3d PointOnRay(const double d) const {
      return start+direction*d;
    }
};
