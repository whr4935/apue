/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

unsigned int roundUpToNextPowerOfTwo(unsigned int x)
{
  x--;
  x |= x >> 1;  // handle  2 bit numbers
  x |= x >> 2;  // handle  4 bit numbers
  x |= x >> 4;  // handle  8 bit numbers
  x |= x >> 8;  // handle 16 bit numbers
  x |= x >> 16; // handle 32 bit numbers
  x++;

  return x;
}

int main(int argc, char *argv[])
{
    std::cout << std::hex << std::showbase << roundUpToNextPowerOfTwo(9) << std::endl;
    return 0;
}
