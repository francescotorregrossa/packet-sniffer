#include "datatypes.h"

word switch_encoding_w(word w)
{
  word lower = (w & (0b1111111100000000)) >> 8;
  word upper = (w & (0b0000000011111111)) << 8;
  return lower + upper;
}

dword switch_encoding_dw(dword dw)
{
  dword a = (dw & (0b11111111000000000000000000000000)) >> 24;
  dword b = (dw & (0b00000000111111110000000000000000)) >> 8;
  dword c = (dw & (0b00000000000000001111111100000000)) << 8;
  dword d = (dw & (0b00000000000000000000000011111111)) << 24;
  return a + b + c + d;
}