#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef byte *packet;
typedef char *string;

// Ethernet Header

#define ETH_HEADER_SIZE 14

typedef struct
{

    byte a;
    byte b;
    byte c;
    byte d;
    byte e;
    byte f;

} mac_address;

typedef struct
{

    mac_address destination_host;
    mac_address source_host;
    word type_code;

    packet next;

} * eth_header;

// IP Header

#define IP_HEADER_SIZE 20

typedef struct
{

    byte a;
    byte b;
    byte c;
    byte d;

} ip_address;

typedef struct
{

    byte
        header_length : 4,
        version : 4;
    byte type_of_service;
    word total_length;

    word id;
    word : 1,
        flag_do_not_fragment : 1,
        flag_more_fragments : 1,
        fragment_offset : 13;

    byte time_to_live;
    byte protocol;
    word checksum;

    ip_address source_address;
    ip_address destination_address;

    packet next;

} * ip_header;

//ICMP

#define ICMP_HEADER_SIZE 8 ///////magic header

typedef struct {
    byte type;
    byte code;
    word checksum;

    packet next;
} * icmp_header;

//TCP

#define TCP_HEADER_SIZE 20

typedef struct {
    byte 
        cwr : 1,
        ece : 1,
        urg : 1,
        ack : 1,
        psh : 1,
        rst : 1,
        syn : 1,
        fin : 1;
} tcp_flags;

typedef struct {
    word source_port;
    word destination_port;
    dword sequence_number;
    dword acknowldge_number;
    byte 
        data_offset : 4,
         : 4;
    tcp_flags flags;
    word windows_size;
    word checksum;
    word urgent;

    packet next;
} * tcp_header;

eth_header prepare_ethernet_header(packet data);
ip_header prepare_ip_header(packet data);
icmp_header prepare_icmp_header(packet data);
tcp_header prepare_tcp_header(packet data);

string get_mac_address(mac_address address);
string get_ip_address(ip_address address);

void describe_eth_header(eth_header header);
void describe_ip_header(ip_header ip_header);
void describe_icmp_header(icmp_header ip_header);
void describe_tcp_header(tcp_header tcp_header);
