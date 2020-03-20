
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
  - [Analisi dei pacchetti](#analisi-dei-pacchetti)
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

Mediante le librerie `sys/socket.h` e `netinet/if_ether.h`, abbiamo richiesto al sistema la creazione di una socket in grado di ricevere qualsiasi pacchetto, la cui lettura viene realizzata tramite il metodo `read` della libreria `unistd.h`.

Nel dettaglio, la socket viene creata con questi parametri

```c
int socket(int family, int type, int protocol)
```

- `family` viene impostato a `AF_PACKET`, così la comunicazione può avvenire al livello di collegamento, quindi si riceveranno i frame Ethernet
- `type` viene impostato a `SOCK_RAW`, per accettare pacchetti con qualsiasi tipo di socket
- `protocol` viene impostato a `ETH_P_ALL`, per indicare che pacchetti con qualsiasi protocollo della `family` sono accettati (anche se successivamente analizzeremo solo quelli contenenti IPv4)

Pertanto, il codice che esegue la lettura è il seguente, in cui la funzione `ntohs` esegue il passaggio da `LITTLE_ENDIAN` a `BIG_ENDIAN` e viceversa (se necessario)

```c
int sock = socket(AF_PACKET, SOCK_RAW, ntohs(ETH_P_ALL));
if (sock < 0)
    perror("Socket creation error");

while (read(sock, buffer, PKT_LEN) > 0)
    analyze(buffer);
```

### Analisi dei pacchetti

Ogni volta che arriva un pacchetto qualsiasi, esso viene analizzato e scomposto utilizzando le funzioni e le strutture relative ai vari protocolli.

Di seguito riportiamo lo scheletro della funzione `analyze`, che mostra la logica con cui i pacchetti vengono estratti a vari livelli. La funzione vera, nel file `sniffer.c`, gestisce, tra le altre cose, anche i filtri sull'output a video.

```c
void analyze(packet buffer)
{
  eth_header eh = prepare_eth_header(buffer);
  describe_eth_header(eh);

  if (eh->type_code == ntohs(ETH_P_IP))
  {
    ip_header iph = prepare_ip_header(eh->next);
    describe_ip_header(iph);

    switch (iph->protocol)
    {
    case IPPROTO_ICMP:
    {
      icmp_header icmph = prepare_icmp_header(iph->next);
      describe_icmp_header(icmph);
      print_plaintext(icmph->next, iph->total_length - size_ip_header(iph) - size_icmp_header(icmph));
      break;
    }
    case IPPROTO_TCP:
    {
      tcp_header tcph = prepare_tcp_header(iph->next);
      describe_tcp_header(tcph);
      print_plaintext(tcph->next, iph->total_length - size_ip_header(iph) - size_tcp_header(tcph));
      break;
    }
    case IPPROTO_UDP:
    {
      udp_header udph = prepare_udp_header(iph->next);
      describe_udp_header(udph);
      print_plaintext(udph->next, iph->total_length - size_ip_header(iph) - size_udp_header(udph));
      break;
    }
    default:
      print_plaintext(iph->next, iph->total_length - size_ip_header(iph));
      break;
    }
  }
}
```

## Preparazione dell'OrangePi
l'OrangePi è una scheda **Open-Source** basata su architettura **ARM**, su questa scheda possono essere eseguite distribuzioni linux e quella che abbiamo utilizzzato noi è  **Armbian**, una distribuzione di linux creata per dispositivi che funzionano con architetture **ARM**.

Successivamente abbiamo configurato le impostazioni di rete, per poter collegare la scheda di rete wireless ad una rete Wi-Fi, nel seguente modo:

**Verifica dei driver della scheda di rete**
```bash
$ lspci -k
06:00.0 Network controller: Intel Corporation WiFi Link 5100
	Subsystem: Intel Corporation WiFi Link 5100 AGN
	Kernel driver in use: iwlwifi
	Kernel modules: iwlwifi
```
**Trovare il nome della scheda di rete**
```bash
$ iwconfig
wlan0  unassociated  Nickname:"<WIFI@REALTEK>"
          Mode:Auto  Frequency=2.412 GHz  Access Point: Not-Associated   
          Sensitivity:0/0  
          Retry:off   RTS thr:off   Fragment thr:off
          Power Management:off
          Link Quality:0  Signal level:0  Noise level:0
          Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
          Tx excessive retries:0  Invalid misc:0   Missed beacon:0
```

**Verifica avvenuta creazione interfaccia wireless**
```bash
$ ip link set dev wlan0 up
```
Non restituendo nessun errore, il terminale conferma l'avvenuta creazione.

**Attivazione dell'interfaccia**
```bash
$ iw wlan0 up
```
**Creazione file di configurazione**

```bash
/etc/wpa_supplicant.conf
ctrl_interface=/run/wpa_supplicant
update_config=1
```
**Avvio wpa_supplicant**
```bash
$ wpa_supplicant -B -i interface -c /etc/wpa_supplicant.conf
```
**Avvio del prompt interattivo del tool wpa **
```bash
$ wpa_cli
```
Scansione delle reti con stampa del risultato
```bash
> scan									#scansione reti
OK
<3>CTRL-EVENT-SCAN-RESULTS
> scan_results					#stampa
bssid / frequency / signal level / flags / ssid
00:00:00:00:00:00 2462 -49 [WPA2-PSK-CCMP][ESS] MIOSSID
11:11:11:11:11:11 2437 -64 [WPA2-PSK-CCMP][ESS] ALTROSSID
```
Aggiunta della rete con inserimento delle credenziali
```bash
> add_network						#aggiunta rete
0
> set_network 0 ssid "MIOSSID"
> set_network 0 psk "passphrase"
> enable_network 0
<2>CTRL-EVENT-CONNECTED - Connection to 00:00:00:00:00:00 completed (reauth) [id=0 id_str=]
```
Questo combinazione di comandi non fa altro che creare una configurazione di rete nel file ```/etc/wpa_supplicant.conf``` come segue:
```
network={
    ssid="MIOSSID"
    #psk="passphrase"
    psk=59e0d07fa4c7741797a4e394f38a5c321e3bed51d54ad5fcbd3f84bc7415d73d
}
```
Se la connessione è avvenuta con successo, allora bisogna salvare la configurazione:
```bash
>save_config
OK
```
**Richiesta indirizzo IP**
Ultimo passaggio è quello della richiesta dell'indirizzo IP per finalemente partecipare alla rete:
```bash
$ dhcpcd wlan0
```

**TODO** Gestione sessioni wpa_supplicant

## Sviluppo del sito web


## Prova
