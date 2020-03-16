
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

Ogni pacchetto di rete è stratificato, e ogni strato segue i suoi protocolli, ciascuno con delle proprietà e dei dati diversi. Perciò, partendo dai byte raw, abbiamo predisposto delle `struct` adatte a contenere gli header di ciascun protocollo.

Di seguito riportiamo la schematizzazione e la struttura in C relativa ad ogni protocollo analizzato, tenendo conto dei seguenti alias, definiti nel file [`datatypes.h`](./protocols/datatypes.h), scritti per fare chiarezza sui dati 
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

##### Header del protocollo

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

##### Definizione della struttura

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

##### Preparazione di un'istanza

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

##### Header del protocollo

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

##### Definizione della struttura

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

##### Preparazione di un'istanza

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

##### Header del protocollo

```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|      type      |       code      |             checksum             |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                               options                               |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
```

##### Definizione della struttura

```c

# define ICMP_HEADER_SIZE 8

struct icmp_header
{
  byte type;
  byte code;
  word checksum;

  dword options;

  packet next;
};

typedef struct icmp_header *icmp_header;
```

##### Preparazione di un'istanza

```c
icmp_header prepare_icmp_header(packet data)
{
  icmp_header header = malloc(sizeof(struct icmp_header));
  memcpy(header, data, ICMP_HEADER_SIZE);
  header->next = data + ICMP_HEADER_SIZE;
  header->checksum = switch_encoding_w(header->checksum);
  return header;
}
```

#### TCP

##### Header del protocollo

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

##### Definizione della struttura

```c

# define TCP_HEADER_SIZE 20

typedef struct
{
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

struct tcp_header
{
  word source_port;
  word destination_port;

  dword sequence_number;
  dword acknowldge_number;

  byte : 4,
      data_offset : 4;
  tcp_flags flags;
  word window_size;

  word checksum;
  word urgent;

  void *options;

  packet next;
};

typedef struct tcp_header *tcp_header;
```

##### Preparazione di un'istanza

```c
tcp_header prepare_tcp_header(packet data)
{
  tcp_header header = malloc(sizeof(struct tcp_header));
  memcpy(header, data, TCP_HEADER_SIZE);

  unsigned int options_length = size_tcp_header(header) - TCP_HEADER_SIZE;
  if (options_length)
  {
    header->options = malloc(options_length);
    memcpy(header->options, data + TCP_HEADER_SIZE, options_length);
  }
  else
    header->options = NULL;

  header->next = data + TCP_HEADER_SIZE + options_length;

  header->source_port = switch_encoding_w(header->source_port);
  header->destination_port = switch_encoding_w(header->destination_port);
  header->sequence_number = switch_encoding_dw(header->sequence_number);
  header->acknowldge_number = switch_encoding_dw(header->acknowldge_number);
  header->window_size = switch_encoding_w(header->window_size);
  header->checksum = switch_encoding_w(header->checksum);
  header->urgent = switch_encoding_w(header->urgent);

  return header;
}

dword size_tcp_header(tcp_header header) {
  return header->data_offset * 4;
}
```

#### UDP

##### Header del protocollo

```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|           source port            |          destination port        |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|              length              |              checksum            |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
```

##### Definizione della struttura

```c

# define UDP_HEADER_SIZE 8

struct udp_header
{
  word source_port;
  word destination_port;

  word length;
  word checksum;

  packet next;
};

typedef struct udp_header *udp_header;
```

##### Preparazione di un'istanza

```c
udp_header prepare_udp_header(packet data)
{
  udp_header header = malloc(sizeof(struct udp_header));
  memcpy(header, data, UDP_HEADER_SIZE);
  header->next = data + UDP_HEADER_SIZE;

  header->source_port = switch_encoding_w(header->source_port);
  header->destination_port = switch_encoding_w(header->destination_port);
  header->length = switch_encoding_w(header->length);
  header->checksum = switch_encoding_w(header->checksum);
  return header;
}
```

Tutto ciò è stato raccolto all'interno di vari file, `.h` e `.c`, una coppia per protocollo. I file contengono anche alcune funzioni di utilità tra cui, per esempio, troviamo la conversione in stringa degli indirizzi `MAC` e di quelli `IP`.

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
      break;
    }
    free_ip_header(iph);
  }
  else
  {
  }
  free_eth_header(eh);
}
```

## Preparazione dell'OrangePi


## Sviluppo del sito web


## Prova
