#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "./protocols/ethernet.h"
#include "./protocols/ip.h"
#include "./protocols/icmp.h"
#include "./protocols/tcp.h"
#include "./protocols/udp.h"

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
    eth_header eh = prepare_eth_header(buffer);
    describe_eth_header(eh);

    if (eh->type_code == 8)
    {

        ip_header iph = prepare_ip_header(eh->next);
        describe_ip_header(iph);

        switch (iph->protocol)
        {
        case 1:
        {
            icmp_header icmph = prepare_icmp_header(iph->next);
            describe_icmp_header(icmph);
            free_icmp_header(icmph);
            break;
        }
        case 6:
        {
            tcp_header tcph = prepare_tcp_header(iph->next);
            describe_tcp_header(tcph);

            if (tcph->source_port == 80 || tcph->destination_port == 80)
            {
                dword http_size = iph->total_length - (iph->header_length + tcph->data_offset) * 4;
                packet data = malloc(http_size);
                memcpy(data, tcph->next, http_size);

                printf("\nDATA: ");
                for (dword i = 0; i < http_size; i++)
                    printf("%c", data[i]);
                printf("\n");

                free(data);
            }

            free_tcp_header(tcph);
            break;
        }
        case 17:
        {
            udp_header udph = prepare_udp_header(iph->next);
            describe_udp_header(udph);
            free_udp_header(udph);
            break;
        }
        default:
            break;
        }

        free_ip_header(iph);
    }
    else
    {
    }

    free_eth_header(eh);
}
