#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

typedef char byte;
typedef byte* packet;
typedef char* string;


// Ethernet Header

typedef struct {

    byte destination_host[6];
    byte source_host[6];
    unsigned short type_code;

} eth_header;


// IP Header

typedef struct {
    unsigned int a  :8,
                 b  :8,
                 c  :8,
                 d  :8;
} ip_address;

typedef struct {

    byte header_length  :4,
         version        :4;
    byte type_of_service;
    unsigned short total_length;

    unsigned short id;
    unsigned short fragment_offset;

    byte time_to_live;
    byte protocol;
    unsigned short checksum;

    ip_address source_address;
    ip_address destination_address;

} ip_header;



eth_header* prepare_ethernet_header(packet data);
ip_header* prepare_ip_header(packet data);

string get_mac_address(byte address[6]);
string get_ip_address(ip_address address);

void describe_eth_header(eth_header* header);
void describe_ip_header(ip_header* ip_header);
