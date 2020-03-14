#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define UDP_HEADER_SIZE 8

struct udp_header
{
    word source_port;
    word destination_port;

    word length;
    word checksum;

    packet next;
};

typedef struct udp_header *udp_header;

udp_header prepare_udp_header(packet data)
{
    udp_header header = malloc(sizeof(struct udp_header));
    memcpy(header, data, TCP_HEADER_SIZE);
    header->next = data + UDP_HEADER_SIZE;

    header->source_port = switch_encoding(header->source_port);
    header->destination_port = switch_encoding(header->destination_port);

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