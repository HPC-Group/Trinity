#pragma once

#include <cmath>
#include "silverbullet/base/SilverBulletBase.h"

namespace Core {
  namespace Math {
    namespace MathTools {
      
      template<typename T> T logN(T value, T base) {
        return T(log(double(value)) / log(double(base)));
      }
      
      template<typename T> T powN(T base, T exponent) {
        return T(0.5+pow(double(base), double(exponent)));
      }
            
      template<typename T> T log2(T n) {
        int iLog=0;
        while( n>>=T(1) ) iLog++;
        return iLog;
      }
      
      template<typename T> T pow2(T e) {
        return T(1) << e;
      }
      
      template<typename T> T gaussianSum(T n) {
        return n*(n+1)/2;
      }
      
      template<typename T>  bool isPow2(T n) {
        return ((n&(n-1))==0);
      }
      
      template<typename T>  T nextPow2(T n, bool bReturn_ID_on_Pow2=true) {
        if (bReturn_ID_on_Pow2 && isPow2(n)) return n;
        return pow2(log2(n)+1);
      }

      template<typename T> T sign(T v) {
        return T((v > T(0)) - (v < T(0)));
      }
      
      template<typename T> T MakeMultiple(T v, T m) {
        return v + (((v%m) == T(0)) ? T(0) : m-(v%m));
      }
      
      template<typename in, typename out>
      static inline out
      lerp(in value, in imin, in imax, out omin, out omax)
      {
        out ret = out(omin + (value-imin) * (static_cast<double>(omax-omin) /
                                             (imax-imin)));
        return ret;
      }
      
      template<typename T> T clamp(T val, T a, T b) {
        return std::max(a, std::min(b, val));
      }
    }
  }
}
