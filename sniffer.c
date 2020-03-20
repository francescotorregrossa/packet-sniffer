#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>

#include "protocols/ethernet.h"
#include "protocols/ip.h"
#include "protocols/icmp.h"
#include "protocols/tcp.h"
#include "protocols/udp.h"

#define PKT_LEN 8192
#define MAX_TERMINAL_LINE_LENGTH 100

void analyze(packet buffer);
void print_separator();
void print_plaintext(packet data, dword size);

typedef struct
{
  unsigned char eth : 1,
                ip: 1,
                icmp: 1,
                tcp : 1,
                udp : 1,
                plain : 1,
                plainempty : 1,
                unknown : 1;
} * print_config;

print_config config;

int main(int argc, char *argv[])
{

  config = malloc(sizeof(unsigned char));
  config->eth = config->ip = config->icmp = config->tcp = config->udp =
      config->plain = config->plainempty = config->unknown = 1;
  
  for (int i = 1; i < argc; ++i)
  {
    // printf("argv[%d]: %s\n", i, argv[i]);

    if (strcmp("--noeth", argv[i]) == 0)
      config->eth = 0;
    if (strcmp("--noip", argv[i]) == 0)
      config->ip = 0;
    if (strcmp("--noicmp", argv[i]) == 0)
      config->icmp = 0;
    if (strcmp("--notcp", argv[i]) == 0)
      config->tcp = 0;
    if (strcmp("--noudp", argv[i]) == 0)
      config->udp = 0;
    if (strcmp("--noplain", argv[i]) == 0)
      config->plain = 0;
    if (strcmp("--noplainempty", argv[i]) == 0)
      config->plainempty = 0;
    if (strcmp("--nounknown", argv[i]) == 0)
      config->unknown = 0;
  }

  packet buffer = malloc(PKT_LEN);
  memset(buffer, 0, PKT_LEN);

  int sock = socket(AF_PACKET, SOCK_RAW, ntohs(ETH_P_ALL));
  if (sock < 0)
    perror("Errore socket()");

  while (read(sock, buffer, PKT_LEN) > 0)
    analyze(buffer);

  free(config);
  return 0;
}

void analyze(packet buffer)
{
  eth_header eh = prepare_eth_header(buffer);

  if (eh->type_code == ntohs(ETH_P_IP))
  {
    ip_header iph = prepare_ip_header(eh->next);
    
    switch (iph->protocol)
    {
    case IPPROTO_ICMP:
    {
      if (config->icmp)
      {
        icmp_header icmph = prepare_icmp_header(iph->next);
        
        dword psize = iph->total_length - size_ip_header(iph) - size_icmp_header(icmph);
        if (config->plainempty || psize) 
        {
          print_separator();

          if (config->eth)
            describe_eth_header(eh);
          if (config->ip)
            describe_ip_header(iph);
          
          describe_icmp_header(icmph);
          print_separator();
          
          if (config->plain)
          {
            print_plaintext(icmph->next, psize);
            print_separator();
          }

          printf("\n\n\n");
        }

        free_icmp_header(icmph);
      }
      break;
    }
    case IPPROTO_TCP:
    {
      if (config->tcp)
      {
        tcp_header tcph = prepare_tcp_header(iph->next);

        dword psize = iph->total_length - size_ip_header(iph) - size_tcp_header(tcph);
        if (config->plainempty || psize) 
        {
          print_separator();

          if (config->eth)
            describe_eth_header(eh);
          if (config->ip)
            describe_ip_header(iph);
          
          describe_tcp_header(tcph);
          print_separator();
          
          if (config->plain)
          {
            print_plaintext(tcph->next, psize);
            print_separator();
          }

          printf("\n\n\n");
        }

        free_tcp_header(tcph);
      }
      break;
    }
    case IPPROTO_UDP:
    {
      if (config->udp)
      {
        udp_header udph = prepare_udp_header(iph->next);

        dword psize = iph->total_length - size_ip_header(iph) - size_udp_header(udph);
        if (config->plainempty || psize) 
        {
          print_separator();

          if (config->eth)
            describe_eth_header(eh);
          if (config->ip)
            describe_ip_header(iph);
          
          describe_udp_header(udph);
          print_separator();

          if (config->plain)
          {
            print_plaintext(udph->next, psize);
            print_separator();
          }

          printf("\n\n\n");
        }

        free_udp_header(udph);
      }
      break;
    }
    default:
      if (config->unknown)
      {
        dword psize = iph->total_length - size_ip_header(iph);
        if (config->plainempty || psize)
        {
          if (config->eth || config->ip)
            print_separator();

          if (config->eth)
            describe_eth_header(eh);
          if (config->ip)
            describe_ip_header(iph);

          if (config->eth || config->ip || config->plain)
            print_separator();
          
          if (config->plain)
            print_plaintext(iph->next, psize);

          if (config->eth || config->ip || config->plain)
          {
            print_separator();
            printf("\n\n\n");
          }
        }
      }      
      break;
    }

    free_ip_header(iph);
  }
  else {
    if (config->unknown && config->eth && config->plainempty)
    {
      print_separator();
      describe_eth_header(eh);
      print_separator();
      printf("\n\n\n");
    }
  }

  free_eth_header(eh);
}

void print_separator()
{
  for (int i = 0; i < MAX_TERMINAL_LINE_LENGTH; i++)
    printf("-");
  printf("\n");
}

void print_plaintext(packet data, dword size)
{
  printf("Data Length: %d, Raw Data:\n", size);
  for (dword i = 0; i < size; i++)
  {
    if (i % 100 == 0 && i != 0)
      printf("\n");
    if (32 <= data[i] && data[i] <= 126)
      printf("%c", data[i]);
    else
      printf(".");
  }
  if (size > 0)
    printf("\n");
}