#include "datatypes.h"

word switch_encoding_w(word w)
{
  word lower = (w & (0b1111111100000000)) >> 8;
  word upper = (w & (0b0000000011111111)) << 8;
  return lower + upper;
}

dword switch_encoding_dw(dword dw)
{
  dword lower = (dw & (0b11111111000000001111111100000000)) >> 8;
  dword upper = (dw & (0b00000000111111110000000011111111)) << 8;
  return lower + upper;
}