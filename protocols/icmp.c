#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define ICMP_HEADER_SIZE 8

struct icmp_header
{
  byte type;
  byte code;
  word checksum;

  dword options;

  packet next;
};

typedef struct icmp_header *icmp_header;

icmp_header prepare_icmp_header(packet data)
{
  icmp_header header = malloc(sizeof(struct icmp_header));
  memcpy(header, data, ICMP_HEADER_SIZE);
  header->next = data + ICMP_HEADER_SIZE;
  return header;
}

void free_icmp_header(icmp_header header)
{
  free(header);
}

// Utility functions

void describe_icmp_header(icmp_header header)
{
  printf("\t\tICMP Header:\n");
  printf("\t\t\t- Type: %d, Code: %d\n", header->type, header->code);
}