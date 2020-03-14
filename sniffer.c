#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "headers.h"

#define PKT_LEN 8192
#define MAGIC1 65536
#define MAGIC2 768

void analyze(packet buffer);

int main(int argc, char *argv[])
{

    packet buffer = malloc(PKT_LEN);
    memset(buffer, 0, PKT_LEN);

    int sock = socket(AF_PACKET, SOCK_RAW, MAGIC2);
    if (sock < 0)
        perror("Errore socket()");

    while (read(sock, buffer, PKT_LEN) > 0)
        analyze(buffer);

    return 0;
}

void analyze(packet buffer)
{
    eth_header eh = prepare_ethernet_header(buffer);
    describe_eth_header(eh);

    if (eh->type_code == 8)
    {

        ip_header iph = prepare_ip_header(eh->next);
        describe_ip_header(iph);
        
        if (iph->protocol == 1)
        {
            icmp_header icmph = prepare_icmp_header(iph->next);
            describe_icmp_header(icmph);
        }
        else if(iph->protocol == 6)
        {
            
        }


        free(iph);
    }
    else
    {
    }

    free(eh);
}
