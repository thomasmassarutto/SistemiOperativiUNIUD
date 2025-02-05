\newpage

# Memoria centrale

## Generalità

Per fare in modo che la memoria venga condivisa fra processi bisogna essere affrontare le problematiche riguardanti l'allocazione della memoria, la protezione dello spazio di indirizzamento, la condivisione dello spazio di indirizzamento e la gestione dello swap. Inoltre, nei sistemi moderni, si fa spesso uso della _memoria virtuale_ che aggiunge un ulteriore layer di complessità.

Un **programma** per essere eseguito deve essere caricato in memoria principale e trasformato in un **processo**. Per eseguire le istruzioni, la CPU le preleva dalla memoria in base al valore del _program counter_. Le istruzioni vengono quindi decodificate e se necessario, si recuperano gli operandi dalla memoria. Al termine dell'esecuzione dell'istruzione, il risultato può essere scritto in memoria, mentre alla fine del programma, la memoria allocata per il processo viene rilasciata.

### Indirizzi e memoria

Durante questo processo si adottano in ogni fase diverse semantiche per distinguere gli _indirizzi di memoria_ dallo _spazio di memoria_. In particolare, si differenziano **indirizzi logici** e **indirizzi fisici** fra le fasi di costruzione del programma.

### Binding degli indirizzi

Nel programma **sorgente** gli indirizzi sono **simbolici**, astratti: non indicano un indirizzo fisico o numerico specifico, ma fungono da riferimento. 

Nella fase di **compilazione** gli indirizzi simbolici vengono trasformati in **indirizzi rilocabili**, ovvero non ancora associati a una posizione fisica precisa nella memoria del sistema, ma rilocati/adattati in base a dove il programma viene effettivamente caricato. 

Durante la fase di **collegamento** o **caricamento** gli indirizzi rilocabili vengono trasformati in **indirizzi assoluti** associati a indirizzi esistenti in memoria centrale.

L'associazione fra _indirizzi simbolici_ e _indirizzi fisici_ è detta **binding** e può avvenire in vari momenti:

- **In compilazione**: il **binding statico** avviene al _compile time_ in cui la posizione in memoria centrale è nota a priori. Questo approccio genera un codice assoluto per cui, se il programma deve essere spostato, necessita di ricompilazione.
- **In caricamento**: il **binding statico** avviene al _load time_. In compilazione si genere del codice rilocabile in cui gli indirizzi sono relativi e inizializzati all'inizio del caricamento del programma. Per cambiare la collocazione basta ricaricare il programma.
- **In esecuzione**: avviene **binding dinamico** al _run time_, in questo caso il binding è posticipato in fase di esecuzione. In questo modo il programma può essere spostato in posizioni diverse della memoria. Per questa metodologia è richiesto un supporto hardware dedicato come la **MMU** (_Memory Management Unit_). Ad ogni _indirizzo logico_ viene aggiunto il contenuto di un _registro di rilocazione_ (che funge da offset). L'indirizzo risultante viene usato per accedere alla memoria. Ad esempio, se il valore all'interno del _registro di rilocazione_ è $14000$ e la CPU genera l'_indirizzo logico_ $123$, l'indirizzo effettivo per accedere alla memoria è $14000 + 123= 14123$.

Schematicamente la CPU genera, a partire dal sorgente, _indirizzi logici_ che corrispondono a _locazioni di memoria fisica_.

### Caricamento statico vs dinamico

Il **caricamento** (trasferimento di codice eseguibile da memoria secondari a primaria) può essere:

- **statico**: l'intero codice viene caricato in memoria
- **dinamico**: i moduli di codice vengono caricati al momento del loro primo utilizzo per risparmiare memoria. Solitamente richiede un intervento minimo da parte del S.O.

### Collegamento statico vs dinamico

Il **collegamento** (associazione dei riferimenti simbolici nel codice alla loro controparte fisica in memoria) può essere:

- **statico**: tutti i riferimento del codice sono definiti prima dell'avvio dell'esecuzione
- **dinamico**: utilizzato nelle librerie, il _linking_ avviene posticipato a tempo di esecuzione, così il codice del programma non include il codice delle librerie, ma solo riferimenti (_stub_) che indicano dove trovare il codice di libreria. Una volta che questa viene caricata in memoria lo _stub_ viene rimpiazzato con la locazione di memoria in cui si trova la libreria. Con questa metodologia più processi possono utilizzare la stessa istanza del codice di libreria di cui ne viene mantenuta in memoria solo una copia.

Il _linking_ dinamico richiede un intervento essenziale da parte del S.O.

### Swapping

Tecnica secondo la quale un processo può essere rimosso dalla memoria principale (_swapped out_) e spostato in memoria secondaria. In seguito potrà essere ricaricato in memoria principale (_swapped in_) se necessario.

Il tempo necessario per eseguire lo _swap_ e direttamente proporzionale alla quantità di dati da _swappare_. Questa tecnica viene utilizzata in sistemi che supportano la multiprogrammazione in cui è presente _binding dinamico_ per rilocare i processi in posizioni differenti della memoria. Il _binding statico_ imporrebbe di  ricaricare il processo sempre nella stessa posizione.

Lo _swapping standard_ prevede lo spostamento dell'intero processo, ma, per ottimizzare il funzionamento, molti S.O. effettuano swapping limitatamente a porzioni del processo dette _pagine_.

Nei sistemi mobile le tecniche di swap sono più limitate a causa di un hardware meno solido e performante.

### Considerazioni

In presenza di _multiprogrammazione_ non è possibile determinare staticamente la locazione in cui un processo verrà caricato e la presenza di swapping impone l'impiego di rilocazione dinamica.

Nel caso si preveda di caricare interamente il programma in memoria principale vengono utilizzate varie tecniche per la gestione della memoria principale:

- allocazione continua
- paginazione
- segmentazione
- segmentazione paginata

## Tecniche della gestione della memoria

### Allocazione contigua

I processi vengono allocati in posizioni contigue: ogni processo occupa un'unica sezione di memoria di dimensioni pari alla sua dimensione. La memoria viene suddivisa in due partizioni: una per il S.O. e l'altra per i processi utente. Ci sono due varianti a questo approccio: **partizioni fisse** e **partizioni variabili**.

#### Partizioni fisse

Tramite la tecnica delle **partizioni fisse**, la memoria viene suddivisa in partizioni di dimensioni prefissate (anche di grandezze differenti) e ogni processo viene caricato in una partizione in grado di contenerlo interamente.

L'allocazione della memoria avviene utilizzando una coda per partizione tramite cui un processo viene assegnato alla coda della partizione più piccola in grado di contenerlo, oppure una coda singola tramite cui un processo viene allocato nella partizione libero più piccola in grado di contenerlo

In questo caso la **Memory Management Unit** (**MMU**) gestisce di registri base e registri limite:

- **Registro base**: contiene l'indirizzo di partenza della partizione assegnata al processo. Tutti gli _indirizzi logici_ generati dal processo vengono sommati a questo valore per ottenere l'_indirizzo fisico_ in memoria.

- **Registro limite**: specifica la dimensione della partizione assegnata. Ogni accesso alla memoria viene verificato per assicurarsi che l'indirizzo risultante non ecceda questo limite.

Può essere presente un **registro di rilocazione** che, quando sommato, restituisce l'_indirizzo fisico_ del dato in memoria.

**Vantaggi**:

- semplice da implementare
- basso carico per il S.O.

**Svantaggi**:

- grado di multiprogrammazione fissato
- genera _frammentazione interna_ (partizioni più grandi del necessario) e _frammentazione esterna_ (alcune partizioni non vengono utilizzate)

#### Partizioni variabili

Non fissando a priori ne il numero ne le dimensioni delle partizioni è possibile eliminare la frammentazione interna: la memoria viene suddivisa in parti dalle dimensioni corrispondenti alle dimensioni dei processi che vi vengono caricati.

La memoria è vista come un insieme di buchi (sezioni libere) da riempire. Il S.O. mantiene in memoria le informazioni sulle partizioni assegnate e sui buchi disponibili, ogni processo viene allocato in uno dei buchi in grado di contenerlo in base ad una strategia che può essere _first fit_, _best fit_, _worst fit_.

La **MMU** gestisce registri base e registri limite.

**Vantaggi**:

- non ha _frammentazione interna_
- non ha grado di multiprogrammazione prefissato

**Svantaggi**:

- possibile _frammentazione esterna_

Per mitigare il problema della frammentazione esterna è possibile rilocare i processi per compattare lo spazio libero in un unico grande buco, una tecnica utilizzata è quella del **buddy system**. Questa opzione è disponibile solo se è possibile presente la rilocazione dinamica della memoria.

Sia $2^{m}$ il blocco di memoria più piccolo che si vuole allocare e $2^{M}$ la dimensione della di tutta la memoria, allora la memoria viene vista come un insieme di _liste di blocchi_ che possono assumere una dimensione $2^{k}$ con $m \leq k \leq M$.

All'inizio la memoria è vista come un unico blocco di dimensioni $2^{M}$.

Quando giunge una richiesta di dimensione $s$, si cerca un blocco di memoria che sia di dimensione sufficiente a contenerla e che sia una potenza di $2$. Se $s$ è maggiore della metà della dimensione del blocco, allora l'intero blocco viene concesso, altrimenti viene diviso a metà e si verifica, ricorsivamente, che $s$ possa essere contenuto in uno dei blocchi generati:

- se $2^{M-1} < s \leq 2^{M}$ allora viene allocato l'intero blocco
- altrimenti il blocco viene diviso in 2 e si controlla se $2^{M-2} < s \leq 2^{M-1}$

Quando un blocco, di qualsiasi dimensione, viene rilasciato, viene inserito in una lista che contiene tutti i blocchi liberi e se un suo vicino ha la stessa dimensione, i due vengono accorpati: due blocchi adiacenti di dimensione $2^{i}$, vengono uniti in un unico blocco di dimensione $2^{i+1}$.

Questa tecnica presenta il vantaggio di rendere semplice la gestione delle liste di blocchi, ma comporta comunque una certa _frammentazione interna_ poiché non è detto che i processi abbiano esattamente la dimensione di un blocco $2^{i}$.

### Paginazione

Questa tecnica mitiga la _frammentazione esterna_ permettendo ad un processo di non essere allocato in modo contiguo in memoria. La _memoria fisica_ viene organizzata in blocchi di dimensione fissa detti **frame**, mentre la _memoria logica_ viene suddivisa in blocchi detti **pagine** dalla stessa dimensione dei frame.

Solo l'ultima _pagina_ di ogni processo mostra _frammentazione interna_.

Il S.O. tiene traccia dei _frame_ liberi  tramite la **tabella dei frame** e per caricare un processo che occupa $n$ _pagine_ bisogna trovare $n$ _frame_ liberi (anche non contigui o non in ordine). Per tradurre gli indirizzi viene usata una **page table**, unica per ogni processo, che indica per ogni _pagina_ in che _frame_ è contenuta. Un indirizzo logico viene suddiviso in due parti:

- _numero di pagina_: indice nella _page table_ che determina il numero del _frame_ in memoria fisica
- _offset_: determina in che posizione del frame si trova l'_indirizzo fisico_ a cui si vuole fare riferimento.

La gestione dei processi si complica in quanto nel PCB di ogni processo bisogna inserire un puntatore alla propria _page table_. Questo rende il _context switch_ più oneroso e durante lo _swapping_ bisogna tenere conto anche della _tabella dei frame_.

#### Tabella delle pagine

Può essere realizzata tramite:

- **registri dedicati**: efficiente, ma non adatta se la tabella è grande. Rende più complesso lo switch di contesto.
- **tabella in memoria**: localizzata tramite un registro (**PTBR**), impone due accessi alla memoria per accedere ad un singolo dato
- meccanismi di **caching** tramite **TLB** (_Translation Lookaside Buffer_): una tabella che tiene in memoria solo parte delle pagine, per tradurre un indirizzo si fa prima accesso alla _TLB_, se vi è _TLB-miss_ si deve accedere alla memoria secondaria. Durante il context switch la TLB va svuotata. L'efficienza della rilocazione tabella è data dalla _TLB-hit ratio_.

Ogni elemento della tabella delle pagine può avere associati più bit di validità, che indicano lo stato del frame, ad esempio se è stato letto, modificato, eseguito o utilizzato da un processo.

Quando la tabella è di dimensioni elevate, conviene strutturare ulteriormente l'accesso alla memoria tramite:

- paginazione gerarchica
- tabella delle pagine con hashing
- tabella delle pagine invertita

##### Paginazione gerarchica

Tramite la **paginazione gerarchica**, anche la _page table_ viene **paginata** (divisa in pagine). Un esempio a due livelli prevede che la parte "numero di pagina" dell'_indirizzo logico_ venga suddivisa in due porzioni che indicano una la _tabella delle pagine esterna_ e l'altra la pagina della _page table_.

Ad esempio, considerando indirizzi logici da 32 bit con pagine da 4K (4096 bit), l'indirizzo è diviso in:

- 20 bit per numero di pagina ($2^{20}$ pagine disponibili)
- 12 bit di offset all'interno della pagina ($2^{12} = 4096$)

Dato che anche la tabella delle pagine è paginata con pagine da 1K, i 20 bit sono divisi in:

- 10 bit per la outer page table
- 10 bit per l'offset all'interno della pagina della tabella delle pagine

Sono possibili più livelli organizzativi, ma bisogna tener conto che la paginazione gerarchica aumenta il numero di accessi alla memoria per ottenere l'indirizzo fisico.

##### Tabella delle pagine con hashing

Si può utilizzare una **funzione di hash** che traduce la prima parte dell'_indirizzo logico_ in un indice per una tabella di hash che contiene _indirizzi delle pagine fisiche_. In caso di collisioni, viene utilizzata una struttura di concatenamento per gestirle.

##### Tabella delle pagine invertita

La **tabella delle pagine invertita** è un'ottimizzazione rispetto alla normale tabella delle pagine e viene usata per ridurre il'uso di memoria. Nella _page table_ tradizionale ogni pagina logica ha una propria entry, questo può portare a una memoria eccessiva, specialmente se solo una parte delle pagine logiche è effettivamente utilizzata. L' **inverted page table** memorizza solo le pagine fisiche effettivamente utilizzate e ha una sola entry per ogni frame fisico, contenente l'indirizzo logico della pagina e il processo a cui appartiene.

Questa tecnica occupa meno memoria, ma richiede maggior tempo di traduzione in quanto si deve cercare la coppia processo-pagina nella tabella che è ordinata secondo gli _indirizzi fisici_. Per rimediare a questa lentezza è possibile usare hashing.

##### Pagine condivise

Con questa tecnica è possibile mappare diverse _pagine logiche_ nella stessa _pagina fisica_ quando il codice è _rientrante_ (non in grado di modificarsi da solo). In questo modo è possibile condividere più facilmente dati e codice di librerie.

### Segmentazione

La memoria è spesso strutturata in vari **segmenti**, ognuno identificato da un nome, una dimensione e una specifica funzione: main program, heap, stack, libreria, ... Gli indirizzi possono essere specificati attraverso il codice del segmento e l'offset al suo interno.

Il programma è visto come un insieme di _segmenti_ (anche di dimensioni diverse) e ogni segmento è allocato in una porzione di memoria fisica contigua. La traduzione degli indirizzi avviene tramite **segment table** che per ogni segmento contiene:

- base: indirizzo fisico di inizio segmento
- limite: lunghezza segmento

Questa tabella può essere mantenuta in registri dedicati, oppure può essere allocata in memoria e gestita tramite _Segment Table Base Register_ e il _Segment Table Length Register_.

L'allocazione presenta il problema della _frammentazione esterna_ in quanto i vari segmenti possono avere dimensioni diverse, tuttavia la rilocazione dinamica può mitigare, almeno in parte, questo problema.

La condivisione dei segmenti è permessa e, per evitare problemi, è possibile usare un bit di validità per segnalare privilegi e modalità d'accesso.

Mentre la _paginazione_ è trasparente all'utente, la **segmentazione** è visibile in quanto l'utente o il compilatore devono descrivere i segmenti.

### Segmentazione paginata

Lo **spazio logico è segmentato** riflettendo la struttura logica del programma e ogni _segmento_ viene **paginato** secondo le dimensioni del _frame fisico_.

Di ogni processo si gestisce la **segment table** e per ogni segmento è presente una **page table**.

Questo approccio, grazie alle _pagine_, evita la _frammentazione esterna_, ma nell'ultima pagina di ogni segmento è quasi sempre presente un minimo di _frammentazione interna_.
