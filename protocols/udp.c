#include "udp.h"

udp_header prepare_udp_header(packet data)
{
  udp_header header = malloc(sizeof(struct udp_header));
  memcpy(header, data, UDP_HEADER_SIZE);
  header->next = data + UDP_HEADER_SIZE;

  header->source_port = switch_encoding_w(header->source_port);
  header->destination_port = switch_encoding_w(header->destination_port);
  header->length = switch_encoding_w(header->length);
  header->checksum = switch_encoding_w(header->checksum);
  return header;
}

void free_udp_header(udp_header header)
{
  free(header);
}

// Utility functions

void describe_udp_header(udp_header header)
{
  printf("\t\tUDP Header:\n");
  printf("\t\t\t- Source Port: %hu, Destination Port: %hu, Length: %u\n", header->source_port, header->destination_port, header->length);
}