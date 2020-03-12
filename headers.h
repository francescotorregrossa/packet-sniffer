#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

typedef char byte;
typedef byte* packet;
typedef char* string;

typedef struct {

    byte destination_host[6];
    byte source_host[6];
    short type_code;

} eth_header;

typedef struct {

    byte header_length  :4,
         version        :4;
    byte type_of_service;
    short total_length;

    short id;
    short fragment_offset;

    byte time_to_live;
    byte protocol;
    short checksum;

    unsigned int source_address;
    unsigned int destination_address;

} ip_header;

eth_header* prepare_ethernet_header(packet data);
ip_header* prepare_ip_header(packet data);

string get_mac_address(byte address[6]);
string get_ip_address(unsigned int address);

void describe_eth_header(eth_header* header);
void describe_ip_header(ip_header* ip_header);
