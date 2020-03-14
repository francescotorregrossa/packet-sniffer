
# Progetto Reti di calcolatori — Packet Sniffer
_**di Aldo Fumagalli e Francesco Torregrossa, A.A. 19/20**_

Abbiamo realizzato un programma in C che ascolta e analizza tutti i pacchetti ricevuti dal computer, permettendo di mostrare dettagli come la sorgente, il destinatario, i protocolli utilizzati, e anche i contenuti che essi trasportano.

Successivamente, abbiamo caricato il programma su un dispositivo [OrangePi](http://www.orangepi.org) munito di una scheda di rete wireless TP-Link [TL-WN722N](https://www.tp-link.com/it/home-networking/adapter/tl-wn722n/). Abbiamo anche preparato un sito web fittizio con le funzionalità di accesso e registrazione.

Così, accedendo e utilizzando il sito web, abbiamo potuto simulare l'invio di alcuni pacchetti che sono poi stati analizzati dal nostro programma, attivo sull'OrangePi. Questo ci ha permesso di accedere ai dati sensibili e di verificare la correttezza delle informazioni ottenute dal programma stesso.

<!-- TOC depthfrom:2 -->

- [Sviluppo del programma](#sviluppo-del-programma)
  - [Protocolli e strutture dati](#protocolli-e-strutture-dati)
    - [Ethernet](#ethernet)
    - [IP](#ip)
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

Di seguito riportiamo la schematizzazione e la struttura in C relativa ad ogni protocollo analizzato, tenendo conto dei seguenti alias, scritti per fare chiarezza sui dati 
```c
typedef unsigned char byte;
typedef unsigned short word;
typedef byte *packet;
typedef char *string;
```

#### Ethernet
```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                   Destination MAC (first 4 bytes)                   |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|  Destination MAC (last 2 bytes)  |    Source MAC (first 2 bytes)    |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|                       Source MAC (last 2 bytes)                     |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|            Type Code             |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- +
```

```c
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

    packet next;  // puntatore al pacchetto incapsulato

} * eth_header;

eth_header prepare_ethernet_header(packet data)
{
    eth_header header = malloc(ETH_HEADER_SIZE);
    memcpy(header, data, ETH_HEADER_SIZE);
    header->next = header + ETH_HEADER_SIZE;
    return header;
}
```

#### IP
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
|                        destination IP address                       |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
```

```c
typedef struct
{

    byte a;
    byte b;
    byte c;
    byte d;

} ip_address;

typedef struct
{

    byte header_length : 4,
         version : 4;
    byte type_of_service;
    word total_length;

    word id;
    word : 1,
         flag_do_not_fragment : 1,
         flag_more_fragments : 1,
         fragment_offset;

    byte time_to_live;
    byte protocol;
    word checksum;

    ip_address source_address;
    ip_address destination_address;

    packet next;

} * ip_header;

ip_header prepare_ip_header(packet data)
{
    ip_header header = malloc(IP_HEADER_SIZE);
    memcpy(header, data, IP_HEADER_SIZE);
    header->next = header + IP_HEADER_SIZE;
    return header;
}
```

#### TCP
```
```

```c
```

#### UDP
```
```

```c
```

Tutto ciò è stato raccolto all'interno del file [`headers.h`](./headers.h), accompagnato da alcuni metodi di utilità scritti in [`headers.c`](./headers.c). Questi metodi includono, per esempio, la conversione in stringa degli indirizzi `MAC` e di quelli `IP`.

Oltre ai normali contenuti di ogni pacchetto, le strutture `eth_header` e `ip_header` contengono un puntatore `next` che permette di raggiungere facilmente l'inizio del pacchetto incapsulato.

### Ricezione dei pacchetti

Mediante la libreria `sys/socket.h` abbiamo richiesto al sistema la creazione di una socket, i cui parametri sono magici. La lettura dello stream appena aperto viene poi realizzata tramite il metodo `read` della libreria `unistd.h`.

```c
int sock = socket(AF_PACKET, SOCK_RAW, MAGIC2);
if (sock < 0)
    perror("Socket creation error");

while (read(sock, buffer, PKT_LEN) > 0)
    analyze(buffer);
```

- **todo** spiegare i parametri magici

### Interpretazione dei pacchetti

Ogni volta che arriva un pacchetto qualsiasi, esso viene analizzato e scomposto utilizzando le funzioni e le strutture relative ai vari protocolli, a partire da quello [ethernet](#ethernet).

```c
void analyze(packet buffer)
{
    eth_header eh = prepare_ethernet_header(buffer);
    describe_eth_header(eh);

    if (eh->type_code == 8)
    {

        ip_header iph = prepare_ip_header(eh->next);
        describe_ip_header(iph);

        free(iph);
    }
    else
    {
    }

    free(eh);
}
```

## Preparazione dell'OrangePi


## Sviluppo del sito web


## Prova
