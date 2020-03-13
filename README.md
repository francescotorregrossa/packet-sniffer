
# Progetto Reti di calcolatori — Packet Sniffer
_**Di Aldo Fumagalli e Francesco Torregrossa, A.A. 19/20**_


## Introduzione

Abbiamo realizzato un programma in C che ascolta e analizza tutti i pacchetti ricevuti dal computer, permettendo di mostrare dettagli come la sorgente, il destinatario, i protocolli utilizzati, e anche i contenuti che essi trasportano.

Successivamente, abbiamo caricato il programma su un dispositivo [OrangePi](http://www.orangepi.org) munito di una scheda di rete wireless TP-Link [TL-WN722N](https://www.tp-link.com/it/home-networking/adapter/tl-wn722n/). Abbiamo anche preparato un sito web fittizio con le funzionalità di accesso e registrazione.

Così, accedendo e utilizzando il sito web, abbiamo potuto simulare l'invio di alcuni pacchetti che sono poi stati analizzati dal nostro programma, attivo sull'OrangePi. Questo ci ha permesso di accedere ai dati sensibili e di verificare la correttezza delle informazioni ottenute dal programma stesso.


## Sviluppo del programma

Per realizzare il programma abbiamo fatto uso delle seguenti librerie:

- `sys/socket.h` per richiedere al sistema la creazione di una socket tramite 
    ```c
    socket(AF_PACKET, SOCK_RAW, MAGIC2);
    ```

- `unistd.h` per leggere i contenuti della socket
    ```c
    read(sock, buffer, PKT_LEN);
    ```


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

} eth_header;
```

#### IP
```
 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|  ihl  |  vers  | type of service |            total length          |
+-+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+- + -+-+-+-+-+-+-+-+
|          identification          | f|f|f|     fragment offset       |
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

    byte
         header_length : 4,
         version : 4;
    byte type_of_service;
    word total_length;

    word id;
    word fragment_offset;

    byte time_to_live;
    byte protocol;
    word checksum;

    ip_address source_address;
    ip_address destination_address;

    packet next;

} ip_header;
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

Tutto ciò è stato raccolto all'interno del file `headers.h`, accompagnato da alcuni metodi di utilità scritti in `headers.c`. Questi metodi includono, per esempio, la conversione in stringa degli indirizzi `MAC` e di quelli `IP`.


## Preparazione dell'OrangePi


## Sviluppo del sito web


## Prova
