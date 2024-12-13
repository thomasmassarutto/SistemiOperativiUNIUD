# Introduzione

## Contenuti

1. Cosa è un sistema operativo
2. Organizzazione di un sistema di calcolo
3. Sistemi Mono/Multi programmati
4. Principali funzionalità di un sistema operativo

## Cosa è un sistema operativo

Un sistema operativo è un insieme di programmi che operano a stretto contatto con l'hardware in modo da semplificare all'utente l'uso e lo sviluppo di programmi e di efficientare l'utilizzo delle risorse hardware. Il S.O. funge quindi da assegnatore e controllore delle risorse hardware e solitamente è l'unico processo sempre in esecuzione nel sistema di calcolo, da qui il nome **kernel**.

## Organizzazione di un sistema di calcolo

In un sistema di calcolo CPU e devices (GPU, memoria di massa, ...) operano concorrentemente. Ogni device ha un controller che ne gestisce il buffer locale che permette la comunicazione fra device e CPU: il controller legge e scrive nel suo buffer locale e la cpu fa lo stesso. Per evitare clash la sincronizzazione fra controller e CPU viene gestita con il metodo delle **interruzioni**.

### Interruzioni

Tecnica di I/O per gestire l'accesso ad una risorsa condivisa.

1. Il controller lancia un segnale di interruzione
2. La CPU intercetta il segnale e lo riconosce
3. La CPU invoca una _procedura di servizio_ per gestire l'interruzione
4. Lo stato della CPU viene salvato
5. L'interruzione viene gestita
6. Lo stato della cpu viene ripristinato

Il **vettore delle interruzioni** è la tecnica maggiormente utilizzata.

### Memoria

Ci sono vari tipi di memoria:

1. registri
2. cache
3. memoria principale
4. memoria secondaria

Anche la memoria è una risorsa che va gestita.

#### DMA: Direct Access Memory

Alternativamente all' I/O, in questo caso la CPU invia una richiesta al controller.

1. La CPU invia una richiesta al controller del device
2. Il controller del device trasferisce in prima persona i dati dal suo buffer alla memoria principale
3. Il controller notifica la fine dell'operazione alla CPU

Con questa tecnica il processore e il controller possono lavorare contemporaneamente.

## Sistemi mono-programmati vs multi programmati

### Sistemi mono-programmati

In questi sistemi, oltre al S.O., nella memoria principale risiede al più un applicativo ed è possibile eseguire un solo lavoro, **job**, alla volta.

### Sistemai multi-programmati

Nella memoria principale sono presenti più programmi. Il S.O. ha il compito di selezionarne uno alla volta e di eseguirlo fino alla fine o finché non vi è un evento esterno che ne impedisce l'esecuzione (es: operazione I/O).

### Time sharing

Il S.O. esegue a intervalli predeterminati dei **context switch** per fare in modo che ogni lavoro abbia accesso alla risorsa "tempo di CPU" in maniera equa.

### Processo

Un processo è definito come un programma caricato in memoria e predisposto per l'esecuzione. Se lo spazio non è sufficiente i processi (o parte di essi) risiedono nella **job pool** in memoria di massa.

Il S.O. tramite il **job scheduling** sceglie quali processi caricare in memoria (**swap in** / **swap out**), mentre tramite il **CPU scheduling** decide quale fra i processi in memoria principale eseguire.

Un processo è caratterizzato da:

- un flusso sequenziale di esecuzione
- uno stato corrente
- una collezione di risorse assegnate

### Tecniche a supporto della multiprogrammazione

- **swapping**: migrazione dei processi fra memoria principale e secondaria
- **Memoria virtuale**: alcuni processi vengono eseguiti con parti caricate in memoria secondaria.

## Principali funzionalità di un sistema operativo

### Modalità di funzionamento

In presenza di più processi che condividono le stesse risorse è necessario garantire che nessun processo danneggi gli altri. Per garantire questo i S.O hanno almeno 2 modalità di esecuzione: **modalità utente** e **modalità kernel**. Quest'ultima è l'unica autorizzata a eseguire istruzioni privilegiate che permettono il passaggio di modalità, la gestione delle interruzioni, la gestione delle risorse, ... .

Per implementare questo meccanismo vengono utilizzate le **system call**.

Un processo utente richiede l'intervento del S.O. tramite una system call. Questa genera una interruzione (**trap**) che viene gestita tramite una routine interna al S.O in modalità kernel. Se la richiesta è legittima il servizio viene fornito e prima di restituire il controllo al processo utente si ritorna in modalità utente.

### Gestione processi

Il S.O. deve fornire le funzionalità per:

- creazione / cancellazione processi
- sospensione / ripristino processi
- comunicazione fra processi
- sincronizzazione fra processi
- gestione situazioni di stallo

### Gestione memoria centrale

Il S.O. deve fornire le funzionalità per la gestione della memoria principale:

- tenere traccia delle porzioni di memoria utilizzate da ciascun programma
- assegnare o revocare spazio in memoria ai processi eventualmente spostandoli in memoria secondaria

### Gestione memoria di massa

Il S.O. deve essere in grado di gestire e organizzare i file che compongono il file system

- creazione / cancellazione / accesso a file
- gestione affidabilità
- gestione spazio
- scheduling disco

### Gestione I/O

Il S.O. deve fornire le funzionalità per la gestione dell'I/O

- gestione dei buffer dei vari dispositivi
- gestione del caching fra dispositivi
- gestione dello spooling (esecuzione contemporanea, asincrona)
- mascherare all'utente le peculiarità dei vari dispositivi tramite i **device driver**

### Protezione e sicurezza

- **Protezione**: controllo dell'accesso alle risorse del sistema da parte dei processi o utenti.
- **Sicurezza**: strategia di difesa del sistema da accessi/operazioni dannose

Per garantire queste proprietà il S.O distingue gli utilizzatori tramite gli **user ID** e i **group ID**.

Nello specifico il sistema operativo deve garantire le proprietà di:

- Disponibilità dei dati evitando malfunzionamenti
- Privatezza dei dati
- Integrità dei dati in caso di modifiche non autorizzate o incontrollate
- Autenticazione e gestione dell'identità degli utenti
