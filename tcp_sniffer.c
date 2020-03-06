#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define PKT_LEN 8192 //lunghezza pacchetto

int main(int argc, char *argv[])
{
  struct iphdr *ip;   //IP header
  struct tcphdr *tcp; //TCP header
  int sd;
  char *buffer; //buffer pacchetti IP + TCP
  char *dati;   //buffer dati pacchetto
  struct in_addr s_ind;
  char *IpOrigin, *IpDest;
  u_int16_t PortToCheck;
  IpOrigin = (char *)malloc(21);
  IpDest = (char *)malloc(21);
  buffer = (char *)malloc(PKT_LEN);
  memset(buffer, 0, PKT_LEN);
  sd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
  if (sd < 0)
    perror("Errore socket()");
  PortToCheck = 0;
  while (read(sd, buffer, 8192) > 0)
  {
    if (argc == 2)
      PortToCheck = atol(argv[1]);
    ip = (struct iphdr *)buffer;                   //header IP = inizio buffer
    tcp = (struct tcphdr *)(buffer + ip->ihl * 4); //inizio header TCP = buffer + spazio header IP
    printf("-- IP Header --\n");
    printf("Lunghezza header: %d bytes\n", ip->ihl * 4); //32 bits octets
    printf("Versione: %d\n", ip->version);
    printf("Priorità: %d\n", ip->tos);
    printf("Lunghezza pacchetto: %d bytes\n", ntohs(ip->tot_len));
    printf("ID: %d\n", ntohs(ip->id));
    printf("Offset: %d\n", ip->frag_off);
    printf("Time to live: %d\n", ip->ttl);
    printf("Protocollo trasporto: %d\n", ip->protocol);
    printf("Checksum: %d\n", ip->check);
    printf("Indirizzo origine in long int: %ul\n", ip->saddr);
    s_ind.s_addr = ip->saddr;
    IpOrigin = inet_ntoa(s_ind);
    printf("Indirizzo origine: %s\n", IpOrigin);
    s_ind.s_addr = ip->daddr;
    IpDest = inet_ntoa(s_ind);
    printf("Indirizzo destinazione in long int:%ul\n", ip->daddr);
    printf("Indirizzo destinazione: %s\n", IpDest);
    printf("-- TCP Header --\n");
    printf("Porta sorgente: %d\n", ntohs(tcp->source));
    printf("Porta destinazione: %d\n", ntohs(tcp->dest));
    printf("Numero di sequenza: %u\n", ntohl(tcp->seq));
    printf("Numero di ACK: %u\n", ntohl(tcp->ack_seq));
    printf("Lunghezza header TCP: %d bytes\n", tcp->doff * 4);
    printf("Flag FIN: %d\n", tcp->fin);
    printf("Flag SYN: %d\n", tcp->syn);
    printf("Flag RST: %d\n", tcp->rst);
    printf("Flag PSH: %d\n", tcp->psh);
    printf("Flag ACK: %d\n", tcp->ack);
    printf("Flag URG: %d\n", tcp->urg);
    printf("Finestra TCP: %d\n", ntohs(tcp->window));
    printf("Checksum: %d\n", tcp->check);
    printf("Puntatore Urgente: %d\n", tcp->urg_ptr);
    dati = (buffer + (ip->ihl * 4) + (tcp->doff * 4)); //inizio dati = spazio header IP + spazio header TCP
    printf("%s\n", dati);
    printf("**********************************************\n");
    memset(buffer, 0, PKT_LEN);
  }
  return 0;
}