#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "headers.h"

#define PKT_LEN 8192
#define MAGIC1 65536
#define MAGIC2 768

int main(int argc, char *argv[])
{

    packet buffer = malloc(PKT_LEN);
    memset(buffer, 0, PKT_LEN);

    int sock = socket(AF_PACKET, SOCK_RAW, MAGIC2);
    if (sock < 0)
        perror("Errore socket()");

    while (read(sock, buffer, PKT_LEN) > 0)
    {

        packet eth_pointer = buffer;
        eth_header *eh = prepare_ethernet_header(eth_pointer);
        describe_eth_header(eh);

        if (eh->type_code == 8)
        {

            packet ip_pointer = buffer + sizeof(eth_header);
            ip_header *iph = prepare_ip_header(ip_pointer);
            describe_ip_header(iph);

            free(iph);
        }
        else
        {
        }

        free(eh);
    }

    return 0;
}
