#include "icmp.h"

icmp_header prepare_icmp_header(packet data)
{
  icmp_header header = malloc(sizeof(struct icmp_header));
  memcpy(header, data, ICMP_HEADER_SIZE);
  header->next = data + ICMP_HEADER_SIZE;
  header->checksum = switch_encoding_w(header->checksum);
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