
# Progetto Reti di calcolatori — Packet Sniffer
_**di Aldo Fumagalli e Francesco Torregrossa, A.A. 19/20**_

Abbiamo realizzato un programma in C che ascolta e analizza tutti i pacchetti ricevuti dal computer, permettendo di mostrare dettagli come la sorgente, il destinatario, i protocolli utilizzati, e anche i contenuti che essi trasportano.

Successivamente, abbiamo caricato il programma su un dispositivo [OrangePi](http://www.orangepi.org) munito di una scheda di rete wireless TP-Link [TL-WN722N](https://www.tp-link.com/it/home-networking/adapter/tl-wn722n/). Abbiamo anche preparato un sito web fittizio con le funzionalità di accesso e registrazione.

Così, accedendo e utilizzando il sito web, abbiamo potuto simulare l'invio di alcuni pacchetti che sono poi stati analizzati dal nostro programma, attivo sull'OrangePi. Questo ci ha permesso di accedere ai dati sensibili e di verificare la correttezza delle informazioni ottenute dal programma stesso.

<!-- TOC depthfrom:2 depthto:4 -->

- [Sviluppo del programma](#sviluppo-del-programma)
  - [Protocolli e strutture dati](#protocolli-e-strutture-dati)
    - [Ethernet](#ethernet)
    - [IPv4](#ipv4)
    - [ICMP](#icmp)
    - [TCP](#tcp)
    - [UDP](#udp)
  - [Ricezione dei pacchetti](#ricezione-dei-pacchetti)
  - [Interpretazione dei pacchetti](#interpretazione-dei-pacchetti)
- [Preparazione dell'OrangePi](#preparazione-dellorangepi)
- [Sviluppo del sito web](#sviluppo-del-sito-web)
- [Prova](#prova)

<!-- /TOC -->

## Sviluppo del programma

### Protocolli e strutture dati

Ogni pacchetto di rete è stratificato, e ogni strato segue i suoi protocolli, ciascuno con delle proprietà e dei dati diversi. Perciò, partendo dai byte grezzi, abbiamo predisposto delle `struct` adatte a contenere gli header di ciascun protocollo e delle funzioni per analizzarli e visualizzarli. Il tutto è stato raccolto all'interno di vari file `.h` e `.c`, una coppia per protocollo. 

Per prima cosa, però, riportiamo i seguenti alias, che abbiamo definito nel file [`datatypes.h`](./protocols/datatypes.h) per fare chiarezza sui dati 
```c
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef byte *packet;
typedef char *string;
```

Abbiamo anche predisposto le seguenti funzioni per facilitare il passaggio tra codifiche `LITTLE_ENDIAN` e `BIG_ENDIAN`

```c
word switch_encoding_w(word w);
dword switch_encoding_dw(dword dw);
```

In questa relazione analizzeremo soltanto i protocolli Ethernet e IP perché le considerazioni fatte su di essi possono essere facilmente riportate per tutti gli altri protocolli presi in esame.

#### Ethernet

Il protocollo Ethernet prevede un header di lunghezza fissa di 14 byte, di cui 6 sono dedicati all'indirizzo MAC del destinatario e 6 a quello della sorgente, e gli ultimi 2 indicano il protocollo utilizzato. 

```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                   destination MAC (first 4 bytes)                   |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|  destination MAC (last 2 bytes)  |    source MAC (first 2 bytes)    |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                       source MAC (last 2 bytes)                     |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|            type code             |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- +
```

Abbiamo predisposto una struttura con 6 byte, nominati da `a` ad `f`, per rappresentare un singolo indirizzo MAC. Questa struttura è stata utilizzata nella realizzazione di quella principale, `eth_header`.

```c
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

struct eth_header
{
  mac_address destination_host;
  mac_address source_host;
  word type_code;

  packet next;  // puntatore al pacchetto incapsulato
};

typedef struct eth_header *eth_header;
```

La struttura contiene anche un puntatore `packet next` che serve per raggiungere facilmente il pacchetto incapsulato. Abbiamo definito la seguente funzione per assegnare il valore corretto al puntatore.

```c
eth_header prepare_eth_header(packet data);
{
  eth_header header = malloc(sizeof(struct eth_header));
  memcpy(header, data, ETH_HEADER_SIZE);
  header->next = data + ETH_HEADER_SIZE;
  return header;
}
```

#### IPv4

```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|  ihl  |  vers  | type of service |            total length          |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|          identification          | 0|d|m|     fragment offset       |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|  time to live  |    protocol     |              checksum            |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                          source IP address                          |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                       destination IP address                        |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                      options (variable length)                      |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
```

Anche per i pacchetti IPv4 abbiamo seguito la stessa logica, ovvero dopo aver analizzato l'header del pacchetto abbiamo creato una struttura adeguata ad ospitare i dati previsti. In particolare, è stato molto utile rappresentare alcuni dati utilizzando i bit field.

```c
#define IP_HEADER_SIZE 20

typedef struct
{
  byte a;
  byte b;
  byte c;
  byte d;
} ip_address;

struct ip_header
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

  void *options;

  packet next;
};

typedef struct ip_header *ip_header;
```

Come in precedenza, abbiamo definito la funzione `prepare_ip_header` per stabilire il valore del puntatore al pacchetto incapsulato `next`. 

Tuttavia, questo header non ha lunghezza fissa, perché potrebbe contenere delle `options`. Per sapere se le contiene possiamo leggere il parametro `header_length`, che ha un valore minimo di 5 (`options` non presente) e un valore massimo di 15, e va interpretato come numero di *gruppi di 4 byte*. Se vale 5, la lunghezza dell'header IP è di 5 gruppi di 4 byte, cioè 20 byte.

Inoltre, in questo header sono presenti dei dati di tipo `word` che devono essere convertiti prima dell'utilizzo.

```c
ip_header prepare_ip_header(packet data)
{
  ip_header header = malloc(sizeof(struct ip_header));
  memcpy(header, data, IP_HEADER_SIZE);

  unsigned int options_length = size_ip_header(header) - IP_HEADER_SIZE;
  if (options_length)
  {
    header->options = malloc(options_length);
    memcpy(header->options, data + IP_HEADER_SIZE, options_length);
  }
  else
    header->options = NULL;

  header->next = data + IP_HEADER_SIZE + options_length;

  header->total_length = switch_encoding_w(header->total_length);
  header->id = switch_encoding_w(header->id);
  // todo fragment offset encoding
  header->checksum = switch_encoding_w(header->checksum);

  return header;
}

dword size_ip_header(ip_header header) {
  return header->header_length * 4;
}
```

#### ICMP

```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|      type      |       code      |             checksum             |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                               options                               |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
```

#### TCP

```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|           source port            |          destination port        |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                           sequence number                           |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                       acknowledgment number                         |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|       |  data  | c|e|u|a|p|r|s|f |                                  |
|0 0 0 0| offset | w|c|r|c|s|s|y|i |              window              |
|       |        | r|e|g|k|h|t|n|n |                                  |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|            checksum              |           urgent pointer         |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                    options            ... | ...       padding       |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
```

#### UDP

```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|           source port            |          destination port        |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|              length              |              checksum            |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
```

### Ricezione dei pacchetti

Mediante la libreria `sys/socket.h` abbiamo richiesto al sistema la creazione di una socket, i cui parametri sono **magici**. La lettura dello stream appena aperto viene poi realizzata tramite il metodo `read` della libreria `unistd.h`.

```c
int sock = socket(AF_PACKET, SOCK_RAW, MAGIC2);
if (sock < 0)
    perror("Socket creation error");

while (read(sock, buffer, PKT_LEN) > 0)
    analyze(buffer);
```

- **todo** spiegare i parametri magici

### Interpretazione dei pacchetti

Ogni volta che arriva un pacchetto qualsiasi, esso viene analizzato e scomposto utilizzando le funzioni e le strutture relative ai vari protocolli.

```c
void analyze(packet buffer)
{
  eth_header eh = prepare_eth_header(buffer);
  if (eh->type_code == 8)
  {
    ip_header iph = prepare_ip_header(eh->next);
    switch (iph->protocol)
    {
    case 1:
    {
      icmp_header icmph = prepare_icmp_header(iph->next);
      print_plaintext(icmph->next, iph->total_length - size_ip_header(iph) - size_icmp_header(icmph));
      free_icmp_header(icmph);
      break;
    }
    case 6:
    {
      tcp_header tcph = prepare_tcp_header(iph->next);
      print_plaintext(tcph->next, iph->total_length - size_ip_header(iph) - size_tcp_header(tcph));
      free_tcp_header(tcph);
      break;
    }
    case 17:
    {
      udp_header udph = prepare_udp_header(iph->next);
      print_plaintext(udph->next, iph->total_length - size_ip_header(iph) - size_udp_header(udph));
      free_udp_header(udph);
      break;
    }
    default:
      print_plaintext(iph->next, iph->total_length - size_ip_header(iph));
      break;
    }
    free_ip_header(iph);
  }
  free_eth_header(eh);
}
```

## Preparazione dell'OrangePi


## Sviluppo del sito web


## Prova
