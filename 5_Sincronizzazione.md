# Sincronizzazione

## Il problema della sezione critica

### Processi concorrenti

Un S.O. deve gestire più processi contemporaneamente. Questi possono eseguire in maniera logicamente indipendente o cooperante. In generale due processi compiono le loro azioni inframezzandole e utilizzando risorse condivise, tali processi necessitano di meccanismi di sincronizzazione, comunicazione e accesso a dati/informazioni condivise.

L'accesso contemporaneo ai dati condivisi può essere problematico in quanto i dati possono essere modificati in maniera incoerente. Il S.O. deve implementare metodo che garantiscano la consistenza dei dati permettendo ad un solo processo per volta di accedere a dati condivisi. Questo deve avvenire garantendo un tempo di attesa ragionevole, con una politica che non penalizzi nessun processo e senza la creazione di attese infinite.

- **Sezione critica**: sezione di codice che legge/scrive dati condivisi.
- **Race condition**: situazione in cui due o più processi eseguono concorrentemente sulla sezione critica. Il risultato dell'esecuzione dipende dall'ordine in cui i processi si alternano e perciò è fonte di inconsistenza.
- **Mutua esclusione**: meccanismo che evita le race condition assicurando l'accesso esclusivo ai dati ad un solo processo alla volta.
- **Deadlock**: situazione in cui due o più processi sono bloccati a causa di un attesa reciproca.
- **Starvation**: situazione in cui ad un processo che ha richiesto la sezione critica non viene mai dato il permesso di proseguire nell'esecuzione.

### Esempio

Due processi, uno _produttore_ e uno _consumatore_ hanno in condivisione un buffer

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

Anche se presi singolarmente i processi sono corretti, in caso di esecuzione concorrente la gestione della variabile ```counter``` non è adeguata: ```counter++``` e ```counter--``` sono due sezioni critiche in quanto a livello assembly sono eseguite come più operazioni basilari in successione. Un interrupt può avvenire quando solo parte delle istruzioni sono state eseguite e causare inconsistenza. Le due istruzioni sono _sezioni critiche_ e questa è una _race condition_.

### Soluzione generale al problema della sezione critica

Una soluzione a questo problema deve garantire:

- **Mutua esclusione**: un solo processo esegue la sezione critica.
- **Progresso**: la decisione su chi esegue in sezione critica deve avvenire entro un certo limite di tempo, senza interferenze da eventuali processi già all'interno della sezione critica.
- **Attesa limitata**: non deve esserci starvation per processi che chiedono di entrare in sezione critica.

Quindi si ha che:  

- Nessun processo esterno alla sezione critica può impedire ad un altro di entrarvi
- se un processo si blocca in sezione critica, non può interferire con l'esecuzione i altri processi
- ogni processo resta nella sezione solo per un tempo finito
- se nessun processo è presente nella sezione critica, allora un richiedente deve poterci entrare subito
- tutti i processi devono evolvere
- non bisogna fare assunzioni sul numero delle CPU

Bisogna quindi progettare un protocollo che se adottato dai processi garantisca che l'effetto globale delle loro azioni non dipenda dall'ordine in cui queste si inframezzano. In generale questo prevede che la sezione critica sia racchiusa fra due porzioni di codice dette **sezione d'ingresso** e **sezione d'uscita**. La richiesta d'accesso avviene eseguendo il codice relativo alla _sezione d'ingresso_, mentre l'esecuzione del codice relativo alla _sezione d'uscita_ abbandona la sezione critica.

``` pseudo
    sezione non critica
    sezione d'ingresso
    sezione critica
    sezione d'uscita
    sezione non critica
```

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

Pur garantendo mutua esclusione, non garantisce progresso e attesa limitata: ad un processo $P_{i}$ viene conferita l'autorizzazione ad entrare in sezione critica solo da un altro processo $P_{j}$ quando quest'ultimo esce dalla sezione critica. Se $P_{j}$ si blocca o non necessita di entrare allora il primo processo $P_{i}$ resta bloccato all'esterno della sezione critica. Un implementazione del genere non prevede che un processo entri in sezione critica due volte consecutivamente.

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

Il problema di questa implementazione risiede nel fatto che il funzionamento dipende dalla velocità reciproca dei processi. Potrebbe accadere che `flag[0]=flag[1]=True`, in questo caso si presenta deadlock.

#### Soluzione di Peterson

Questa soluzione utilizza sia una variable `turno` che due flag per dichiarare interesse a entrare in sezione critica. Il codice di un processo $P_{i}$ è il seguente:

``` C
boolean flag[2];
int turno;
do {
    ....sezione non critica...
    flag[i] = true; // Pi dichiara il suo interesse
    turno = j; // assume che sia il turno dell’altro e gli offre l’occasione di entrare
    while (flag[j] && turno==j) // basta che una non sia vera per uscire
    ; // attesa del proprio turno
    ....sezione critica...
    flag[i] = false; // fine dell’interesse
    ....sezione non critica...
} while (1);
```

La variabile `turno` garantisce la mutua esclusione, elimina la possibilità di deadlock ed evita che un processo monopolizzi la sezione critica, mentre l'uso dei flag evita che un processo venga chiuso fuori dalla sezione critica nel caso un altro processo non gli dia l'autorizzazione, permettendo così ai processi di entrare in sezione critica più volte consecutivamente.

Questa soluzione non è ottimale in quanto introduce _busy waiting_, prevede che gli aggiornamenti di `turno` e `flag[i]` avvengano con istruzioni atomiche ed è difficile da scalare nel caso dovessero esserci più programmi. Un altro problema risiede nel fatto che in processi multithread il compilatore potrebbe applicare delle ottimizzazioni che riordinano le istruzioni macchina.

#### Soluzioni basate sulle interruzioni

Questo tipo di soluzioni prevedono la disabilitazione delle interruzioni durante l'esecuzione della sezione critica per evitare preemption:

``` pseudo
    sezione non critica
    disabilita interruzioni
    sezione critica
    abilita interruzioni
    sezione non critica
```

Nei sistemi _mono processore_ garantisce mutua esclusione e atomicità, ma gli switch di contesto inficiano l'efficienza. Nei sistemi _multi processore_ la mutua esclusione non viene garantita in quanto bisognerebbe bloccare/allertare tutti gli altri processori. La sezione critica resta atomica (le operazioni vengono eseguite come un'unica unità indivisibile), ma non esclusiva.

Inoltre è possibile che si verifichino deadlock a causa di operazioni di I/O durante una sezione critica.

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

Istruzione atomica che legge il valore di una variabile (`target`), lo restituisce e contemporaneamente la imposta a TRUE. Garantisce che nessun altro processo possa modificare `target` durante l'operazione. Garantisce mutua esclusione.

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

Soffre di busy waiting e non garantisce attesa limitata.

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
Soffre di busy waiting e non garantisce attesa limitata.

##### Compare and swap

Istruzione atomica che confronta il valore di una variabile (`val`) con un valore atteso (`test`). Se coincidono, aggiorna la variabile con un nuovo valore (`newval`). In ogni caso, restituisce il valore originale di `val`. Serve a garantire la sincronizzazione tra processi.

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

Garantisce mutua esclusione, progresso e attesa limitata, ma è ancora presente busy waiting.

#### Variabili atomiche

Queste istruzioni implementano operazioni aritmetiche e booleane in maniera atomica, così facendo eseguono codice in mutua esclusione anche in sistemi multi processore. Possono essere utilizzate come base per costruire protocolli che assicurano mutua esclusione, progresso e attesa limitata.

### Busy waiting

Una soluzione ottimale dovrebbe evitare che i processi in attesa di entrare nella sezione critica occupino inutilmente la CPU. Per fare ciò è necessario che un processo in attesa di entrare in sezione critica cambio stato in _waiting_ per non occupare la CPU. Questo processo tornerebbe _ready_ solo quando ha la possibilità di entrare nella sezione critica.

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

Un semaforo generico con `S` inizializzato a un valore $n > 1$ diventa un contatore generico in cui $n$ programmi hanno accesso alla zona critica/risorsa in comune.

### Problema del busy waiting

Un semaforo implementato con busy waiting è detto _spinlock_. Se l'attesa è breve sono vantaggiosi in quanto non richiedono switch di contesto, ma se le attese diventano lunghe eseguire uno switch di contesto diventa preferibile al mantenere _running_ un processo che utilizza tempo di CPU facendo nulla.

Per risolvere questo problema un processo che esegue una `wait(S)` quando il valore `S` è minore o uguale a zero viene inserito in una coda di attesa al semaforo e portato in stato di _waiting_. In questo modo il controllo passa allo _short term scheduler_ che permette ad un o dei processi in coda di passare allo stato di _ready_ quando viene eseguita una `signal(S)`.

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
        Processo *P = togli_da_lista(S->lista);
        wakeup(P);
    }
}
```

## Tipici problemi di sincronizzazione

### Produttori e consumatori

### Lettori e scrittori

### Cinque filosofi

Ci sono $N=5$ filosofi che passano la vita mangiando e pensando. Sono tutti seduti ad una tavola su cui sono $N$ bacchette e una ciotola di riso. Se un filosofo ha fame, per mangiare, può usare solo 2 bacchette, ma ne riesce a prenderne solo 1 alla volta: se un filosofo non ha 2 bacchette, allora non può mangiare. Quando un filosofo termina di mangiare appoggia le bacchette.

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

Questa soluzione non è deadlock free e nemmeno starvation: se tutti i filosofi prendessero contemporaneamente una bacchetta, rimarrebbero bloccati e non è garantito che tutti i filosofi possano mangiare entro un tempo finito.

### Rimedi possibili

- controllare che la bacchetta di destra sia libera prima di prendere la bacchetta di sinistra: risolve il problema deadlock, ma la starvation rimane possibile.
- utilizzando lo stesso controllo, ma introducendo un ritardo casuale prima di riprovare a prendere la bacchetta: deadlock free, semplice da implementare e spesso usato in situazioni non critiche
- introdurre un _mutex_ che racchiude la sezione critica prima delle _wait_: deadlock free e si evita starvation se la coda è gestita in modo fair, ma il parallelismo ne risente.
- introdurre un semaforo inizializzato a $S=4$ che racchiude la sezione critica prima delle _wait_ permettendo solo a 4 filosofi di mangiare
- utilizzare una soluzione asimmetrica in cui i filosofi dispari hanno priorità sui filosofi pari

La soluzione ottimale è quella di modellare lo stato di ogni filosofo in `THINKING`, `HUNGRY`, `EATING` e imporre che ogni filosofo controlli lo stato dei vicini prima di prendere le bacchette.

### Implementazione

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
