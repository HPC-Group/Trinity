#ifndef MD5_H
#define MD5_H

#include <array>
#include <vector>

#include <core/SilverBulletBase.h>

namespace Core {
  namespace IO {
    class MD5
    {
    public:
      MD5();
      virtual ~MD5() {};

      //RSA MD5 Implementation
      void Transform(const uint8_t Block[64], int& error);
      void Update(const uint8_t* Input, uint32_t nInputLen, int& error);
      std::vector<uint8_t> Final(int& iErrorCalculate);

    protected:
      inline uint32_t RotateLeft(uint32_t x, int n);
      inline void FF( uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T);
      inline void GG( uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T);
      inline void HH( uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T);
      inline void II( uint32_t& A, uint32_t B, uint32_t C, uint32_t D, uint32_t X, uint32_t S, uint32_t T);

      // Helpers
      inline void UINTToByte(uint8_t* Output, const uint32_t* Input, uint32_t nLength, int& error);
      inline void ByteToUINT(uint32_t* Output, const uint8_t* Input, uint32_t nLength, int& error);
      void MemoryMove(uint8_t* from, uint8_t* to, uint32_t size);

    private:
      uint8_t  m_lpszBuffer[64];    // InputBuffer
      uint32_t m_nCount[2] ;        // bitcount, modulo 2^64 (lsb first)
      uint32_t m_lMD5[4] ;          // MD5 sum
    };

    template<typename Iter, typename T>
    std::array<uint8_t,16> md5(Iter begin, Iter end) {
      MD5 md;
      int error = 0;
      while(begin != end) {
        md.Update((const uint8_t*)&*begin, sizeof(T), error);
        ++begin;
      }
      std::vector<uint8_t> final = md.Final(error);

      std::array<uint8_t,16> rv;
      std::copy(final.begin(), final.end(), rv.begin());
      return rv;
    }
  }
}

#endif // MD5_H

/*
 The MIT License

 Copyright (c) 2014 HPC Group, Univeristy Duisburg-Essen

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */
