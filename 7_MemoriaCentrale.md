# Memoria centrale

## Generalità

Condividere la memoria fra più processi è essenziale, ma per farlo bisogna affrontare le seguenti problematiche: allocazione della memoria, protezione dello spazio di indirizzamento, condivisione dello spazio di indirizzamento, gestione dello swap. Nei sistemi moderni si fa spesso uso della _memoria virtuale_.

Per essere eseguito un _programma_ deve essere portato in memoria principale e trasformato in un _processo_. Per fare ciò la CPU deve prelevare dalla memoria le _istruzioni_ da eseguire in base al valore del _program counter_. Queste istruzioni vengono decodificate, eventualmente vengono prelevati degli operandi dalla memoria e al termine dell'esecuzione dell'istruzione, il risultato può essere scritto in memoria. Al termine del processo la sia memoria viene rilasciata.

### Indirizzi e memoria

In questo processo per ogni fase si assume una diversa semantica fra indirizzi di memoria e spazio di memoria, in particolare vi è una distinzione fra **indirizzi logici** e **indirizzi fisici**. Gli indirizzi hanno diverse rappresentazioni nelle varie fasi di costruzione di un programma.

### Binding degli indirizzi

Gli indirizzi nel programma sorgente sono **simbolici**, astratti: non indicano un indirizzo fisico o numerico specifico. Nella fase di _compilazione_ gli indirizzi simbolici vengono associati a **indirizzi rilocabili**: non ancora associati a una posizione fisica precisa nella memoria del sistema, ma rilocati/adattati in base a dove il programma viene effettivamente caricato. Durante la fase di collegamento o caricamento gli _indirizzi rilocabili_ vengono associati a **indirizzi assoluti**.

L'associazione fra _indirizzi simbolici_ e _indirizzi fisici_ è detta **binding** e può avvenire in vari momenti.

- **In compilazione**: avviene **binding statico** al compile time e viene utilizzato se la posizione in memoria centrale è nota a priori, genere un codice assoluto in cui se se vuole collocare il programma da un altra parte, bisogna ricompilare.
- **In caricamento**: avviene **binding statico** al load time in cui si genera in compilazione del codice rilocabile in cui tutti gli indirizzi sono relativi e inizializzati all'inizio del programma, per cambiare la collocazione basta ricaricare il programma.
- **In esecuzione**: avviene **binding dinamico** al run time, in questo caso il binding è posticipato e il programma può essere spostato in posizioni diverse della memoria durante l'esecuzione. Per questa metodologia è richiesto supporto hardware dedicato: **MMU** (_Memory Management Unit_). Ad ogni _indirizzo rilocabile_ viene aggiunto il contenuto di un _registro di rilocazione_ (che funge da offset), con l'indirizzo risultante si accede alla memoria. Ad esempio, se il valore all'interno del _registro di rilocazione_ è $14000$ e la CPU genera l'_indirizzo logico_ $123$, l'indirizzo effettivo per accedere alla memoria è $14000 + 123= 14123$.

Schematicamente la CPU genera a partire dal sorgente indirizzi logici che corrispondono a locazioni di memoria fisica.

### Caricamento statico vs dinamico

Il **caricamento** (trasferimento di codice eseguibile da memoria secondari a primaria) può essere:

- **statico**: l'intero codice viene caricato in memoria
- **dinamico**: i moduli di codice vengono caricati al momento del loro primo utilizzo per risparmiare memoria. Solitamente richiede un intervento minimo da parte del S.O.

### Collegamento statico vs dinamico

Il **collegamento** (associazione dei riferimenti simbolici nel codice alla loro controparte fisica in memoria) piò essere:

- **statico**: tutti i riferimento del codice sono definiti prima dell'avvio dell'esecuzione
- **dinamico**: utilizzato nelle librerie, il _linking_ avviene posticipato a tempo di esecuzione, così il codice del programma non include il codice delle librerie, ma solo riferimenti (_stub_) che indicano dove trovare il codice di libreria. Una volta che la libreria viene caricata in memoria lo _stub_ viene rimpiazzato con la locazione di memoria in cui si trova la libreria. Con questa metodologia piò processi possono utilizzare la stessa istanza del codice di libreria di cui ne viene mantenuta una sola copia in memoria.

Il _linking_ dinamico richiede un intervento essenziale da parte del S.O.

### Swapping

Tecnica secondo la quale un processo può essere rimosso dalla memoria principale _swapped out_ e spostato in memoria secondaria. In seguito potrà essere ricaricato in memoria principale _swapped in_.

Più è grande la quantità di dati da swappare è direttamente proporzionale al tempo necessario, questa tecnica ha senso se viene utilizzata la multiprogrammazione e se è presente _binding dinamico_ per ri locare i processi _swapped in_: se venisse utilizzato con il _binding statico_ si dovrebbe ricaricare il processo sempre nella stessa posizione.

Lo _swapping standard_ sposta l'intero processo, ma per evitare lo swap dell'intero processo, molti S.O. effettuano lo swapping limitatamente a porzioni del processo dette _pagine_.

Nei sistemi mobile bisogna fare i conti con hardware meno solido, quindi le tecniche di swap sono più limitate.

### Considerazioni

In presenza di multiprogrammazione non è possibile fissare staticamente la locazione in cui un processo verrà caricato e la presenza di swapping impone l'impiego di rilocazione dinamica.

Sono necessarie tecniche specifiche per la gestione della memoria principale, nel caso si preveda di _caricare interamente il programma in memoria principale_ vengono utilizzate le tecniche di:

- allocazione continua
- paginazione
- segmentazione
- segmentazione paginata

## Tecniche della gestione della memoria

### Allocazione contigua

I processi vengono allocati in posizioni contigue: ogni processo occupa un'unica sezione di memoria di dimensioni para alla sua dimensione. La memoria viene suddivisa in due partizioni: una per il S.O. e l'altra per i processi utente. Ci sono due variati a questo approccio: **partizioni fisse** e **partizioni variabili**.

#### Partizioni fisse

La memoria viene suddivisa in partizioni di dimensioni prefissate (anche di grandezze differenti) e ogni processo viene caricato in una partizione in grado di contenerlo interamente.

L'allocazione della memoria avviene utilizzando:

- una coda per partizione: un processo viene assegnato alla coda della partizione più piccola in grado di contenerlo.
- coda singola: un processo viene allocato nella partizione libero più piccola in grado di contenerlo

La MMU consiste di registri base e registri limite:

- **Registro base**: contiene l'indirizzo di partenza della partizione assegnata al processo. Tutti gli indirizzi logici generati dal processo vengono sommati a questo valore per ottenere l'indirizzo fisico in memoria.

- **Registro limite**: specifica la dimensione della partizione assegnata. Ogni accesso alla memoria viene verificato per assicurarsi che l'indirizzo risultante non ecceda questo limite.

Può essere presente un **registro di rilocazione** che quando sommato restituisce l'indirizzo fisico del dato in memoria.

**Vantaggi**:

- semplice da implementare
- basso carico per il S.O.

**Svantaggi**:

- grado di multiprogrammazione fissato
- genera _frammentazione interna_ (partizioni più grandi del necessario) e _frammentazione esterna_ (alcune partizioni non vengono utilizzate)

#### Partizioni variabili

Non fissando a priori il numero e le dimensioni delle partizioni è possibile eliminare la frammentazione interna: la memoria viene suddivisa in parti di dimensioni corrispondenti alle dimensioni dei processi che vi vengono caricati.

La memoria è vista come un insieme di buchi (sezioni libere) da riempire. Il S.O. mantiene in memoria le informazioni sulle partizioni assegnate e sui buchi disponibili, ogni processo viene allocato in uno dei buchi in grado di contenerlo in base ad una strategia che può essere _first fit_, _best fit_, _worst fit_.

La MMU consiste di registri base e registri limite.

**Vantaggi**:

- non ha _frammentazione interna_
- non ha grado di multiprogrammazione prefissato

**Svantaggi**:

- possibile _frammentazione esterna_

Per mitigare il problema della frammentazione esterna, quando è presenta la rilocazione dinamica della memoria, è possibile rilocare i processi per compattare lo spazio libero in un unico grande buco. Una tecnica utilizzata è quella del **buddy system**.

Sia $2^{m}$ il blocco di memoria più piccolo che si vuole allocare e $2^{M}$ la dimensione della di tutta la memoria, allora la memoria viene vista come un insieme di liste di _blocchi_ che possono assumere una dimensione potenza di due $2^{k}$ con $m \leq k \leq M$.

All'inizio la memoria è vista come un unico blocco di dimensioni $2^{M}$.

Quando giunge una richiesta di dimensione $s$ si cerca un blocco con dimensione sufficiente, ma potenza di $2$. Se $s$ è maggiore della metà della dimensione del blocco, l'intero blocco viene concesso, altrimenti il blocco viene diviso a metà e si verifica ricorsivamente che $s$ possa essere contenuto nel nuovo blocco generato:

- se $2^{M-1} < s \leq 2^{M}$ allora viene allocato l'intero blocco
- altrimenti il blocco viene diviso in 2 e si controlla se $2^{M-2} < s \leq 2^{M-1}$

Quando un blocco, di qualsiasi dimensione, viene rilasciato, viene inserito in una lista che contiene tutti i blocchi liberi e se un suo vicino ha la stessa dimensione, i blocchi vengono riuniti in un unico blocco: due blocchi adiacenti di dimensione $2^{i}$, vengono uniti in un unico blocco di dimensione $2^{i+1}$.

Questa tecnica ha come vantaggio che scorrere le liste di blocchi sono facili da maneggiare, ma la frammentazione interna resta sempre presente in quanto non è detto che i processi abbiano dimensione $2^{i}$.

### Paginazione

Questa tecnica mitiga la frammentazione esterna permettendo ad un processo di non essere allocato in modo contiguo in memoria. La _memoria fisica_ viene organizzata in blocchi di dimensione fissa detti **frame**, mentre la _memoria logica_ viene suddivisa in blocchi detti **pagine** dalla stessa dimensione dei frame.

Solo l'ultima _pagina_ di ogni processo mostra frammentazione interna.

Il S.O. tiene traccia dei _frame_ liberi  tramite la **tabella dei frame** e per caricare un processo che occupa $n$ pagine si devono trovare $n$ _frame_ liberi (anche non contigui o non in ordine). Per tradurre gli indirizzi viene usata una **page table** unica per ogni processo che indica per ogni _pagina_ in che _frame_ è contenuta. Un indirizzo logico viene suddiviso in due parti:

- _numero di pagina_: indice nella page table che determina il numero del _frame_ in memoria fisica
- _offset_: determina in che posizione del frame si trova l'_indirizzo fisico_ a cui si vuole fare riferimento.

La gestione dei processi si complica in quanto nel PCB di ogni processo bisogna inserire un puntatore alla propria _page table_. Questo rende il _context switch_ più oneroso e durante lo _swapping_ bisogna tenere conto anche della tabella dei frame.

#### Tabella delle pagine

Può essere realizzata tramite:

- registri dedicati: efficiente, ma non adatta se la tabella è grande. Rende più complesso lo switch di contesto.
- tabella in memoria: localizzata tramite un registro (**PTBR**), impone due accessi alla memoria per accedere ad un singolo dato
- utilizzo di meccanismi di caching tramite **TLB**: tabella che tiene in memoria solo parte delle pagine, per tradurre un indirizzo si fa prima accesso alla _TLB_, se vi è _TLB-miss_ si deve accedere alla memoria secondaria. Durante il context switch la TLB va svuotata. L'efficienza della rilocazione tabella è data dalla _TLB-hit ratio_.

Per quanto riguarda la protezione, ad ogni elemento della _page table_, viene associato un bit di validità, inoltre, possono essere presenti altri bit per indicare che un frame è stato letto, modificato, eseguito, ... da parte di un processo.

Quando la tabella è di dimensioni elevate, conviene strutturare ulteriormente l'accesso alla memoria tramite:

- paginazione gerarchica
- tabella delle pagine con hashing
- tabella delle pagine invertita

##### Paginazione gerarchica

Anche la _page table_ viene paginata (divisa in pagine). Con un esempio a due livelli la parte "numero di pagina" dell'indirizzo logico è suddivisa in due porzioni che indicano la tabella delle pagine esterna e la pagina della page table.

Ad esempio, considerando indirizzi logici da 32 bit con pagine da 4K (4096 bit), l'indirizzo è diviso in:

- 20 bit per numero di pagina ($2^{20}$ pagine disponibili)
- 12 bit di offset all'interno della pagina ($2^{12} = 4096$)

Dato che anche la tabella delle pagine è paginata con pagine da 1K,  i 20 bit sono divisi in:

- 10 bit per la outer page table
- 10 bit per l'offset all'interno della pagina della tabella delle pagine

Sono possibili più livelli organizzativi, ma bisogna tener conto che la paginazione gerarchica aumenta il numero di accessi alla memoria per ottenere l'indirizzo fisico.

##### Tabella delle pagine con hashing

Una funzione di hash traduce la prima parte dell'indirizzo logico in una tabella di hash che contiene indirizzi delle pagine fisiche.
In caso di collisioni, viene utilizzata una struttura di concatenamento per gestirle.

##### Tabella delle pagine invertita

La tabella delle pagine deve avere una entry per ogni pagina logica dello spazio di quel processo, anche se non sono state realmente allocate tutte le pagine che potrebbero essere utilizzate. Questo fa si che la dimensione delle tabella delle pagine possa essere troppo grande rispetto ai frame fisici a disposizione.

L'**inverted page table** è una tabella che memorizza solamente le informazioni sulle pagine realmente utilizzate dai processi, è unica per tutto il sistema e dispone di una entry per ogni frame fisico. Questa entry contiene l'indirizzo logico della pagina caricata in quel frame e l'indicazione del processo possessore.

Questa tecnica occupa meno memoria, ma richiede maggior tempo di traduzione in quanto si deve cercare la coppia processo - pagina nella tabella che è ordinata secondo gli indirizzi fisici. Per rimediare a questa lentezza è possibile usare hashing.

##### Pagine condivise

Con questa tecnica è possibile mappare diverse pagine logiche nella stessa pagina fisica quando il codice è _rientrante_ (non in grado di modificarsi da solo). In questo modo è possibile condividere più facilmente dati e codice di librerie.

### Segmentazione

La memoria è spesso strutturata in vari **segmenti**, ognuno identificato da un nome, una dimensione e una specifica funzione: main program, heap, stack, libreria, ... Gli indirizzi possono essere specificati attraverso il codice del segmento e l'offset al suo interno.

IL programma è visto come un insieme di segmenti (anche di dimensioni diverse) e ogni segmento è allocato in una porzione di memoria fisica contigua. La traduzione degli indirizzi avviene tramite **segment table** che per ogni segmento contiene:

- base: indirizzo fisico di inizio segmento
- limite: lunghezza segmento

Questa tabelle può essere mantenuta in registri dedicati, oppure può essere allocata in memoria e gestita tramite _Segment Table Base Register_ e il _Segment Table Length Register_.

L'allocazione presenta il problema della frammentazione esterna in quanto i segmenti hanno dimensioni diverse, tuttavia è presente la rilocazione dinamica che può mitigare in parte questo problema.

La condivisione dei segmenti è permessa e per evitare problemi è possibile usare bit di validità, bit per privilegi e modalità d'accesso.

Mentre la _paginazione_ è trasparente all'utente, la **segmentazione** è visibile in quanto l'utente o il compilatore devono descrivere i segmenti.

### Segmentazione paginata

Lo **spazio logico è segmentato** riflettendo la struttura logica del programma e ogni **segmento viene paginato** secondo le dimensioni del frame fisico.

Di ogni processo si gestisce la segment table ed una page table per ogni segmento.

Questo approccio, grazie alle pagine, evita la frammentazione esterna, ma nell'ultima pagina di ogni segmento è quasi sempre presente un minimo di frammentazione interna.