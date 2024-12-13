# Il file system

Un **file system** è il sottosistema per la gestione dei file ed è la parte più visibile del S.O. Il suo scopo è fornire meccanismi per registrazione, accesso e protezione di dati e programmi, deve inoltre virtualizzare i dispositivi di memorizzazione permanente. Tramite il _file system_, il S.O. ha il dovere di fornire una visione logica e dei metodi di accesso uniformi ai diversi mezzi di memorizzazione.

Il _file system_ è responsabile della gestione dei file in memoria di massa:

- struttura i dati in file
- organizza i file in directory
- fornisce all'utente un insieme di funzioni di alto livello che permettono di operare sui file mascherando le operazioni che vengono realmente effettuate

In poche parole il file system garantisce una gestione dei file indipendente dalle caratteristiche fisiche dei dispositivi che costituiscono la memoria di massa

## File

Ogni **file** è un insieme di informazioni correlate e registrate nella memoria secondaria. Dal punto di vista dell'utente è la più piccola porzione di memoria secondaria indirizzabile logicamente. Inoltre, i dati possono essere scritti nella memoria secondaria tramite attraverso un file.

Dal punto di vista del S.O.i files sono mappati su dispositivi fisici di memorizzazione non volatili.

Un file può contenere dati o programmi.

### Struttura di un file

Un file può essere strutturato in vari modi:

- una sequenza di byte
- sequenza di record (linee, record a lunghezza fissa o variabile)
- struttura complessa (documento formattato, file eseguibile)

La struttura di un file è decisa dal S.O., dall'applicativo che crea il file o in generale da chi crea il file.

Tipicamente un file ha una struttura definita dal suo tipo, ma tutti i S.O. prevedono almeno un file di tipo **eseguibile**. Solo alcuni S.O. prevedono la gestione diretta di diverse strutture di file tramite operazioni specifiche.

### Attributi dei file

- **Nome**: id del file per l'utente
- **Identificativo**: numero che identifica il file all'interno del file system
- **Tipo**: tipo del file
- **Locazione**: puntatore al dispositivo e alla posizione del file sul dispositivo
- **Dimensione**: dimensione del file
- **Protezione**: parametri per il controllo di accesso per la lettura, scrittura e esecuzione del file
- **Ora, data**: dati necessari alla sicurezza del sistema e per il controllo d'uso

Queste informazioni sono conservate nella struttura di **directory** che risiede in memoria secondari.

### Operazioni sui file

**Creazione**:

- reperire lo spazio per memorizzare il file nel file system
- creare un nuovo elemento nella directory in cui registrare le informazioni del file (nome, posizione,...)

**Scrittura**:

- chiamata al sistema con nome del file e dati da scrivere
- reperimento del file nel file system
- scrittura dei dati nella posizione indicata dal _puntatore di scrittura_ e aggiornamento del puntatore

**Lettura**:

- chiamata al sistema con nome del file e indirizzo di memoria dove scrivere i file letti
- reperimento del file nel file system
- lettura dei dati nella posizione indicata dal _puntatore di lettura_ e aggiornamento del puntatore

Di solito è mantenuto un solo puntatore alla posizione corrente nel file che serve sia per lettura che scrittura.

**Posizionamento del file**:

- reperimento del file nel file system
- aggiornamento alla posizione corrente
- nessuna operazione I/O

**Cancellazione**:

- reperimento del file nel file system
- si rilascia lo spazio allocato al file e si elimina il corrispondente elemento della directory

**Troncamento**:

- cancellazione del contenuto del file, ma parte degli attributi restano immutati
- si rilascia lo spazio allocato al file

**Impostazione degli attributi**:

- reperimento e aggiornamento del relativo elemento di directory

Prima di qualsiasi accesso occorre aprire il file. Il S.O. mantiene in memoria centrale una tabella contenente informazioni su tutti i file aperti (**tabella dei file aperti**) e quando si richiede un operazione sul file si ricercano la informazioni relative tramite un indice nella tabella dei file aperti.

Quando il file non è più in uso deve essere chiuso e il S.O. rimuove l'elemento relativo nella tabella dei file aperti.

L'apertura avviene tramite un operazione `open(FILE)`. Questa operazione ricerca `FILE` nel file system, ne copia il contenuto nella tabella dei file aperti in memoria principale e restituisce il puntatore all'elemento nella tabella.

La chiusura avviene tramite `close(FILE)`. Questa operazione copia il contenuto di `FILE` presente in memoria principale nella directory nel disco. Rimuove poi il contenuto dalla _tabella dei file aperti_.

### File aperti

Nei sistemi multi utente sono presenti due livelli di tabelle.

**Tabella di sistema**:

- riferimenti a tutti i file aperti nel sistema
- posizione del file nel disco (cache delle informazioni di
accesso ai dati permanenti)
- dimensione del file
- date di ultimo accesso e modifica
- contatore di aperture (conta il numero di processi che hanno
aperto il file, una entry della tabella di sistema può essere rimossa quando il contatore vale 0)

**Tabella associata al processo**:

- riferimenti a tutti i file aperti dal processo
- puntatore alla posizione corrente nel file (ultima
locazione dove è stata realizzata un’operazione di
lettura/scrittura)
- diritti di accesso (controllati dal SO per permettere o negare
le operazioni di I/O richieste)
- informazioni di contabilizzazione
- gli elementi della _tabella dei file associata al processo_ puntano al relativo elemento contenuto nella _tabella di sistema_

#### Lock dei file aperti

Il lock dei file garantisce una mediazione per l'accesso condiviso a un file. Può essere obbligatorio o consigliato.

- **obbligatorio**: l'accesso al file è negato se il lock è già stato acquisito da un altro processo
- **consigliato**: i processi che trovano lo stato di un file impostato su bloccato devono decidere sul da farsi.

Se il lock è obbligatorio è il S.O. ad assicurare l'integrità dei dati, altrimenti è compito del programmatore.

### Tipi di file

Il tipo di un file indica la struttura logica interna del file e permette la corretta interpretazione da parte del S.O.

Esistono tre tecniche principali per gestire i tipi di file

- **estensioni**: il tipo è indicato da un suffisso del nome (DOS)
- **Attributo** “tipo” associato al file nella directory (MAC OS)
- **Magic number**: il tipo è rappresentato da un valore posto all’inizio del file (UNIX)

Il tipo dei un file e la sua struttura logica possono essere riconosciuti e gestiti in modi diversi dai S.O. Se il SO gestisce molti formati: il codice di sistema diventa più ingombrante, vi è incompatibilità di programmi con file di formato differente, ma di contro la gestione per i formati supportati è più efficiente.

Se il S.O. non gestisce molti formati il codice di sistema diventa più snello, ma i formati devono essere gestiti dal programmatore.

In UNIX tutti i file sono stringhe di byte e solo gli eseguibili hanno un formato specifico.

### Struttura interna dei file

La dimensione dei record logici di un file spesso non corrisponde alla dimensione dei record fisici sul disco. Questo fenomeno è detto _packing_ e la sua manifestazione più visibile avviene quando parte dell'ultimo blocco fisico contenete il file rimane inutilizzata.

### Modalità di accesso

Un file può essere acceduto in **modalità sequenziale**, ovvero, come in un nastro, si può leggere solo il blocco successivo/precedente rispetto alla posizione corrente.

Il metodo ad **accesso diretto** prevede la possibilità di accedere ad ogni blocco conoscendone il numero identificativo.

Accedendo tramite **acceso indicizzato** si fa riferimento ad un _file indice_ in cui sono contenuti i puntatori ai vari blocchi di memoria. Per reperire un elemento occorre prima cercare nell’indice l’elemento desiderato ed utilizzare poi il puntatore per accedere ai dati.

## Directory

La directory può essere vista come una tabella che ha un elemento per ogni nom e di file. Tale elemento contiene gli attributi del del file e le informazioni per accedere al contenuto del file su disco. Permette quindi di tradurre i nomi dei file negli element in essa contenuti permettendo l'accesso a dati e informazioni. Anche le directory risiedono su disco e sono un tipo di file.

### Informazioni

Le informazioni che una directory contiene sono:

- Nome
- Tipo
- Indirizzo
- Lunghezza attuale
- Lunghezza massima
- Data ultimo accesso
- Data ultima modifica
- ID del proprietario
- Informazioni di protezione

### Operazioni

Le operazioni che una directory permette sono:

- Ricerca di un file
- Creazione/aggiunta di un file
- Cancellazione di un file
- Elenco dei contenuti di una directory
- Ridenominazione di un file
- Attraversamento del file system

### Directory ad un livello

Questo tipo di approccio prevede che tutti i file di tutti gli utenti vengano messi in un unica directory. Non vi è alcun raggruppamento logico e occorre scegliere un nome diverso per ogni file. Quando il numero di file è grande questi problemi vengono accentuati.

### Directory a due livelli

Il sistema è diviso in **MFD** (Master File Directory) e **UFD** (User File Directory).

Ogni utente è una radice da cui partono i rami delle proprie cartelle. In questa situazione sono permessi nomi uguali per file di utenti diversi, ma non vi è raggruppamento logico se non quello basato sul proprietario. La ricerca diventa più efficiente in quanto si può specificare un percorso per identificare un file.

Quando si apre la sessione di lavoro si ricerca nella _MFD_ l’identificativo dell’utente, che viene ammesso al sistema all’interno della propria _UFD_.

Ogni riferimento a un file da parte dell'utente viene interpretato dal S.O. come riferito alla propria _UFD_. Se l'accesso è autorizzato, per accedere ai file di altri utenti è necessario utilizzare il pathname completo del file composto da nome utente e file.

I file di sistema vengono raccolti in opportune directory raggiungibili da tutti gli utenti.

### Directory con struttura ad albero

Il _pathname_ può essere **assoluto** o **relativo**. La creazione o cancellazione di un nuovo file viene effettuata nella cartella corrente e la creazione di una nuova directory viene interpretata come la creazione di una sottodirectory della cartella corrente.

La cancellazione è ammessa e cancella tutto il contenuto della directory.

### Directory a grafico aciclico

Caratterizzata da file e sottodirectory condivise implementate tramite **link** o tramite **duplicazione**. L'**aliasing** è permesso e nomi diversi possono essere utilizzati per riferirsi ad uno stesso oggetto.

I link vanno gestiti a parte e richiedono procedure per la creazione/cancellazione. **Risolvere un link** significa seguire il puntatore del link per reperire il file.

I _link_ fanno insorgere il problema della cancellazione: cancellando un file o una directory si rischia di creare puntatori _dandling_ che non punta a nulla. Una soluzione prevede di implementare puntatori all'indietro per consentire il reperimento e la cancellazione di tutti i link al file. In Questo modo un file viene conservato finché esiste almeno un link che punta a esso.

Mantenere il grafo _aciclico_ garantisce semplicità negli algoritmi di attraversamento. Per garantirlo tale sono ammessi link solo ai file, ma non alle directory. Permettendo di generare link alle directory bisogna verificare l'assenza di cicli.

### Directory a grafo generico

Possono introdurre cicli che devono essere gestiti tramite **garbage collection**: per implementare la cancellazione di parti non più riferite si attraversa il file system, marcando i file
accessibili e con un secondo passaggio, si rimuovono i file non
marcati.

## Montaggio del file system

Un file system deve essere montato in un _mount point_ per essere acceduto dal sistema.

Per montare un file system si fornisce al S.O. il nome del dispositivo da montare e la locazione che dovrà assumere nella struttura del file system. Solitamente la locazione è una directory vuota.

Una volta montato il file system risulta accessibile a programmi e utenti e diventa parte del _grafo delle directory_.

I sistemi Mac e Windows rilevano all'avvio in automatico tutti i file system, mentre in sistemi UNIX-like i file system devono essere montati esplicitamente tramite l'operazione di `mount`.

``` bash
mount /dev/usr /users
unmount /dev/usr
```

monta (e poi smonta) la partizione `/dev/usr` in `/user`.

In Linux `/etc/fstab` contiene il file per i dispositivi da montare automaticamente.

## Condivisione di un file

Nei sistemi distribuiti si usa **NFS** (Network File System).

Nei sistemi multiutente il modello più diffuso utilizza i concetti di **owner** e **group**. Il proprietario possiede tutti i diritti di unu file (o directory), mentre un gruppo definisce un insieme di utenti autorizzati a condividere l'accesso al file.

L'identificatore di un gruppo è il **GID**, mentre l'identificatore di un utente è il **UID** e sono memorizzati come attributi nel file (o directory).

## File system remoti

I file si possono condividere tramite la rete. Questa condivisione si implementa tramite:

- **trasferimento** (anonimo o autenticato) richiesto esplicitamente tramite un protocollo apposito (FTP, SFTP, ...)
- **file system distribuito** che permette la visibilità e l'accesso da remoto a file o directory come se fossero in locale
- **accesso tramite browser** (anonimo)

Nel modello _client-server_ il server mette a disposizione risorse (directory e file) ai client che ne fanno richiesta. I client possono essere molti, ma sono presenti problemi di autenticazione che vanno risolti tramite autenticazione reciproca e scambio di chiavi. Possono comunque persistere problemi di compatibilità nello scambio di chiavi.

**NFS** è il protocollo client-server standard in UNIX e prevede che gli UID coincidano nel client e nel server.

Dopo il montaggio del file system remoto, le richieste di accesso a file vengono inviate al server attraverso la rete ed è il server ad applicare i controlli di accesso. Qualora producano un risultato positivo, restituisce un **file handle** al client, che lo usa per eseguire le successive operazioni su file.

**CIFS** (Common Internet File System) è invece il protocollo Windows.

Le chiamate di sistema locali vengono tradotte in chiamate
(per gli stessi servizi) su file system remoti.

In caso di funzionamenti è necessario mantenere alcune informazioni di stato sul client e sul server, tuttavia i protocolli più diffusi, come _NFS_, non mantengono tali informazioni. _NFS_ trasferisce tutte le informazioni nella singola richiesta, ciò permette alta tolleranza ai guasti, ma bassa sicurezza.

La **semantica della coerenza** specifica quandole modifiche apportate da un utente ad un dato sono disponibili agli altri utenti, è direttamente correlata agli algoritmi di sincronizzazione fra processi. In UNIX le scritture sono immediatamente visibili agli altri utenti che hanno aperto lo stesso file e il puntatore nel file è condiviso fra gli accessi che si alternano.

## Protezione nel File System

Un file system deve gestire l'eventualità di danni e attacchi al sistema. L'**affidabilità** ai danni fisici è affidata alla **ridondanza**, mentre la **protezione** da accessi indesiderati si ottiene tramite **controllo degli accessi**.

Il proprietario di un file deve poter controllare quali tipi di accesso sono leciti e chi può accedere al file.

I tipi di accesso a file:

- lettura
- scrittura
- esecuzione
- append
- cancellazione
- elencazione degli attributi

Il controllo degli accessi si ottiene tramite

- liste di _utenti_ autorizzati
- liste di _gruppi di utenti_ autorizzati
- uso di password

## UNIX

Nel caso di sistemi UNIX file e directory possono appartenere a più directory, alcune di queste hanno uno scopo speciale:

- `/dev` contiene i device driver
- `/bin` contiene il codice eseguibile
- `/usr/include` contiene le librerie di sistema
- `/etc` contiene i file di configurazione

Ad ogni utente è associata una directory detta **home directory** che è una sotto-cartella di `/usr`.

Ciascun file è identificato dal pathname assoluto che include l'intero cammino dalla radice `/`. Tutti i file e sotto-cartella presenti nella stessa directory devono avere nomi distinti.

Quando un utente interagisce con il sistema ha una **present working directory** (**pwd**) che indica la sua posizione corrente nel grafo e può essere variata spostandosi nell'albero delle directory. Questo movimento può avvenire tramite **relative pathname**, ovvero omettendo il pathname corrispondente alla _pwd_.

Nello specifico `.` fa riferimento alla _pwd_ e il simbolo `..` fa riferimento alla directory che contiene la _pwd_.

### Operazioni su file e directory

- `mkdir`: crea una directory
- `cd`: cambia il contesto
- `ls`: elenca i file e le sottodirectory contenute in una directory
- `pwd`: mostra il pathname della directory corrente
- `rmdir`: rimuove una directory
- `mount`: monta un file system
- `man`: per consultare il manuale
- `rm`: cancellazione o rimozione

### Links

Un **hard link** è un etichetta o un nome alternativo assegnato ad un file. Accessibile tramite `ln`: il comando `ln oldfile newlink` crea nella _pwd_ un link relativo a `oldfile`. Qualsiasi cambiamento effettuato su `oldfile` riguarda anche `newlink` e viceversa. Dopo la rimozione del link il file viene mantenuto in memoria finché c'è almeno un nome a cui fa riferimento.

Un **soft link** è un tipo di file che punta ad un altro file e non contiene dati ad esso relativi. Se il file a cui punta viene cancellato questo link diventa inutilizzabile. Un _soft link_ si crea con il comando `ln -s filename mylink`.

### Protezione

Le tre modalità di accesso ad un file sono **read**, **write**, **execute**. A ciascun file sono associati tre campi (_user_, _group_, _all_), ciascuno composto dai bit `rwx`

- accesso del proprietario `RWX`
- accesso del gruppo `RWX`
- accesso pubblico `RWX`

I permessi possono essere cambiati tramite `chmod`, ad esempio `chmod 111101001 FILE` equivale a:

|              | R   | W   | X   |
| ------------ | --- | --- | --- |
| proprietario | 1   | 1   | 1   |
| gruppo       | 1   | 0   | 1   |
| pubblico     | 0   | 0   | 1   |

Il proprietario può leggere, scrivere ed eseguire, il gruppo può leggere ed eseguire, mentre il pubblico può solo eseguire.

Le triple possono essere espresse anche in decimale: `chmod 751 FILE`.

La stessa logica si applica alle directory.