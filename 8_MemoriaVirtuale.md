# Memoria virtuale

## Motivazioni

Non è necessario mantenere in memoria l'intero programma perché questo venga eseguito. Caricare in memoria solo la parte (le _pagine_) di programma che sono necessarie all'esecuzione istante per istante ottimizza l'uso della memoria. Così facendo lo _spazio logico_ degli indirizzi è svincolato dallo _spazio fisico_ e può essere maggiore. Il grado di multiprogrammazione può aumentare in quanto più processi possono essere mantenuti (parzialmente) in memoria.

Il meccanismo chiave prevede lo **swapping di pagine** invece che di processi.

La **memoria virtuale** permette la separazione (**virtualizzazione**) tra _memoria logica_  e _memoria fisica_: $Memoria virtuale = Memoria\ fisica (RAM) + Memoria\ secondaria (Disco)$.

L'implementazione avviene tramite il meccanismo di **demand paging** (_paginazione su richiesta_).

## Paginazione su richiesta

Una pagina viene caricata in memoria solo quando è necessaria per proseguire l'esecuzione. Sfrutta il principio della _località dei riferimenti_  secondo il quale durante l'esecuzione di una data istruzione presente in memoria, con molta probabilità, le successive istruzioni saranno ubicate nelle vicinanze di quella in corso. In questo modo un processo occupa meno memoria e si può mantenere in esecuzione più processi, inoltre, se solo parte del programma è caricato in memoria, saranno necessarie meno operazioni I/O per caricarlo o scaricarlo.

Diventa necessario associare ad ogni pagina logica uno stato che indica _in memoria_ o _non in memoria_.

### Tabella delle pagine

Ad ogni pagina nella tabella delle pagine è associato un _bit di validità_ che indica se la pagina è presente in memoria centrale (1) o se si trova in memoria secondaria (0) e quindi non è possibile accedervi direttamente. All'avvio del processo tutti i bit di validità sono 0 (il processo è tutto in memoria secondaria), se durante la traduzione degli indirizzi logici in indirizzi fisici si trova il bit di validità a 0 si ha **page fault** che indica che la pagina _non è caricata_ oppure che l'indirizzo è _illegale_.

### Page fault

Un **page fault** causa un **interrupt** che passa il controllo al S.O. che esegue le seguenti azioni:

- accede alla tabella interna sel processo per determinare la causa del page fault:
  - _accesso illegale_: il processo viene abortito
  - _pagina non è in memoria_: attiva la procedura per caricarla
- cerca un frame vuoto dalla tabella dei frame liberi
- trasferisce la pagina dal disco al frame
- aggiorna le tabelle:
  - page table del processo
  - tabella interna associata al processo
  - bit di validità
  - ...
- riavvia l'istruzione che ha generato page fault

NB: Il primo accesso in memoria di un programma risulta essere sempre un page fault.

Riavviare l'istruzione che ha generato il _page fault_ potrebbe non essere banale. Ad esempio un istruzione potrebbe copiare dati da un area d'origine (_source_) ad un area di destinazione (_destination_) parzialmente sovrapposta a _source_. Durante questa copia, l'accesso a una delle pagine coinvolte (origine o destinazione) può causare un _page fault_:

Supponendo che la _pagina destinazione_ non sia in memoria quando inizia la copia, questo genera un _page fault_. Il sistema operativo interrompe quindi l'esecuzione per caricare la pagina mancante.

Una volta caricata la pagina, l'istruzione deve essere riavviata. Tuttavia, se l'istruzione riparte dall'inizio, potrebbe riscrivere parte dei dati già copiati, portando a un risultato errato, soprattutto se _source_ e _destination_ si sovrappongono.

Esempio:

- source = `[0x1000, 0x2000]`
- destination = `[0x1800, 0x2800]`

Si vuole copiare byte per byte da `0x1000` a `0x2000` verso `0x1800` a `0x2800`.

Step della copia:

- Copia il primo byte da `0x1000` (in source) a `0x1800` (in destination).
- Prosegue con i successivi byte.
- Ad un certo punto, si accede alla pagina che contiene `0x2000` per leggere da source, ma questa pagina non è in memoria. Questo genera un page fault.
- Il S.O. gestisce carica la pagina mancante e riprende a copiare i dati da `[0x1000]`, ma una parte di memoria verrebbe ricopiata nuovamente con i dati che vi erano già stati copiati in precedenza.

Per risolvere il problema alcuni S.O. operano un _ripristino_ del contenuto della memoria prima di gestire il _fault_, mentre altri iniziano l'esecuzione solo se sono presenti tutte le pagine.

Nel caso non ci fossero frame liberi si deve attuare una procedura di **rimpiazzamento delle pagine** che prevede la ricerca di un frame in memoria occupato da una pagina _vittima_ e lo scambio di questa pagina con quella richiesta. Solo se la pagina vittima è stata modificata si deve aggiornare la corrispondente pagina in memoria secondaria.

Una _pagina vittima_ ideale sarebbe una pagina che non verrebbe più utilizzata, oppure una che abbia poca probabilità di essere scelta subito dopo il suo _swap_ in memoria secondaria.

### Supporto alla paginazione su richiesta

La paginazione su richiesta necessita di un supporto hardware per:

- tabella delle pagine
- swap area nella memoria secondaria
- possibilità di riavviare le istruzioni in caso di fault

### Prestazioni della paginazione su richiesta

Il tempo di accesso effettivo alla memoria (**Effective Acces Time**: **EAT** ) è definito come:

$$
EAT = (1-p) \times tm + p \times tg
$$

in cui:

- $p$: **tasso di page fault**, ovvero la probabilità che si verifichi un page fault
- $tm$: tempo di accesso alla memoria
- $tg$: tempo di gestione del page fault:
  - tempo servizio dell'interrupt
  - tempo di lettura della pagina (swap-in della singola pagina)
  - tempo di riavvio del processo

Ad esempio:

- $tm= 100ns$
- $tg= 1ms= 1000000ns$

$$
\begin{aligned}
  EAT &= (1 - p) \times 100 + 1000000 \times p\\
  EAT &= 100 ((1-p) + p \times 1000)\\
  EAT &= 100 (1- p + p \times 1000)\\
  EAT &= 100 (1 + p(-1 + 1000))\\
  EAT &= 100 (1 + 999 \times p)
\end{aligned}
$$

Se volessimo un peggioramento al massimo del $10%$ dovremmo rispettare il vincolo:

$110 > 100 \times (1 + 999 \times p)$

Ovvero mantenere $p < 10^{-4}$.

Ciò significa che possiamo permettere al più un _page fault_ ogni $10^{4}$ accessi

## Rimpiazzamento delle pagine

In assenza di frame liberi sono necessari due accessi alla memoria secondaria:

- _swap out_ della pagina vittima
- _swap in_ della pagina da caricare

Questo determina una perdita di tempo doppia rispetto a quanto servirebbe se ci fosse un frame libero.

Un ottimizzazione prevede l'uso nella page table di **dirty bit** che indicano se sono state fatte o meno delle modifiche alla pagina. In questo modo si evita di trasferire su disco le vittime con _dirty bit_ a zero (quelle non modificate).

Ci sono quindi due sottoproblemi da risolvere:

- **rimpiazzamento pagine**: come scegliere la pagina da rimpiazzare
- **allocazione frame**: quanti frame assegnare ad ogni processo

In linea di massima la frequenza di page fault è inversamente proporzionale al numero di frame a disposizione.

## Algoritmi di rimpiazzamento delle pagine

L'obiettivo di questi algoritmi è ottenere il minor numero possibile di _page fault_ e possono essere confrontati paragonando il loro operato alla sequenza di accessi alla memoria, inoltre, anziché lavorare a livello di indirizzi, questi algoritmi lavorano a livello di _pagine_. Ad esempio avendo pagine da 1KB e volendo accedere agli indirizzi:

`1050` `7076` `1100` `1040` `2095` `2210` `2058` `3111`

le pagine accedute sarebbero:

`1` `6` `1` `1` `2` `2` `2` `3`

tuttavia gli accessi successivi alla stessa pagina generano al massimo un page fault, quindi la sequenza diventa:

`1 6 1 2 3`

Nei prossimi esempi di algoritmi per la _selezione della pagina vittima_ si farà riferimento alla sequenza di pagine:

`7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`

Inoltre sarà assunto di avere a disposizione 3 frame.

### Anomalia di Belady

Un algoritmo di selezione della pagina soffre dell'**anomalia di Belady** se all'aumentare del numero dei frame disponibili il numero di page fault non decresce, ma può aumentare.

### Algoritmo ottimale

L'algoritmo ottimale è quello che garantisce il numero minimo di _page fault_, è molto difficile da implementare in quanto bisogna conoscere in anticipo la sequenza di pagine richieste.

Dal punto di vista teorico può essere utilizzato come benchmark per valutare la bontà di altri algoritmi in dati esempi di sequenze.

L'algoritmo ottimale per la sequenza di riferimento è:

| 7   | 0   | 1   | 2   | 0   | 3   | 0   | 4   | 2   | 3   | 0   | 3   | 2   | 1   | 2   | 0   | 1   | 7   | 0   | 1   |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 7   | 7   | 7   | 2   |     | 2   |     | 2   |     |     | 2   |     |     | 2   |     |     |     | 7   |     |     |
|     | 0   | 0   | 0   |     | 0   |     | 4   |     |     | 0   |     |     | 0   |     |     |     | 0   |     |     |
|     |     | 1   | 1   |     | 1   |     | 3   |     |     | 3   |     |     | 1   |     |     |     | 1   |     |     |

In questo caso sono presenti 9 page fault.

### FIFO: First In First Out

Seleziona la pagina presente in memoria da più tempo.

Questo è un algoritmo semplice da implementare, è _cieco_ in quanto non considera l'effettivo utilizzo delle pagine. Tende ad aumentare la frequenza dei _page fault_ e soffre dell'_anomalia di Belady_

| 7   | 0   | 1   | 2   | 0   | 3   | 0   | 4   | 2   | 3   | 0   | 3   | 2   | 1   | 2   | 0   | 1   | 7   | 0   | 1   |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 7   | 7   | 7   | 2   |     | 2   | 2   | 4   | 4   | 4   | 0   |     |     | 0   | 0   |     |     | 7   | 7   | 7   |
|     | 0   | 0   | 0   |     | 3   | 3   | 3   | 2   | 2   | 2   |     |     | 1   | 1   |     |     | 1   | 0   | 0   |
|     |     | 1   | 1   |     | 1   | 0   | 0   | 0   | 3   | 3   |     |     | 3   | 2   |     |     | 2   | 2   | 1   |

In questo caso sono presenti 15 page fault.

### LRU: Least Recently used

Seleziona la pagina che nella sequenza degli accessi non è stata usata da più tempo.

Si basa sul fatto che le pagine a cui sono stati fatti accessi di recente verranno subito riutilizzate. Come algoritmo è complesso da implementare in quanto bisogna tenere traccia dei tempi degli accessi alle pagine e c'è bisogno di un supporto hardware

| 7   | 0   | 1   | 2   | 0   | 3   | 0   | 4   | 2   | 3   | 0   | 3   | 2   | 1   | 2   | 0   | 1   | 7   | 0   | 1   |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 7   | 7   | 7   | 2   |     | 2   |     | 4   | 4   | 4   | 0   |     |     | 1   |     | 1   |     | 1   |     |     |
|     | 0   | 0   | 0   |     | 0   |     | 0   | 0   | 3   | 3   |     |     | 3   |     | 0   |     | 0   |     |     |
|     |     | 1   | 1   |     | 3   |     | 3   | 2   | 2   | 2   |     |     | 2   |     | 2   |     | 7   |     |     |

In questo caso sono presenti 12 page fault.

Per trovare la pagina usata meno di recente si può usare:

- un **contatore** associato ad ogni pagina contenente il timestamp dell'ultimo utilizzo e ogni volta si va alla ricerca del timestamp meno recente.

- uno **stack** hardware che rappresenta la cronologia delle pagine a cui è stato fatto l'accesso. Ad ogni accesso di una pagina si elimina l'eventuale doppione nello stack e si mette in prima posizione la pagina a cui è stato fatto l'accesso. La _pagina vittima_ è quella che si trova in fondo allo stack.

#### Approssimazioni LRU

- uso del **reference bit**: alle pagine è associato un bit di validità a 0 e se vengono referenziate questo bit viene messo a 1. Vengono selezionate le pagine con bit a 0.
- uso di **più reference bit**: si usano $K$ bit (es: _shift register_) per pagina e a intervalli regolari si fanno scorrere i bit contenuti nei registri come a simulare un processo di invecchiamento. Viene selezionata la pagina con valore inferiore, ovvero con l'1 più a destra.
- **Not Used Recently (NUR)**: vengono usati _reference bit_ $R$ e _dirty bit_ $M$. Ad ogni referenza si pone $R$ a 1 e ad ogni modifica si pone $M$ a 1. Periodicamente si riazzerano i _reference bit_ $R$. Considerando $MR$ come un numero binario ($MR \in \{00, 01 , 10 ,11\} = \{0, 1, 2, 3\}$) si seleziona la pagina con $MR$ inferiore.

### Tecniche basate sul conteggio

- **Least Frequently Used (LFU)**: ad ogni riferimento si incrementa un _contatore_ alla pagina e viene selezionata la pagina con  il _contatore_ più basso. Come controindicazione ha che alcune pagine in memoria da molto tempo avranno un _contatore_ molto alto rispetto a pagine appena entrate in memoria ed è molto difficile rimpiazzarle anche nel caso non venissero più utilizzate.
- **Most Frequently Used (MFU)**: criterio inverso a _LFU_ si basa sul fatto che una pagina appena caricata in memoria ha un _contatore_ basso, ma, per principio di località, ha più probabilità di essere richiamata a breve.

### Algoritmo di seconda chance

Ad ogni pagina è associato un _reference bit_ che indica la "vecchiaia" della pagina e le pagine sono considerate in una coda circolare ordinate in base al _tempo di caricamento_.

Quando la pagina è _riferita_ si pone il suo _reference bit_ a 1. Quando si deve selezionare una pagina vittima si parte dalla più vecchia nella coda circolare, se il suo _reference bit_ è 0, la pagina viene selezionata, se è 1 il bit viene scalato a 0 (le viene concessa una seconda chance) e si procede con la ricerca della pagina nella coda circolare analizzando la prossima pagina più vecchia.

Intuitivamente è come se una pagina invecchiasse a causa della coda _FIFO_, ma ringiovanisce quando il suo bit viene reimpostato a 0.

## Allocazione dei frame

Supponendo di voler caricare in memoria $N$ processi e di poter utilizzare $M$ frame, bisogna decidere quanti frame assegnare ad ogni processo tenendo conto di eventuali vincoli sul numero minimo di frame.

L'istruzione che ha causato il _page fault_ deve essere riavviata, quindi tutte le _pagine_ che usa devono essere presenti e il numero di _frame_ deve essere sufficiente ad eseguire l'istruzione che prevede il maggior numero di riferimenti possibile. Ad esempio, se il linguaggio macchina prevede indirizzamento indiretto, allora i dati con cui si vuole lavorare possono essere presenti in 3 pagine/frame: una per l'istruzione, una per il dato riferito e una per il dato vero e proprio. Un istruzione di copia e sposta potrebbe aver bisogno di 6 pagine/frame.

### Schemi

Dovendo allocare $M$ frame suddividendoli fra $N$ processi, sono presenti 2 strategie principali di allocazione:

- **allocazione uniforme**: a ciascun processo si concedono $\frac{M}{N}$ frame
- **allocazione proporzionale**: ad un processo $P_{i}$ di dimensioni $D_{i}$, si concede un numero proporzionale di frame alla sua dimensione: $M \times \frac{D_{i}}{\sum_{j}D_{j}}$. Si possono usare anche altri criteri di proporzionalità.

### Contesto di rimpiazzamento

Quando un processo $P$ genera _page fault_ la _pagina vittima_ può essere scelta:

- in **contesto locale** fra le pagine di $P$
- in **contesto globale** fra le pagine di tutti i processi

### Thrashing

Il **thrashing** è un fenomeno che avviene quando un sistema alloca troppi pochi _frame_ ad ogni _processo_. In questa situazione si generano molti _page fault_ che, per essere gestiti, impediscono ai processi di rimanere abbastanza tempo in stato _ready_ rendendo la CPU sottoutilizzata. Per rimediare a questo sottoutilizzo e aumentare la multiprogrammazione, il S.O. caricherà in memoria più processi che avranno bisogno di più pagine che genereranno più _page fault_ peggiorando ulteriormente la situazione.

Il **working set** è il quantitativo di pagine di memoria che un processo richiede per proseguire l'elaborazione in un momento di tempo.

I sistemi di memoria virtuale funzionano in modo efficiente quando i _working set_ utilizzati dai processi non superano di molto il limite di archiviazione reale del sistema: in questa situazione solo poche risorse vengono spese per risolvere i _page fault_. Man mano che i _working set_ crescono, la risoluzione degli errori di pagina rimane gestibile finché la crescita non raggiunge un punto critico in cui il numero di errori aumenta drasticamente e il tempo impiegato per risolverli supera il tempo impiegato dal programma per elaborare dati.

Per risolvere questo problema è necessario ridurre il _grado di multiprogrammazione_ e usare _swapping_ per assicurare ai processi presenti in memoria i frame necessari. Si possono definire i _frame necessari_ in base al modello di località:

- **temporale**: locazioni riferite recentemente hanno buona probabilità di essere riferita nuovamente
- **spaziale**: locazioni fisicamente vicine ad una locazione appena riferita

### Tecnica del working set

Utilizzando il **working set** di un processo è possibile determinare la dimensione della _località attuale_ del processo. 

Per ogni processo $P_{i}$ si definisce una _finestra di working set_ considerando gli ultimi $\Delta$ riferimenti che $P_{i}$ ha effettuato e si definisce il **working set $WS_{i}$** come l'insieme delle pagine riferite negli ultimi $\Delta$ accessi. La dimensione del $WS_{i}$ stima il numero di frame necessari a $P_{i}$.

Se il S.O. rileva che il numero di _frame_ a disposizione è inferiore alla somma delle dimensioni delle $WSS_{i}$ (_Working Set Sizes_) allora deve diminuire il grado di multiprogrammazione.

Se $\Delta$ è troppo piccolo un processo avrà meno pagine caricate in memoria e genererà più _page fault_, mentre se è troppo grande il processo occupa più spazio del necessario.

Dato cha la valutazione delle $WSS$ può comportare inefficienza si approssima il $WS$ con dei reference bit posti a 1 in caso di accesso e azzerati a intervalli regolari. Le pagine che hanno almeno un bit a 1 tra quelli salvati negli ultimi $k$ intervalli compongono un approssimazione del _working set_.

### Tecnica del page fault frequency

Viene monitorata la frequenza di page fault. Se un processo genera un numero di _page fault_ inferiore ad una soglia minima, allora si può sottrarre frame a questo processo, altrimenti, se genera un numero di _page fault_ maggiore ad una soglia massima, bisogna concedere più frame al processo.

Una frequenza di _page fault_ compresa tra le due soglie risulta accettabile.

### Compressione memoria

Nel caso non sia possibile fare né _swapping_ né rimpiazzamento pagine, si può utilizzare la tecnica della **compressione della memoria**.

Quando è necessario liberare della memoria si selezionano $k$ frame da salvare in un formato compresso in uno dei frame liberi che viene posto nella lista dei frame compressi. Così facendo vengono liberati $k-1$ frame.

Quando capita un riferimento ad uno dei frame compressi si attua la decompressione.

L'efficienza della tecnica dipende dagli algoritmi di compressione/decompressione e dal bilanciamento fra rapporto di compressione e velocità di esecuzione.
