\newpage

# Sincronizzazione

## Il problema della sezione critica

### Processi concorrenti

Un S.O. deve gestire più processi contemporaneamente, questi possono eseguire in maniera _logicamente indipendente_ o _cooperante_. In generale due processi compiono le loro azioni inframezzandole e utilizzando risorse condivise, tali processi necessitano di meccanismi di _sincronizzazione_, _comunicazione_ e _accesso a dati/informazioni condivise_.

L'_accesso contemporaneo_ ai dati condivisi può essere problematico in quanto i dati possono essere modificati in maniera incoerente dai processi. Il S.O. deve implementare metodi che garantiscano la consistenza dei dati permettendo ad un solo processo per volta di accedere a dati condivisi. Questo deve avvenire garantendo un tempo di attesa ragionevole, con una politica che non penalizzi nessun processo e senza la creazione di attese infinite.

- **Sezione critica**: sezione di codice che legge/scrive dati condivisi.
- **Race condition**: situazione in cui due o più processi eseguono concorrentemente sulla sezione critica. Il risultato dell'esecuzione dipende dall'ordine in cui i processi si alternano e perciò è fonte di inconsistenza.
- **Mutua esclusione**: meccanismo che evita le race condition assicurando l'accesso esclusivo ai dati ad un solo processo alla volta.
- **Deadlock**: situazione in cui due o più processi sono bloccati a causa di un attesa reciproca.
- **Starvation**: situazione in cui ad un processo che ha richiesto la sezione critica non viene mai dato il permesso di proseguire nell'esecuzione.

### Esempio

Due processi, uno detto _produttore_ e uno detto _consumatore_ hanno in condivisione un buffer

``` c
#define BUFFER_SIZE 10
typedef struct {
. . .
} item;
// dimensione del buffer
item buffer[BUFFER_SIZE]; // array circolare
int in = 0;
 // posizione in cui scrivere
int out = 0;
 // posizione da cui leggere
int counter = 0;
 // num. di record memorizzati
```

Produttore:

``` c
item nextProduced;
while (1) {
 // loop forever
while (counter == BUFFER_SIZE)
; /* non fa niente (busy waiting!!!) */
buffer[in] = nextProduced;
in = (in+1) % BUFFER_SIZE;
counter++;
}

```

Consumatore:

``` c
item nextConsumed;
while (1) {
 // loop forever
while (counter == 0)
; /* non fa niente (busy waiting!!!) */
nextConsumed = buffer[out];
out = (out+1) % BUFFER_SIZE;
counter--;
}
```

Presi singolarmente i processi sono corretti, ma in caso di _esecuzione concorrente_, la gestione della variabile ```counter``` risulta problematica: l'istruzione ```counter++``` (e rispettivamente ```counter--```) è una _sezione critica_ che a livello assembly viene eseguita come un insieme di più operazioni basilari in successione. Se avviene un interrupt quando solo parte delle istruzioni sono state eseguite si possono generare inconsistenze. Una situazione del genere è detta _race condition_.

### Soluzione generale al problema della sezione critica

Una soluzione a questo problema deve garantire:

- **Mutua esclusione**: un solo processo alla volta esegue la sezione critica.
- **Progresso**: la decisione su chi esegue in sezione critica deve avvenire entro un certo limite di tempo, senza interferenze da eventuali processi già all'interno della sezione critica.
- **Attesa limitata**: non deve esserci starvation per processi che chiedono di entrare in sezione critica.

Quindi si ha che:  

- Nessun processo esterno alla sezione critica può impedire ad un altro di entrarvi
- se un processo si blocca in sezione critica, non può interferire con l'esecuzione di altri processi
- ogni processo resta nella sezione critica solo per un tempo finito
- se nessun processo è presente nella sezione critica, allora un richiedente deve poterci entrare subito
- tutti i processi devono evolvere
- non bisogna fare assunzioni sul numero delle CPU

Bisogna quindi progettare un protocollo che, se adottato, garantisce che l'effetto globale delle azioni dei processi non dipende dall'ordine in cui queste si inframezzano. In generale questo prevede che la sezione critica sia racchiusa fra due porzioni di codice dette **sezione d'ingresso** e **sezione d'uscita** che permettono rispettivamente di entrare e uscire da essa.

``` pseudo
    sezione non critica
    sezione d'ingresso
    sezione critica
    sezione d'uscita
    sezione non critica
```

Una soluzione ideale deve essere in grado di funzionare per qualsiasi insieme $\langle P_{1} \dots P_{n} \rangle$ di processi.

#### Primo tentativo di soluzione

Questo tentativo di soluzione utilizza una variabile `turno` per indicare chi ha diritto d'ingresso nella sezione critica. Il codice di un processo $P_{i}$ è il seguente:

``` C
int turno = 0; /* turno=i indica ok per Pi */
do {
    ....sezione non critica...
    while (turno != i)
    ; // attesa del proprio turno
    ....sezione critica...
    turno = j; // permetto all’altro di entrare
    ....sezione non critica...
} while (1);

```

Pur garantendo _mutua esclusione_, non garantisce _progresso_ e _attesa limitata_: ad un processo $P_{i}$ viene conferita l'autorizzazione ad entrare in sezione critica solo da un altro processo $P_{j}$ quando quest'ultimo esce dalla sezione critica. Se $P_{j}$ si blocca o non necessita di entrare, allora il primo processo $P_{i}$ resta bloccato all'esterno della sezione critica. 

Questo tipo di implementazione non prevede che un processo entri in sezione critica due volte consecutivamente.

#### Secondo tentativo di soluzione

I processi utilizzano due flag per indicare il loro interesse ad entrare in sezione critica. Il codice di un processo $P_{i}$ è il seguente:

``` C
boolean flag[2];
/* 
flag[i]=true indica che Pi vuole entrare
inizialmente flag[0]=flag[1]=false
*/
do {
    ....sezione non critica...
    flag[i] = true; // Pi dichiara il suo interesse
    while (flag[j])
    ; // attesa del proprio turno controllando l'interesse di Pj
        ....sezione critica...
        flag[i] = false; // dichiara la fine dell’interesse
        ....sezione non critica...
} while (1);
```

Il problema di questa implementazione risiede nel fatto che il funzionamento dipende dalla velocità reciproca dei processi. Potrebbe accadere che `flag[0]=flag[1]=True`, in questo caso si presenta _deadlock_.

#### Soluzione di Peterson

Questa soluzione utilizza sia una variable `turno` che due flag per dichiarare interesse a entrare in sezione critica. Il codice di un processo $P_{i}$ è il seguente:

``` C
boolean flag[2];
int turno;
do {
    ....sezione non critica...
    flag[i] = true; // Pi dichiara il suo interesse
    turno = j; // assume che sia il turno dell’altro e gli offre l’occasione di entrare
    while (flag[j] && turno==j)
    ; // attesa del proprio turno
    ....sezione critica...
    flag[i] = false; // fine dell’interesse
    ....sezione non critica...
} while (1);
```

La variabile `turno` garantisce la _mutua esclusione_, elimina la possibilità di _deadlock_ ed evita che un processo monopolizzi la sezione critica. 

L'uso dei flag evita che un processo venga chiuso fuori dalla sezione critica nel caso un altro processo non gli dia l'autorizzazione, permettendo così ai processi di entrare in sezione critica più volte consecutivamente.

Questa soluzione non è ottimale in quanto è presente _busy waiting_, prevede che gli aggiornamenti di `turno` e `flag[i]` avvengano con istruzioni atomiche, inoltre è difficile da scalare nel caso dovessero esserci più programmi. Un altro problema risiede nel fatto che in processi multithread il compilatore potrebbe applicare delle ottimizzazioni che riordinano le istruzioni macchina.

#### Soluzioni basate sulle interruzioni

Questo tipo di soluzioni prevedono la disabilitazione delle interruzioni durante l'esecuzione della sezione critica per evitare preemption:

``` pseudo
    sezione non critica
    disabilita interruzioni
    sezione critica
    abilita interruzioni
    sezione non critica
```

Nei sistemi mono processore garantisce mutua _esclusione_ e _atomicità_, ma gli switch di contesto inficiano l'efficienza. 

Nei sistemi multi processore la _mutua esclusione_ non viene garantita in quanto bisognerebbe bloccare/allertare tutti gli altri processori. La sezione critica resta _atomica_ (le operazioni vengono eseguite come un'unica unità indivisibile), ma non _esclusiva_.

Vi è la possibilità che si verifichino _deadlock_ a causa di operazioni di I/O eseguite durante una sezione critica.

#### Soluzioni con supporto hardware

Particolari istruzioni macchina possono essere utilizzate per disciplinare accessi concorrenti alla memoria (es: barriere di memoria) e per eseguire in modo unitario  due o più azioni sulla stessa locazione di memoria (test+assegnamento, read+modifica+write, ...).

##### Barriere di memoria

Sono istruzioni che regolano il load/store della memoria. Particolarmente utili in sistemi multiprocessore.

``` C
//Th1:
while (!flag)
    memory_barrier();
y = x;
```

``` C
//Th2:
x = 100;
memory_barrier();
flag = true;
```

In questo esempio $Th_{1}$ legge `flag` prima di `x`, mentre la modifica di `x` fatta da $Th_{2}$ sarà resa disponibile a $Th_{1}$ prima che $Th_{2}$ modifichi `flag`.

##### Test and set

Istruzione atomica che legge il valore di una variabile (`target`), lo restituisce e contemporaneamente la imposta a TRUE. Garantisce che nessun altro processo possa modificare `target` durante l'operazione. Garantisce _mutua esclusione_.

La sua implementazione è:

``` C
boolean TestAndSet (boolean *target){
    boolean retval = *target;
    *target = TRUE;
    return *retval;
} 
```

Esempio di utilizzo:

``` C
boolean lock;/* condivisa e inizializzata a false */
do {
    ....sezione non critica...
    while (TestAndSet(&lock))
    ;// attesa del proprio turno
    ....sezione critica...
    lock = false;
    // uscita dalla SC
    ....sezione non critica...
} while (true);
```

Soffre di _busy waiting_ e non garantisce _attesa limitata_.

##### Swap

Istruzione atomica che scambia i valori di due variabili booleane (`a` e `b`). Durante l'operazione, nessun altro processo può intervenire garantendo così consistenza.

La sua implementazione è:

``` C
void Swap (boolean *a, boolean *b){
    boolean temp = *a;
    *a = *b;
    *b = temp;
}
```

Esempio di utilizzo:

``` C
boolean lock; // condivisa e inizializzata a false
/* lock==true significa porta chiusa */
boolean chiave; // privata e locale al processo
do {
    ....sezione non critica...
    chiave = true; // "richiesta della chiave"
    while (chiave == true) // attesa del turno, quando
    Swap(&lock, &chiave); // entra esclude gli altri,
    ....sezione critica... // qui chiave e’ false
    lock = false;
    // uscita dalla SC
    ....sezione non critica...
} while (1);
```

L'unico processo che può entrare è colui che, dopo aver richiesto la chiave, trova `lock==false`.
Soffre di _busy waiting_ e non garantisce _attesa limitata_.

##### Compare and swap

Istruzione atomica che confronta il valore di una variabile (`val`) con un valore atteso (`test`). Se coincidono, aggiorna la variabile con un nuovo valore (`newval`). In ogni caso, restituisce il valore originale di `val`. Serve a garantire la _sincronizzazione_ tra processi.

La sua implementazione è:

``` C
int CompareAndSwap (int *val, int test, int newval){
    int temp = *val;
    if (*val == test)
        *val = newval;
    return temp;
}
```

Esempio di utilizzo:

``` C
int lock; //condivisa e iniz. a 0 (cioe‘ "aperto")
boolean waiting[N]; // array condiviso iniz. a false

    /* processo i-esimo */
while (true) {
    ....sezione non critica...
    waiting[i] = true; 
    key = 1;
    
    while (waiting[i] && key == 1) // attesa turno
        key = CompareAndSwap(&lock, 0, 1);
    
    waiting[i] = false;
    ....sezione critica...
    /* ricerca del prossimo processo in attesa di entrare*/
    j = (i+1)%N;
    while ((j!=i) && !waiting[j])
        j = (j+1)%N;
    if (j == i) lock = 0; else waiting[j] = false;
....sezione non critica...
}
```

Garantisce _mutua esclusione_, _progresso_ e _attesa limitata_, ma è ancora presente _busy waiting_.

#### Variabili atomiche

Queste istruzioni implementano operazioni aritmetiche e booleane in maniera atomica, così facendo è possibile eseguire codice in _mutua esclusione_ anche in sistemi multi processore. Possono essere utilizzate come base per costruire protocolli che assicurano _mutua esclusione_, _progresso_ e _attesa limitata_.

### Busy waiting

Una soluzione ottimale al problema della sezione critica dovrebbe evitare che i processi in attesa di entrare nella sezione critica occupino inutilmente la CPU. Per fare ciò è necessario che un processo in attesa cambi il suo stato in _waiting_ per non occupare la CPU. Questo processo può tornare _ready_ solo quando ha la possibilità di entrare nella sezione critica.

Uno strumento per raggiungere questo obiettivo è il **semaforo**.

## Semafori

Un semaforo è una struttura dati costituita da:

- una variabile interna `S`
- l'operazione `wait(S)` (detta anche `lock(S)`)
- l'operazione `signal(S)` (detta anche `unlock(S)`)

Le **uniche** operazioni con cui si può accedere al semaforo `S` sono `wait(S)` e `signal(S)`.

A seconda dei valori ammessi per `S` si distingue fra:

- semaforo binario: il valore può essere solo 1 o 0
- semaforo generico: il valore può essere un intero qualsiasi

### Wait e signal

La procedura **wait(S)** stoppa l'esecuzione del programma finché la variabile `S` non è inferiore o uguale a zero.

``` C
wait(S){
    while (S <= 0)
        ;
    S--;
}
```

Questa versione di `wait(S)` soffre comunque di busy waiting.

La procedura **signal(S)** aumenta di uno la variabile `S` per segnalare che il semaforo non è più occupato.

``` C
signal(S){
    S++;
}
```

Tramite queste due operazioni si può regolare l'accesso alla sezione critica:

``` pseudo
    sezione non critica
    wait(S)
    sezione critica
    signal(S)
    sezione non critica
```

### Tipologie di semaforo

Un semaforo **mutualmente esclusivo** (**mutex**) ha la variabile `S` inizializzata a 1 ed è utilizzato per regolare l'accesso alla sezione critica in maniera mutualmente esclusiva: un solo processo alla volta può entrare.

Un semaforo di **sincronizzazione** ha la variabile `S` inizializzata a 0 e serve a sincronizzare più processi: un processo $P_{1}$ che esegue la `wait(S)` si ferma ad aspettare la `signal(S)` di un processo $P_{2}$.

Un semaforo generico con `S` inizializzato a un valore $n > 1$ diventa un contatore generico in cui $n$ programmi hanno accesso alla sezione critica/risorsa in comune.

### Problema del busy waiting

Un semaforo implementato con busy waiting è detto _spinlock_. Sono vantaggiosi in caso di attese brevi in quanto non richiedono switch di contesto, ma se le attese diventano lunghe, eseguire uno switch di contesto diventa preferibile al mantenere _running_ un processo che utilizza tempo di CPU facendo nulla.

Per risolvere questo problema un processo che esegue una `wait(S)` quando `S` $\leq 0$ viene inserito in una coda di attesa al _semaforo_ e portato in stato di _waiting_. Il controllo passa allo _short term scheduler_ che permette ad uno dei processi in coda di passare allo stato di _ready_ quando viene eseguita una `signal(S)`.

La struttura del semaforo:

``` C
typedef struct{
    int S;  // valore del semaforo
    struct processo *lista; // coda di attesa    
} semaforo
```

Le due operazioni di _wait_ e _signal_ possono essere implementate sfruttando due system call solitamente offerte dal S.O.:

- `sleep()`: porta un processo in stato di _waiting_
- `wakeup(P)`: porta il processo `P` in stato di _ready_

Operazione `wait(S)` senza busy waiting:

``` C
wait (semaforo *S){
    S->valore --;
    if (S->valore < 0){
        aggiungi_a_lista(S->lista, processo_corrente())
        sleep();
    }
}
```

Operazione `signal(S)` senza busy waiting:

``` C
signal (semaforo *S){
    S->valore++;
    if (S->valore <= 0){
        processo *P= togli_un_processo_da_lista(S->lista);
        wakeup(&P);
    }
}
```

## Tipici problemi di sincronizzazione

Esempi di vari problemi tipici.

### Produttori e consumatori

- Uno o più processi **produttori** generano dati e li inseriscono in un buffer.
- Uno o più processi **consumatori** utilizzano dati prelevandoli da un buffer.
- Il buffer è condiviso e si deve accedere in mutua esclusione, inoltre ha capacità limitata ad $n$, per cui un produttore non può inserire un dato in un buffer pieno. Nessun consumatore può prelevare da un buffer vuoto.
- Ogni dato può essere prelevato una volta sola.

Una possibile soluzione prevede l'implementazione di 3 semafori: un mutex per la sincronizzazione e due generici per segnalare ai produttori e ai consumatori il numero di celle che possono utilizzare nell'array.

```C
semaphore piene; // numero di celle usate
semaphore vuote; // numero di celle libere
semaphore mutex; // per la mutua esclusione

// inizializzazione:
piene = 0;
vuote = MAX;
mutex = 1;
```

Il codice del produttore è il seguente:

```C
do {
    /*produzione del dato*/
    wait(vuote); // c’e‘ spazio libero?
    wait(mutex); // entro nella SC
    ...
    /*inserimento del dato nel buffer*/
    ...
    signal(mutex); // lascia la SC
    signal(piene); // segnala nuovo dato inserito
} while (1);
```

Prima di entrare nella sezione critica controlla che ci sia spazio libero nel buffer e dopo esserne uscito segnala ai consumatori, tramite il secondo semaforo, che nel buffer è stato inserito un nuovo dato.

Il codice del consumatore è il seguente:

```C
do {
    wait(piene); // ci sono dati?
    wait(mutex); // entro nella SC
    ...
    /*prelievo un dato dal buffer*/
    ...
    signal(mutex); // lascio la SC
    signal(vuote); // segnalo spazio libero
    /*utilizzo del dato*/
} while (1);
```

Il codice del consumatore è speculare a quello del produttore. Prima di entrare in sezione critica controlla che ci siano dati e dopo esserne uscito segnala ai produttori, tramite il secondo semaforo, che nel buffer si è liberato dello spazio.

### Lettori e scrittori

Un insieme di dati è condiviso da un certo numero di processi. Alcuni di questi devono leggere i dati, altri aggiornarli.

- Le richieste dei lettura e scrittura avvengono in modo asincrono e non prevedibile
- Più processi possono leggere sullo stesso dato, ma sono un processo alla volta può modificarlo. Inoltre, se un file è bloccato in scrittura, non può essere letto.

In questo caso la situazione è asimmetrica e sono presenti 2 varianti di soluzione:

- con **priorità ai lettori**: se non c’è una scrittura in atto nessun lettore deve attendere eventualmente sopravanzando sulle scritture
- con **priorità agli scrittori**: appena uno scrittore vuole aggiornare i dati lo deve poter fare eventualmente sopravanzando sulle letture

#### Priorità ai lettori

```C
int readcount = 0; // num. di lettori attivi
semaphore semW = 1, mutex = 1, extra = 1;
```

- Il semaforo `semW` permette l'accesso alla risorsa da parte di un scrittore 
- Il semaforo `mutex` blocca la variabile `readcount` per un lettore
- Il semaforo `extra` impedisce l'uso del semaforo `semW` da parte di più scrittori contemporaneamente.

Il codice del lettore:

```C
void lettore() {
    wait(mutex); // blocca la variabile readcount
    readcount++;
    if (readcount==1) wait(semW); // blocca eventuali scrittori
    signal(mutex);
    /*letture dati*/
    wait(mutex); // blocca la variabile readcount 
    readcount--;
    if (readcount==0) signal(semW); // sblocca eventuali scritori
    signal(mutex);
}
```

La variabile `readcount` può essere modificata da un solo lettore alla volta. La priorità ai lettori è data dal fatto che gli scrittori vengono subito bloccati se `readcount` sia 1 e possono venire sbloccati solo se `readcount` ridiventa 0, ovvero se tutti i lettori abbandonalo la zona critica.

Il codice dello scrittore:

```C
void scrittore() {
    wait(extra); // blocca altri scrittori
    wait(semW); // blocca altri scrittori ed eventualmente scrittori
    /*scrittura dati*/
    signal(semW);
    signal(extra);
}
```
Il semaforo 


#### Priorità agli scrittori

Vengono utilizzati 5 semafori e 2 variabili.

```C
semaphore W=1, R=1, mutex1=1, mutex2=1, mutex3=1;
int countR=0, countW=0;
```

- Il semaforo `W`: Permette l'accesso esclusivo alla scrittura. Solo uno scrittore alla volta può accedere ai dati. Viene bloccato quando un lettore o un altro scrittore è in azione.
- Il semaforo `R`: Serve per garantire che un lettore possa accedere solo se nessuno scrittore ha richiesto accesso. Blocca lettori quando c'è priorità di scrittura.
- Il semaforo `mutex1`: Protegge la variabile condivisa `countR` (conteggio dei lettori attivi).
- Il semaforo `mutex2`: Protegge la variabile condivisa `countW` (conteggio degli scrittori attivi).
- Il semaforo `mutex3`: Garantisce che l'accesso iniziale dei lettori a `R` sia sincronizzato e impedisce race condition all'ingresso.

Il codice del lettore:

```C
void lettore() {
    wait(mutex3); // sincronizza lettori
    wait(R); // attende tutti gli scrittori 
    wait(mutex1); // protegge countR
    countR++;
    if (countR==1) wait(W); // blocca altri scrittori
    signal(mutex1);
    signal(R);
    signal(mutex3); 
    /*lettura dati*/
    wait(mutex1);
    countR--;
    if (countR==0) signal(W);// sblocca altri scrittori
    signal(mutex1);
}
```

- Ingresso (inizio lettura):        
    1. wait(mutex3): Sincronizza i lettori per l'accesso controllato a R.        
    2. wait(R): Attende che non ci siano scrittori con priorità (gestito da countW).        
    3. wait(mutex1): Protegge l'incremento di countR.        
    4. Incrementa countR: Se è il primo lettore (countR == 1), blocca gli scrittori con wait(W).        
    5. Rilascia i lock (signal) in ordine inverso per consentire ad altri lettori di procedere.    
- Sezione critica (lettura dati): Può essere eseguita da più lettori contemporaneamente, dato che solo il primo lettore ha bloccato gli scrittori.    
- Uscita (fine lettura):        
  1. wait(mutex1): Protegge il decremento di countR.        
  2. Decrementa countR: Se è l'ultimo lettore (countR == 0), sblocca gli scrittori con signal(W).        
  3. Rilascia mutex1.

Il codice dello scrittore

```C
void scrittore() {
    wait(mutex2); // protegge countW
    countW++;
    if (countW==1) wait(R); // blocca altri lettori
    signal(mutex2);
    wait(W); // un solo scrittore può accedere a W
    /*scrittura dati*/
    signal(W); 
    wait(mutex2);
    countW--;
    if (countW==0) signal(R);// sblocca altri lettori
    signal(mutex2);
}
```
- Ingresso (inizio scrittura):
  1. wait(mutex2): Protegge l'incremento di countW.
  2. Incrementa countW: Se è il primo scrittore (countW == 1), blocca i lettori con wait(R).
  3. Rilascia mutex2.
  4. wait(W): Blocca l'accesso ad altri scrittori o lettori, garantendo l'esclusività.
- Sezione critica (scrittura dati): Solo uno scrittore può accedervi grazie al blocco su W.
- Uscita (fine scrittura):
  1. Rilascia W per consentire accesso a lettori o scrittori.
  2. wait(mutex2): Protegge il decremento di countW.
  3. Decrementa countW: Se è l'ultimo scrittore (countW == 0), sblocca i lettori con signal(R).
  4. Rilascia mutex2.

### Cinque filosofi

Ci sono $N=5$ filosofi che passano la vita mangiando e pensando. Sono tutti seduti ad una tavola su cui sono presenti $N$ bacchette e una ciotola di riso. 

Se un filosofo ha fame, per mangiare, può usare solo ed esclusivamente 2 bacchette, ma ne riesce a prendere solo 1 alla volta: se un filosofo non ha 2 bacchette, allora non può mangiare. 

Quando un filosofo termina di mangiare, appoggia le bacchette.

Un tentativo di soluzione prevede che ci sia un vettore di $N$ semafori associati alle bacchette. Il codice di un filosofo è:

``` C
do {
    /*prende le bacchette*/
    wait(sem[i]);
    wait(sem[i+1 % N])
    ...mangia...
    /*lascia le bacchette*/
    signal(sem[i]);
    signal(sem[i+1 % N])
}

```

Questa soluzione non è _deadlock free_ e nemmeno _starvation free_: se tutti i filosofi prendessero contemporaneamente una bacchetta rimarrebbero bloccati. Inoltre, non è garantito che tutti i filosofi possano mangiare entro un tempo finito.

#### Rimedi possibili

- controllare che la bacchetta di destra sia libera prima di prendere la bacchetta di sinistra: risolve il problema _deadlock_, ma la _starvation_ rimane possibile.
- utilizzando lo stesso controllo, ma introducendo un ritardo casuale prima di riprovare a prendere la bacchetta: _deadlock free_, semplice da implementare e spesso usato in situazioni non critiche
- introdurre un _mutex_ che racchiude la sezione critica prima delle _wait_: _deadlock free_ e si evita _starvation_ se la coda è gestita in modo fair, ma il parallelismo ne risente.
- introdurre un semaforo inizializzato a $S=4$ che racchiude la sezione critica prima delle _wait_ permettendo solo a 4 filosofi di mangiare
- utilizzare una soluzione asimmetrica in cui i filosofi dispari hanno priorità sui filosofi pari

La soluzione ottimale è quella di modellare lo stato di ogni filosofo in `THINKING`, `HUNGRY`, `EATING` e imporre che ogni filosofo controlli in che stato siano i vicini prima di prendere le bacchette.

#### Implementazione

``` C
#include <semaphore.h>
#define N 5

// Macro per i vicini
#define RIGHT(i) ((i + 1) % N)
#define LEFT(i) ((i + N - 1) % N)

// Variabili condivise
int stato[N]; // Array per gli stati dei filosofi
sem_t mutex; // Semaforo binario per mutua esclusione
sem_t sem[N]; // Un semaforo per ogni filosofo


// Funzione del filosofo
void filosofo(int process) {
    while (1) {
        ...Pensa...
        get_sticks(process);
        ...Mangia...
        put_sticks(process);
    }
}
```

``` C

void test(int i) {
    if ( stato[i] == HUNGRY &&
    stato[LEFT(i)] != EATING &&
    stato[RIGHT(i)] != EATING ) {
        stato[i] = EATING;
        signal(sem[i]);
    }
}

void get_sticks(int i) {
    wait(mutex);
    stato[i] = HUNGRY;
    test(i);
    signal(mutex);
    wait(sem[i]);
}

void put_sticks(int i) {
    wait(mutex);
    stato[i]= THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    signal(mutex);
}
```
