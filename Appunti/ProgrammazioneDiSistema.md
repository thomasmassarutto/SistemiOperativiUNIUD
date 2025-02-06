\newpage

# Gestione dei processi

## Attributi dei processi

Necessitano di `#include <sys/types.h>`

### `getpid()`

Restituisce il PID del processo chiamante.

`pid_t getpid(void)`

### `getppid()`

Restituisce il PID del processo parent.

`pid_t getppid(void)`

### `getpgrp()`

Restituisce il PGID del processo chiamante

`pid_t getpgrp(void)`

### `getuid()`

Restituisce l'UID del utente che possiede il processo chiamante

`uid_t getuid(void)`

### `getgid()`

Restituisce il group ID dell'utente che possiede il processo chiamante

`git_t getgid(void)`

## Errori

### `perror()`

Stampa sullo standard error un messaggio d'errore.

`void perror(const char *s)`

- `const char *s`: stringa contenente il messaggio d'errore 

## Ambiente

Necessitano di `#include <stdlib.h>`

### `getenv()`

Ritorna un puntatore alla stringa che contiene la variabile d'ambiente. Se la variabile non esiste il valore di ritorno è `NULL`.

`char *getenv(const char *name)`

-  `const char *name`: nome della variabile

### `setenv()`

Modifica il valore ad una variabile d'ambiente.

`int setenv(const char *name, const char *value, int overwrite)`

- `const char *name`: nome della variabile da modificare. Se non esiste viene creata
- `const char *value`: nuovo valore da assegnare alla variabile
- `int overwrite`: se settata a 1 permette l'overwrite del valore nel caso la variabile esista

### `unsetenv()`

Elimina una variabile d'ambiante.

`int unsetenv(const char *name)`

- `const char *name`: nome della variabile da eliminare

## Creazione e teminazione

Necessitano di `<stdlib.h>`, 

### `fork()`

Crea un processo e ritorna il PID del figlio creato. Il figlio è un processo duplicato del padre.

`pid_t fork(void)`

### `exit()`

Causa la terminazione del processo che la invoca (_terminated_, _zombie_). Non ha valore di ritorno, ma il byte _status_ viene salvato nel PCB. Lo spazio d'indirizzamento occupato viene liberato.

`void exit(int status)`

- `int status`: indica il successo o l'eventuale codice associato alla terminazione del processo.

### `wait()`

Il parent eseguendo la `wait()` si porta in stato di _waiting_ per aspettare la terminazione di un figlio. Se almeno uno è _teminated_ ritorna il controllo al chiamante che torna _ready_.

`pid_t wait(int *wsatus)`

- `int *wsatus`: contiene le info relative alla terminazione del figlio.

### `waitpid()`

Similare alla `wait()`, ma attende la terminazione di uno specifico figlio.

`pid_t waitpid(pid_t pid, int *wstatus, int options)`

- `pid_t pid`: PID del processo da attendere
- `int *wsatus`: contiene le info relative alla terminazione del figlio.
- `int options`:

## Cambio di programma 

Il cambio di programma da eseguire è implementato da varie system call. La variante base è la `execve()`.

### `execve()`

`int execve (const char *filename, char *const argv[], char *const envp[])`

- `const char *filename`: nuovo file da eseguire
- `char *const argv[]`: array degli argomenti
- `char *const envp[]`: array delle variabili d'ambiente formattato "VAR=VAL" e con l'ultimo valore settato a `NULL` 

### `execl()`

`int execl(const char *path, const char *arg_1 ... arg_n, (char*) NULL)`

- `const char *path`: path del nuovo file da eseguire
- `const char *arg_1 ... arg_n`: numero variabile di argomenti 
- `(char*) NULL`: `NULL` che indica la fine degli argomenti passati al nuovo programma

# Files e directories

## Operare su files

### `open()`

Permette l'apertura o la creazione di un file restituendo un file descriptor. il file descriptor può essere utilizzato successivamente come riferimento al file aperto.

`int open(const char *pathname, int flags, mode_t perm)`

- `const char *pathname`: indica il pathname del file
- `int flags`: indica le flag riguardanti l'apertura del file
  - `O_RDONLY`: solo lettura
  - `O_WRONLY`: solo scrittura
  - `O_RDRW`: sia lettura che scrittura
  - `O_CREAT`: se non esiste, crea il file con i permessi di `perm`
  - `O_APPEND`: le scritture avvengono solo in coda al file
  - ... 
  Nel caso di altri flag si possono mettere in or bit a bit: `FLAG1 | FLAG2`
- `mode_t perm`:

### `read()`

Legge dal file selezionato attraverso il file descriptor al massimo $n$ byte e li copia in un buffer. Restituisce il numero di byte letto.

`ssize read(int fd, void *buffer, size_t n)`

- `int fd`: file descriptor del file da cui leggere i dati
- `void *buffer`: buffer da riempire
- `size_t n`: numero massimo di bytes da leggere

### `write()`

Opera similmente alla `read()`.

`ssize write(int fd, void *buffer, ssize_t n)`

- `int fd`: file descriptor del file su cui scrivere i dati
- `void *buffer`: contenuto del buffer da scrivere sul file
- `ssize_t n`: numero massimo di bytes da scrivere

### `lseek()`

Sposta le posizioni dei puntatori di lettura e scrittura di un file aperto a partire dalla posizione `whence`.

`off_t lseek(int fd, off_t offset, int whence)`

- `int fd`: file descriptor del file
- `off_t offset`: offset di cui spostare il puntatore
- `int whence`: posizione iniziale nel file da cui cominciare a contare per lo spostamento
  - `SEEK_SET`: inizio del file
  - `SEEK_CUR`: posizione corrente
  - `SEEK_END`: fine del file

### `close()`

Elimina il file descriptor di un file. Se questo era il suo l'ultimo descrittore attivo, il file viene chiuso.

`int close(int fd)`

- `int fd`: file descriptor del file

### `unlink()`

Rimuove l'hard link associato al pathname fornito. Se era l'ultimo allora elimina il file.

`int unlink(const char *pathname)`

- `const char *pathname`: pathname su cui eseguire la system call

## Attributi

Attributi di file e directory.

### `fstat()`

Accede agli attributi di un file o di una directory.

`int fstat(int fd, struct stat *statbuffer)`

- `int fd`: file descriptor
- `struct stat *statbuffer`: struttura contenente gli attributi

```c
struct stat{
    dev_t st_dev; // ID of device containing file
    ino_t  st_ino; // Inode number
    mode_t st_mode; // File type and mode
    nlink_t st_nlink; // Number of hard links
    uid_t st_uid; // User ID of owner
    gid_t st_gid; // Group ID of owner
    dev_t st_rdev; // Device ID (if special file)
    off_t st_size; // Total size, in bytes
    blksize_t st_blksize;// Block size for filesystem I/O
    blkcnt_t st_blocks; // Number of 512B blocks allocated
    struct timespec st_atim; // Time of last access
    struct timespec st_mtim; // Time of last modification
    struct timespec st_ctim; // Time of last status change
}
```
Il campo `st_mode` indica il tipo del file ed è possibile analizzarne il contenuto tramite delle macro predefinite: 

- `S_ISREG`: file regolare
- `S_ISDIR`: directory
- `S_ISFIFO`: named pipe
- `S_ISSOCK`: socket
- ...

### `fcntl()`

Consente di accedere e di modificare i flag di un file utilizzando il file descriptor.

`int fcntl(int fd, int cmd ...arg...)`

- `int fd`: file descriptor
- `int cmd`: esegue ii comandi specificati dalle flag come:
  - `F_GETFL`: restituisce flag relativi all access mode
  - `F_SETFL`: imposta i flag specificati in arg
  - `F_SETLK`, `F_SETLKW`, `F_GETLK`: per i lock

## Operare su directory

Queste system calls necessitano di `#include <dirent.h>` e di `#include <sys/types.h>`.

Una entry di una directory è rappresentata dalla struttura `dirent`:

```c
struct dirent{
    ino_t d_ino; // Inode number del file
    off_t d_off; // vedi man
    unsigned short d_reclen; // vedi man
    unsigned char d_type; // vedi man
    char d_name[256]; // Filename (una stringa)
};
```

### `*opendir()`

Apre la directory e restituisce un puntatore predisposto a scorrere la lista delle entry.

`DIR *opendir(const char *name)`

- `const char *name`: pathname della directory

### `dirent()`

Restituisce una entry della directory e sposta il puntatore in avanti.

`struct dirent *readdir(DIR *dirpath)`

- `DIR *dirpath`: path della directory

### `rewinddir()`

Riposiziona il puntatore all'inizio della directory.

`void rewinddir(DIR *dirp)`

### `closedir()`

Chiude la directory.

`void closedir(DIR *dirp)`

# Pipes

Per utilizzare le _pipes_ è necessario utilizzare `#include <unistd.h>`

Una pipe è rappresentata da un array di due interi che contengono i descrittori di file per la lettura e la scrittura.

`int pipe(pipefd[2])`

- `pipefd[0]`: utilizzabile per leggere dalla pipe
- `pipefd[1]`: utilizzabile per scrivere sulla pipe

La politica è di tipo FIFO.

## Operazioni con le pipes

Una volta creata la pipe, gli estremi sono utilizzabili come se fossero i file descriptor di file normali. Eredita quindi le system call `read()`, `write()` e `close()`.

# IPC: Inter Process Communication

Vi sono tre principali metodi di comunicazione fra processi: **semafori**, **memoria condivisa** e **code di messaggi**. Le system call per le operazioni che si possono svolgere tramite questi metodi sono simili.

|           | semafori      | memoria condivisa   | code di messaggi      |
| --------- | ------------- | ------------------- | --------------------- |
| header    | `<sys/sem.h>` | `<sys/shm.h`        | `<sys/msg.h`          |
| creazione | `semget()`    | `shmget()`          | `msgget()`            |
| controllo | `semctl()`    | `shmctl()`          | `msgctl()`            |
| comandi   | `semop()`     | `shmat()` `shmdt()` | `msgsnd()` `msgrcv()` |

## Key

Ogni risorsa è identificata dal proprio _identifier_ che è generato runtime tramite una _key_ associata alla risorsa.

### `ftok()`

Genera una key a partire da un pathname.

`key_t ftok(const char *pathname, int proj_id)`

- `const char *pathname`: pathname della risorsa
- `int proj_id`: 

## Semafori

Semafori che regolano l'accesso a risorse.

### `int semget ()` 
Richiede al S.O. la creazione di un vettore di semafori.

`int semget (key_t miachiave, int numeroSem, int flagPerm)`

- `key_t miachiave`:
- `int numeroSem`: lunghezza del vettore contenente semafori 
- `int flagPerm`: intero, sequenza di bit che funge da flag:
  - `IPC_CREAT`: il semaforo deve essere creato nel caso non esista già
  - `IPC_EXCL`:

I semafori creati sono generici e ognuno di questi possiede le informazioni di:

- `semval`: valore del semaforo
- `sempid`: PID dell'ultimo processo che ha operato sul semaforo
- `semncnt`: numero di processi in attesa che il valore venga incrementato
- `semzcnt`: numero di processi in attesa che il valore divenga zero

### `int semctl()`

Esegue l'inizializzazione di un semaforo.

`int semctl(int semid, int num, int cmd, union semun args)`

- `int semid`: l'identificativo del vettore dei semafori
- `int num`: l'indice del semaforo da inizializzare all'interno del vettore dei semafori 
- `int cmd`: codice che specifica quale operazione eseguire
- `union semun args`: un tipo di dato ce fornisce ulteriori informazioni
    ```c
    union semun{
        int val;
        struct semid_ds *buf;
        unisgned short *array;
    }
    ```

Alcuni valori per `cmd`:
- `IPC_STAT`: memorizza le informazione sullo stato in `args.buf`
- `IPC_SET`: imposta informazioni (owner, permessi, ...) copiandoli da `args.buf`
- `IPC_RMID`: elimina l’intero vettore di semafo
- `GETVAL`: restituisce `semval`
- `SETVAL`: imposta `semval` al valore di `args.val`
- `GETPID`: restituisce `sempid`
- `GETNCNT`: restituisce `semncnt`
- `GETZCNT`: restituisce `semzcnt`

### `semop()`

Permette di eseguire in modo atomico un intero insieme di operazioni sui semafori di un vettore.

`int semop(int semid, struct sembuf *sops, unisgned nsops)`

- `int semid`: l'identificativo del vettore dei semafori
- `struct sembuf *sops`: una struttura che ha i campi:
  - `unsigned short sem_num`: indice del semaforo
  - `short` sem_op: codice dell'operazione da eseguire
  - `short` sem_flag: flag che possono modificare l'operazione
- `unisgned nsops`: lunghezza dell'array `*sops`, indica quante operazioni fare

## Memoria condivisa

Memoria condivisa fra processi.

### `shmget()`

Crea un blocco di memoria.

`int shmeget(key_t, size_t size, int shmflg)`

- `size_t size`: dimensione del blocco di memoria
- `int shmflg`: flag per i permessi

### `*shmat()`

Attaca il blocco di memoria ad una variabile.

`int *shmat(int shmid, const void *shmaddr, int shmflg)`

- `int shmid`: identificatore del blocco di memoria, 
- `const void *shmaddr`: indirizzo a cui verrà fatto corrispondere il blocco di memoria
- `int shmflg`: flag

### `shmdt()`

Stacca la memoria dallo spazio locale.

`int shmddt(const void *shaddr)`

- `const void *shaddr`: puntatore che fa riferimento al blocco di mmeoria da staccare

### `shmctl()`

Modifica informazioni relative alla memoria condivisa.

`int shmctl(int shmid, int cmd, struct shmid_ds *buf)`

- `int shmid`: id memoria
- `int cmd`: comando da eseguire
- `struct shmid_ds *buf`: 

La distruzione della memoria condivisa avviene attraverso il comando `IPC_RMID`: `shmctl(shmid, IPC_RMID, 0)`.

## Code di messaggi

Un messaggio è una qualsiasi sequenza di caratteri i di byte.

### `msgget()`

Crea o apre una coda. Il valore ritornato fa le veci dell'identifier relativo alla coda.

`int msgget(key_t key, int msgflg)`

-  `key_t key`: chiave
-  `int msgflg`: flag

Ad ogni coda viene associata una struct `msquid_ds`: 

```c
struct msquid_ds {
    struct ipc_perm msg_perm;/* Ownership, permissions */
    time_t msg_stime; /* Time of last msgsnd() */
    time_t msg_rtime; /* Time of last msgrcv() */
    time_t msg_ctime; /* Time of last change */
    unsigned long __msg_cbytes; /* Current number of bytes in
                                    queue (non-standard) */
    msgqnum_t msg_qnum; /* Current number of msg in queue */
    msglen_t msg_qbytes; /* Max number of bytes
                            allowed in queue */
    pid_t msg_lspid; /* PID of last msgsnd() */
    pid_t msg_lrpid; /* PID of last msgrcv() */
}
```

### `msgsnd()`

Invia un messaggio inserendolo nella coda.

`retval= msgsnd(msg_quid, &message, size, flags)`

- `msg_quid`: id della coda
- `message`: 
    ```c
    struct my_msg {
        long mtype;
        char mtext[MIOVALORE];
    } message;
    ```

### `msgrcv()`

Riceve un messaggio tramite la coda ed elimina il messaggio dalla coda

`retval= msrgrcv(msg_qid, &message, size, msg_type, flags)`

# Segnali

I **segnali** sono meccanismi per inviare interrupt software ai processi, sono un tipo di _comunicazione asincrona_.

Possono essere **inviati** da un processo a uno o a più processi. Quando vengono **ricevuti** possono essere _gestiti_ (tramite azione di default o azioni specifiche), _ignorati_ (non sempre è possibile) oppure _bloccati_ (il segnale resta pendente). La gestione del segnale avviene per mezzo di un _handler di segnale_.

I segnali sono definiti nell'header `signal.h`.

Spesso l'azione di default consiste nella teminazione del processo ricevente. Questo avviene con i segnali `SIGABRT`, `SIGBUS`, `SIGSEGV`, `SIGQUIT`, `SIGILL`, `SIGTRAP` e altri che generano un file _core image_. I segnali di `SIGINT`, `SIGKILL`, `SIGUSR1` provocano la terminazione senza generare il file core.

Il segnale `SIGSTOP` blocca il processo ricevente che passa in stato di _suspended_. Per riavviarlo si usa `SIGCONT` che porta il processo dallo stato di _suspended_ allo stato di _running_.

## Approfondimento su `wait()`

Quando un processo termina e diventa _terminated_, il suo valore di ritorno viene scritto nella _process table_ e un segnale `SIGCHLD` viene inviato al padre. Quando quest'ultimo esegue la `wait()` il suo comportamento dipende dalla presenza e dallo stato dei figli. Se non ci sono figli `wait()` restituisce un codice d'errore. Se almeno un figlio è _terminated_ uno di essi viene eliminato dalla process table restituendo PID e return value. Se esistono figli, ma nessuno è _terminated_ il processo chiamante viene messo in attesa.

Di default l'azione di fronte ad un `SIGCHLD` è ignore: i figli restano _terminated_ fino alla `wait()` o alla terminazione del padre. Se il processo decide attivamente di ignorare `SIGCHLD`, il kernel elimina il processo _terminated_ appena riceve il segnale.

Il valore di ritorno della `wait()` viene interpretato in base ai byte che lo compongono:

- Se il _rightmost_ byte è 0, il _leftmost_ byte (8 bit) contiene il valore di ritorno del figlio.

- Se il _rightmost_ byte è 1, i 7 bit meno significativi contengono il codice del segnale di terminazione, mentre il bit rimanenti indicano la generazione di _core dump_.

## Approfondimento su `waitpid()`

Un processo _suspended_ viene riattivato quando il figlio dal PID specificato termina. Utilizzando speciali flag è possibile modificare questo comportamento causando il risveglio del padre ad un cambio di stato del figlio.

- `WUNTRACED`: riattiva il padre se il figlio passa allo stato _suspended_
- `WCONTINUED`: riattiva il padre se il figlio lascia lo stato _suspended_
- `WNOHANG`: riattiva il padre anche se nessun figlio è terminato

## Invio di segnali da linea di comando

- `ctrl+c`: invia un segnale di `SIGINT` ovvero un interrupt da tastiera
- `ctrl+z`: invia un segnale di `SIGTSTP` ovvero un segnale di stop

### `kill()`

Invia un segnale da programma.

`int kill(pid_t pid, int sig)`

- `pid_t pid`: il programma a cui inviare il segnale
  - $n$: il segnale viene inviato al processo $n$
  - $0$: il segnale viene inviato a tutti i processi del gruppo del chiamante
  - $-1$: il segnale viene mandato a tutti i processi con $PID < 1$
  - $-n$: il segnale viene mandato a tutti i processi del gruppo $n$

## Ignorare `SIGCHLD`

Per gestire un segnale un processo può optare per tre opzioni: la scelta di default, getire attivamente o ignorare il segnale.

La scelta di default permette al figlio di rimanere in stato _terminated_ fino all'esecuzione della `wait()`. Se il segnale viene attivamente ignorato tramite `SIG_IGN` il processo terminated viene subitO rimosso. La flag `SA_NOCLDWAIT` ha un effetto simile, ma viene impostata tramite `sigaction()` ed evita che i processi figli diventino zombie, senza la necessità di gestire esplicitamente `SIGCHLD`.

## Gestion dei segnali

### `sigaction()`

Gestisce un segnale.

`int sigaction(int signo, const struct sigaction *act, struct sigaction *oact)`

- `int signo`: il segnale che si vuole gestire
- `const struct sigaction *act`: puntatore a struttura che specifica la gestione del segnale. 
- `struct sigaction *oact`: struttura in cui vengono salvati i valori correnti

La struttura `sigaction`

```c
struct sigaction{
    void (*sa_handler)(int); /*azione da compiere*/
    sigest_t sa_mask; /*altri segnali da bloccare*/
    int sa_flags; /*flag che influiscono sul segnale*/
    /*handler alternativo se flags vale SA_SIGINFO*/
    void (*sa_sigaction)(int, sigingfo_t void *);   
}
```

`sa_handler` è una funzione solitamnte definita dall'utente che ha prototipo `void miagestione(int signo)`.

La registrazione dell'handler avviene con l'assegnamento `act.sa_handler = miagestione`.

Per mascherare altri segnali si può usare `sigfillset(&(act.sa_mask))` che maschera tutti i segnali possibili durante l'esecuzione di `miagestione`.

La funzione `miagestione` può essere sostituita con `SIG_DFL`  o `SIG_IGN`.

Un segnale bloccato resta _peding_, tutti i segnali mascherati durante l'esecuzione di un handler vengono bloccati e se vi sono più segnali _pending_ dello stesso tipo, solo uno viene mentenuto.

Con la chiamata `sigaction()` possono essere installati gli ulteriori handler per gestire un `SIGCHLD`: 

- `SA_NOCLDWAIT`: i figli terminati non permangono come _terminated_
- `SA_NOCLDSTOP`: notifica il passaggio a stato stopped dei figli

Non tutti i segnali possono essere ignorati, gestiti o bloccati: `SIGKILL`, `SIGCONT`.

# Socket

Esistono due principali modi per comunicare tramite socket: _connection oriented model_ e _connectionless oriented model_.

Per comunicare con questi due paradigmi è possibile usare:

- **Stream socket**: forniscono stream di dati affidabili, ordinati. Sono supportati da TCP.
- **Stream datagram**: trasferiscono messaggi di dimensione variabile, il contenuto è controllato da checksum, ma non viene garantito ne l'ordine ne l'arrivo dei pacchetti. Sono supportati da UDP.

## Indirizzi IP

Le stringhe degli indirizzi IP devono essere convertite nel tipo apposito.

### `inet_aton()`

Richiede `#include <sys/socket.h>`, `#include <netinet/in.h>`, `#include <arpa/inet.h>`

`inet_aton(const char *ip_addr, struct in_addr *inp)`

- `const char *ip_addr`: indirizzo stringa da convertire
- `struct in_addr *inp`: struttura adibita a memorizzare IP. Ha un campo `sin_ip` apposito

## Porte

Oltre a conoscere l'indirizzo IP dell'host bisogna disporre dell'informazione sufficiente per collegarsi al processo corretto sul server. Il numero di porta sopperisce a questa mancanza.

Le porte da 0 1023 sono riservate a servizi standard, mentre dalla 1024 alla 65535 sono disponibili per altri processi.

## Strutture dati

Un socket generico è definito come:

```c
struct sockaddr {
    short sa_family; /*address family*/
    char sa_data[]; /*address data*/
}
```

Un socket `UNIX` è definito come:

```c
#DEFINE UNIX_PATH_MAX 108
struct sockaddr_un {
    sa_family_t sun_family; /*costante AF_UNIX*/
    char sun_path[UNIX_PATH_MAX]; /*path name*/
}
```

Un socket nel dominio `INET` è definito come:

```c
struct sockaddr_in {
    sa_family_t sin_family; /* Costante AF_INET */
    u_int16_t sin_port; /* Porta (unsigned short) */
    struct in_addr sin_addr; /* indir. IP */
};
```

in cui

```c
struct in_addr {
    u_int32_t s_addr; /* ulong */
};
```

## System call

Per comunicare attraverso un socket, due processi devono compiere una serie di passi. Sia server che client devono dapprima definire i rispettivi **transport end point** (`socket()`). Il server deve legare l’end point all’indirizzo dell’host (`bind()`), impostarlo in uno stato di _ascolto passivo_ ed alloca le strutture per gestire una coda di client (`listen()`), poi il server si predispone per accettare la connessione (`accept()`). A questo punto il client richiede la connessione (`connect()`) e questa procede bidirezionalmente tramite `send()` e `recv()` (maanche `write()` e `read()`). La chiusura della socket avviene tramite `close()` o `unlink()`.

### `socket()`

Definisce una socket.

`socket(int domain, int type, int protocol)`

- `int domain`: indica il dominio del socket (`AF_INET`,  `AF_UNIX`, ...)
- `int type`: indica se verra usato il paradigma connection oriented `SOCK_STREAM`  o quello connectionless `SOCK_DGRAM`
- `int protocol`: specifica il protocollo

Il valore di ritorno è un descrittore della socket.

### `bind()`

Associa l'endpoint all'indirizzo

`int bind (int sockfd, const struct sockaddr *address, size_t add_len)`

- `int sockfd`: descrittore della socket
- `const struct sockaddr *address`: struct che descrive l'indirizzo 
- `size_t add_len`: la dimensione della struct

### `listen()`

Il server imposta la socket in modo che possa ascoltare le connessioni in entrata.

`int listen(int sockfd, int queue_size)`

- `int sockfd`: descriptor della socket
- `int queue_size`: lunghezza massima della coda di clienti consentita

### `accept()`

Il server usa `accept()` per accettare connessioni in entrata.

`int accept(int sockfd, struct sockaddr *client_address, size_t *add_len)`

- `int sockfd`: descriptor della socket 
- `struct sockaddr *client_address`: 
- `size_t *add_len`:

### `connect()`

Il cliente si connette al server in corrispondenza della socket indicata.

`int connect (int csockfd, const struct sockaddr *server_address, size_t add_len)`

- `int csockfd`: descriptor della socket di connessione al server
- `const struct sockaddr *server_address`: 
- `size_t add_len`:

### `send()`

Serve per inviare messaggi tramite la socket.

`ssize_t send(int sockfd, const void *buffer, size_t length, int flags)`

- `const void *buffer`: buffer contenente dati da inviare
- `size_t length`: lunghezza buffer
- `int flags`: flag che servono per modificare le modalità di spedizione. Se vale 0 equivale a `write()`

### `recv()`

`ssize_t recv(int sockfd, const coid *buffer, size_t length, int flags)`

- `const void *buffer`: buffer per ricevere i dati
- `size_t length`: lunghezza buffer
- `int flags`: flag che servono per modificare le modalità di spedizione. Se vale 0 equivale a `read()`

# POSIX threads

POSIX è una libreria importabile da C che permette al programmatore di gestire a basso livello i threads.

Le principali funzioni sono: 

- `pthread_create()` : creazione di un pthread
- `pthread_exit()` : terminazione del pthread
- `pthread_cancel()` : richiesta terminazione di un pthread
- `pthread_join()` : attesa terminazione di un pthread
- `pthread_detach()` : detached un pthread
- `pthread_self()` : accesso al thread ID
- `pthread_equal()` : controllo uguaglianza tra thread IDs

Necessitano di `#include <pthread.h>` e dell'opzione `-pthread`

## Operazioni sui pthread


### `pthread_create()`

Crea un nuovo thread fratello del chiamante.

`int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)`

- `const pthread_attr_t *attr`: lista di attributi per il pthread
- `*start_routine` è un puntatore a funzione che indica il codice che il pthread eseguirà

### `pthread_detach()`

Un thread rilascia le risorse quando termina e diventa _detached_. In caso di successo restituisce 0. 

`int pthread_detach(pthread thread)`

- `pthread thread`: il TID del thread che si vuole rendere detached

### `pthread_join()`

Quando un thread non è detached è detto _joinable_ e quando termina non rilascia le risorse finchè non avviene una `join()` con un altro thread. In caso di successo restituisce 0.

`int pthread_detach(pthread thread void **retval)`

### `pthread_exit()`

Termina l'esecuzione di un thread.

`void pthread_exit(void *retval)`

### `pthread_cance()`

Un thread può chiedere la terminazine di un altro thread specificandone il TID.

`int pthread_cancel(pthread_t thread)`

Usando i cambi di stato un pthread può, per esempio, ritardare l’effetto di pthread_cancel() per evitare di essere cancellato nel mezzo di un blocco di codice critico che non deve essere eseguito parzialmente.

### `pthresad_setcanceltype()`

Cambia il tipo di cancellazione del pthread.

`int pthread_setcanceltype(int type, int *oldtype)`

Un pthread può controllare i punti del codice in cui ha effetto la sua richiesta di cancellazione tramite i tipi `PTHREAD_CANCEL_ASYNCHRONOUS` che permette al pthread di reagire alla cancellazione in qualsiasi istante, o `PTHREAD_CANCEL_DEFERRED`, scelta di default che permette di cancellare il pthread solo in specifici punti del codice detti _cancellation points_.

### `pthread_testcancel()`

Crea un cancellation point esplicito.

`void pthread_testcancel(void)`

Molti cancellation points sono impliciti.

### `pthread_attr_init()`

Specifica un _attributes object_ che modifica alcune proprietà del pthread. Questa funzione lo inizializza con i valori di default.

`int pthread_attr_init(pthread_attr_t *attr)`

- `pthread_attr_t *attr`: _attribute object_

Prima di usare `*attr` con `pthread_create()` è possibile modificarne gli attributi con le apposite funzioni.

## Mutua esclusione

I pthreads di un processo condividono le risorse allocate al processo, quindi è necessario avere dei meccanismi che ne regolino l'accesso o che eseguano sincronizzazione.

I **mutex lock** garantiscono mutua esclusione grazie a variabili speciali che possono essere in stato di _locked_ o _unlocked_: se un mutex è locked, allora esite un thread che ne detiene il lock. Ogni mutex ha una coda.

Le funzioni che operano sui mutex non sono interrompibili da segnali e non sono cancellation point.

I mutex sono progettati per gestire sezioni critiche di breve durata.

Un mutex lock è una variabile di tipo `pthread_mutex_t` che può essere allocata staticamente o dinamicamente. Nel caso dell'allocazione statica il valore a cui inizializzarlo è `PTHREAD_MUTEX_INITIALIZER`.

```c 
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER
```

Nel caso di allocazione dinamica si deve usaere la funzione `pthread_mutex_init()`.

### `pthread_mutex_init()`

Inizializza un mutex per i pthread.

`int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t  *restrict attr)`

- `pthread_mutex_t *restrict mutex`: puntatore a mutex lock da inizializzare
- `const pthread_mutexattr_t  *restrict attr`: attributi, se `NULL` usa default

### `pthread_mutex_destroy()`

Dealloca un mutex non più utiliizzato.

`int pthread_mutex_destroy(pthread_mutex_t *mutex)`

### `pthread_mutex_lock()`

Chiamata bloccante. Se il mutex è in stato di lock, il pthread chiamante viene messo in coda di attesa.

`pthread_mutex_lock(pthread_mutex_t *mutex)`

### `pthread_mutex_trylock()`

Se il mutex è in stato di lock, restituisce un codice d'errore `EBUSY`.

`pthread_mutex_trylock(pthread_mutex_t *mutex)`

### `pthread_mutex_unlock()`

Rilascia il lock.

`pthread_mutex_unlock(pthread_mutex_t *mutex)`

## Sincronizzazione nei pthread

Le _condition variables_ sono utilizzate nello standard POSIX per implementare la sincronizzazione fra pthread. Una condition variable permete ad un pthread di sospendere l'esecuzione fino al verificarsi di un evento o di una condizione. Quando è in questo stato il pthread si trova in una coda associata alla condition variable. Quando l'evento o la condizione si verifica il thread viene risveglaito. Ad ogni risveglio il pthread deve verificare se la condizione si sia veramente verificata o se sia il caso di riaccodarsi. 

L'uso di una condition variable deve essere associato all'uso di un mutex lock.

Una condition variable è di tipo `pthread_cond_t` può essere inizializzata staticamente tramite il valore `PTHREAD_COND_INITIALIZER` oppure dinamicamnte tramite la funzione `pthread_cond_init()`.

### `pthread_cond_init()`

Inizializza dinamicamnte una condition variable.

`pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr)`

- `pthread_cond_t *restrict cond`: puntatore alla condition da analizzare
- `const pthread_condattr_t *restrict attr`: attributi

### `pthread_cond_destroy()`

Distrugge la condition.

`pthread_cond_init(pthread_cond_t *cond)`

### `pthread_cond_wait()`

Attesa del pthread sulla condition tramite il mutex associato.

`int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex)`

- `pthread_cond_t *restrict cond`: puntatore alla condition
- `pthread_mutex_t *restrict mutex`: puntatore al mutex che protege la condition. Il lock deve essere stato preventivamente acquisito prima di usare `pthread_cond_wait()`.

La chiamata causa in modo atomico la sospensione del pthread (viene inserito in una coda associata a `cond`) e il rilascio del lock sul mutex.

Al risveglio il pthread riacquisisce il lock sul mutex (o aspetta di poterlo fare). Successivamente deve rilasciarlo in maniera esplicita.

### `pthread_cond_broadcast()`

Risveglia tutti i pthread in attesa su una condizione.

`pthread_cond_broadcast(pthread_cond_t *cond)`

### `pthread_cond_signal()`

Risveglia almeno un pthread in attesa sulla condizione.

`pthread_cond_signal(pthread_cond_t *cond)`

## Esempio schematico wait e signal su condition variable

Tipicamente un pthread, operando in mutua esclusione (tramite un mutex lock), verifica se una condizione sia vera, nel caso non lo sia si mette in attesa su una condition variable. 

Quando un altro pthread lo risveglia (perchè per esempio ha causato la potenziale verità della condizione) il pthread riacquisisce il lock e rivaluta (in mutua esclusione) la condizione per avere la garanzia della sua verità.

Se il test fallisce il tutto si ripete, altrimenti il pthread procede.
