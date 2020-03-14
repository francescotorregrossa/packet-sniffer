#include <stdlib.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef byte *packet;
typedef char *string;

word switch_encoding(word w)
{
  word lower = (w & (0b1111111100000000)) >> 8;
  word upper = (w & (0b0000000011111111)) << 8;
  return lower + upper;
}
