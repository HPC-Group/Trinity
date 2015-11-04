#include "FractalIO.h"

#include <iostream>

int main(int argc, char** argv) {
  
  const Mandelbulb<uint8_t> b(10,10,10);
  
  for (int z = 0;z<10;++z) {
    for (int y = 0;y<10;++y) {
      for (int x = 0;x<10;++x) {
        uint8_t f = b.computePoint(x,y,z);
        std::cout << ((f < b.getMaxIterations()) ? ' ' : 'X') << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  
  return 0;
}