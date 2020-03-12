#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include <netinet/ether.h>
/*
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/in.h>
*/
#include <arpa/inet.h>

#include "headers.h"

#define PKT_LEN 8192
#define MAGIC1 65536
#define MAGIC2 768

int main(int argc, char *argv []) {

    packet buffer = malloc(PKT_LEN);
    memset(buffer, 0, PKT_LEN);

    int sock = socket(AF_PACKET, SOCK_RAW, MAGIC2);
    if (sock < 0)
        perror("Errore socket()");
    

    while (read(sock, buffer, PKT_LEN) > 0) {

        packet eth_pointer = buffer;
        eth_header* eh = prepare_ethernet_header(eth_pointer);
        describe_eth_header(eh);
        

        if (eh->type_code == 8) {

            packet ip_pointer = buffer + sizeof(eth_header);
            ip_header* iph = prepare_ip_header(ip_pointer);

        }
        else {

        }
    }

    return 0;
}

/*
eth_header* ethernet_frame(byte_array data) {
    eth_header* header = malloc(sizeof(eth_header));
    memcpy(header, data, sizeof(eth_header));
    header->type_code = htons(header->type_code);
    return header;
}

ip_header* ip_frame(byte_array data) {
    ip_header* header = malloc(sizeof(ip_header));
    memcpy(header, data, sizeof(ip_header));
    return header;
}


void print_eth_header(eth_header* header) {
    printf("dest: ");
    for (int i = 0; i < 6; i++)
        printf("%04x", header->destination_host[i]);
    printf("\n");
}
*/