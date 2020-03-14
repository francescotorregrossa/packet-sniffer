#include "tcp.h"

tcp_header prepare_tcp_header(packet data)
{
  tcp_header header = malloc(sizeof(struct tcp_header));
  memcpy(header, data, TCP_HEADER_SIZE);

  unsigned int options_length = header->data_offset * 4 - TCP_HEADER_SIZE;
  if (options_length)
  {
    header->options = malloc(options_length);
    memcpy(header->options, data + TCP_HEADER_SIZE, options_length);
  }
  else
    header->options = NULL;

  header->next = data + TCP_HEADER_SIZE + options_length;

  header->source_port = switch_encoding_w(header->source_port);
  header->destination_port = switch_encoding_w(header->destination_port);
  header->sequence_number = switch_encoding_dw(header->sequence_number);
  header->acknowldge_number = switch_encoding_dw(header->acknowldge_number);
  header->window_size = switch_encoding_w(header->window_size);
  header->checksum = switch_encoding_w(header->checksum);
  header->urgent = switch_encoding_w(header->urgent);

  return header;
}

void free_tcp_header(tcp_header header)
{
  if (header->options != NULL)
    free(header->options);
  free(header);
}

// Utility functions

void describe_tcp_header(tcp_header header)
{
  printf("\t\tTCP Header:\n");
  printf("\t\t\t- Source Port: %hu, Destination Port: %hu\n", header->source_port, header->destination_port);
  printf("\t\t\t- Sequence Number: %u, Acknowledgment: %u\n", header->sequence_number, header->acknowldge_number);
  printf("\t\t\t- Data Offset: %u, Window Size: %u", header->data_offset, header->window_size);
  if (header->flags.urg)
    printf(", Urgent: %u\n", header->urgent);
  else
    printf("\n");
  
  printf("\t\t\t- CWR: %u, ECE: %u, URG: %u, ACK: %u\n", header->flags.cwr, header->flags.ece, header->flags.urg, header->flags.ack);
  printf("\t\t\t- PSH: %u, RST: %u, SYN: %u, FIN: %u\n", header->flags.psh, header->flags.rst, header->flags.syn, header->flags.fin);
}
