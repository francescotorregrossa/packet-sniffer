
# Progetto Reti di calcolatori — Packet Sniffer
_**Di Aldo Fumagalli e Francesco Torregrossa, A.A. 19/20**_


## Introduzione

Abbiamo realizzato un programma in C che ascolta e analizza tutti i pacchetti ricevuti dal computer, permettendo di mostrare dettagli come la sorgente, il destinatario, i protocolli utilizzati, e anche i contenuti che essi trasportano.

Successivamente, abbiamo caricato il programma su un dispositivo [OrangePi](http://www.orangepi.org) munito di una scheda di rete wireless TP-Link [TL-WN722N](https://www.tp-link.com/it/home-networking/adapter/tl-wn722n/). Abbiamo anche preparato un sito web fittizio con le funzionalità di accesso e registrazione.

Così, accedendo e utilizzando il sito web, abbiamo potuto simulare l'invio di alcuni pacchetti che sono poi stati analizzati dal nostro programma, attivo sull'OrangePi. Questo ci ha permesso di accedere ai dati sensibili e di verificare la correttezza delle informazioni ottenute dal programma stesso.


## Sviluppo del programma

Per realizzare il programma abbiamo fatto uso delle seguenti librerie:

- `netinet/in.h` non sono sicuro che la stiamo usando veramente per ora
- `sys/socket.h` per richiedere al sistema la creazione di una socket
- `arpa/inet.h` non sono sicuro che la stiamo usando veramente per ora

### Protocolli e strutture dati

Ogni pacchetto di rete è stratificato, e ogni strato segue i suoi protocolli, ciascuno con delle proprietà e dei dati diversi. Perciò, partendo dai byte raw, abbiamo predisposto delle `struct` adatte a contenere gli header di ciascun protocollo.

Di seguito riportiamo la schematizzazione e la struttura in C relativa ad ogni protocollo analizzato, tenendo conto dei seguenti alias, scritti per fare chiarezza sui dati 
```c
typedef char byte;
typedef unsigned short word;
typedef byte *packet;
typedef char *string;
```

#### Ethernet
```
```

```c
typedef struct
{

    byte destination_host[6];
    byte source_host[6];
    word type_code;

} eth_header;
```

#### IP
```
```

```c
typedef struct
{

    byte header_length : 4,
         version : 4;
    byte type_of_service;
    word total_length;

    word id;
    word fragment_offset;

    byte time_to_live;
    byte protocol;
    word checksum;

    byte source_address[4];
    byte destination_address[4];

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
