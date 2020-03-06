#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>  // system defined identifiers.
#include <netinet/in.h> // internet address structure.
#include <sys/socket.h> // Need 4 socket(), bind()
#include <arpa/inet.h>
int main(int argc, char *argv[])
{
  int listSock, remSock;
  struct sockaddr_in Server_addr, Client_addr;
  socklen_t sin_size = sizeof(struct sockaddr_in);
  short port;
  char buffer[256];
  int ls_result;
  /*Creazione della Socket in ascolto per i Client*/
  listSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listSock < 0)
  {
    printf("Server:errore creazione socket.\n");
    exit(0);
  }
  else
    printf("La Listening Socket e’ partita\n");
  /* Effettua la bind sull’indirizzo e porta ora specificati */
  port = 8081;
  // zero the struct before filling the fields
  memset(&Server_addr, 0, sizeof(Server_addr));
  Server_addr.sin_family = AF_INET;
  Server_addr.sin_addr.s_addr = INADDR_ANY;
  Server_addr.sin_port = htons(port);
  if (bind(listSock, (struct sockaddr *)&Server_addr, sizeof(struct sockaddr)) < 0)
  {
    printf("Server: errore durante la bind.\n");
    exit(0);
  }
  /* La socket si pone in "ascolto" tramite la listen() */
  ls_result = listen(listSock, 10);
  if (ls_result < 0)
  {
    printf("Server: errore durante la listen.\n");
    exit(0);
  }
  else
    printf("La Socket è in Ascolto\n");
  /* La socket accetta la richiesta del Client */
  remSock = accept(listSock, (struct sockaddr *)&Client_addr, &sin_size);
  printf("Connessione ok con: %s\n", inet_ntoa(Client_addr.sin_addr));
  /* Il Server accetta il messaggio dal Client */
  recv(remSock, buffer, sizeof(buffer), 0);
  printf("Messaggio Arrivato: %s \n", buffer);
  printf("Chiudo il Server");
  close(remSock);
  return 0;
}