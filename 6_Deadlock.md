# Deadlock

## Il problema del deadlock

Un insieme di processi si trova in **deadlock** se ogni processo dell'insieme è in attesa di un _evento_ che solo un altro processo appartenente allo stesso insieme può provocare.

Quando non esiste un protocollo o delle regole da seguire per competere per la risorsa è possibile che nessun processo possa proseguire in quanto non detiene l'uso esclusivo delle risorse necessarie per portare a termine l'esecuzione.

Le risorse in questione possono essere _logiche_ (buffer, file,...) o _fisiche_ (CPU, ram,...) e possono essere **preemptable** (lo stato può essere ripristinato dopo la sottrazione della risorsa) o **non preemptable** (la sottrazione della risorsa causa perdita di dati).

In una macchina sono presenti diversi tipi di **risorsa** ($R_{1}, R_{2}, R_{m}$) e per ogni risorsa $R_{i}$ esistano $W_{i} \geq 1$ **istanze**.

Per utilizzare una risorsa ogni processo deve adottare un protocollo d'uso: _richiesta_, _utilizzo_, _rilascio_. Se non sono disponibili _istanze_ di risorse, il processo può attenderne la disponibilità per un tempo illimitato o limitato, può fallire/terminare, ...

I problemi insorgono quando sono presenti _attese reciproche_.

## Condizioni necessarie

Perché ci sia un _deadlock_, sono **necessarie** 4 condizioni non indipendenti: 

1. **Mutua esclusione**: devono essere coinvolte _risorse_ utilizzabili da un solo _processo_ alla volta.
2. **Possesso e attesa**: un _processo_ detentore di almeno una _risorsa_ deve essere in _attesa_ di un altra _risorsa_ per proseguire.
3. **Assenza di preemption**: non è possibile sottrarre una risorsa ad un processo che l'ha acquisita.
4. **Attesa circolare**: esiste un insieme di processi in cui ognuno attenda la risorsa di un altro.

## Grado di allocazione delle risorse

Per rappresentare la situazione è utile utilizzare il **grafo di allocazione delle risorse** $G(V,E)$:

- **Vertici $V$**: possono essere di due tipi:
  - di **processo**: $P_{i}$
  - di **risorsa**: $R_{j}$

- **Archi $E$**: connettono risorse e processi, possono essere di due tipi:
  - di **richiesta**: $P_{i} \rightarrow R_{j}$ indica che un processo $P_{i}$ è in attesa di ottenere un istanza della risorsa $R_{j}$
  - di **assegnazione** $R_{j} \rightarrow P_{i}$ indica che una risorsa $R_{j}$ è assegnata al processo $P_{i}$

Se il grafo _non_ contiene cicli, allora _non_ può verificarsi un deadlock. Al contrario, se nel grafo sono presenti dei cicli, il deadlock _potrebbe_ verificarsi. In particolare:

- Se i nodi risorsa coinvolti hanno solo istanze singole, si verifica sicuramente un deadlock.
- Se, invece, le risorse hanno più istanze, la presenza di un ciclo è una condizione necessaria ma non sufficiente per il verificarsi del deadlock.

I concetti di _grafo ciclico_ e _attesa circolare_ non sono equivalenti nell'ambito dei deadlock.

## Osservazioni

La successione e la temporizzazione delle sequenze richiesta/uso/rilascio della risorse è importante: l'occorrenza del deadlock dipende dalla velocità relativa dei due processi. Dato che è il S.O. a effettuare lo scheduling dei processi, può operare in modo da scegliere sequenze sicure che non portano al deadlock, tuttavia questo compito non è semplice in quanto richiede di conoscere in anticipo l'operato dei processi.

## Gestione del deadlock

I principali approcci al problema del deadlock mirano a prevenire il verificarsi del deadlock in maniera statica (**deadlock prevention**), evitare il verificarsi del deadlock in maniera dinamica (**deadlock avoidance**) e a rilevare e risolvere le situazioni di deadlock (**deadlock detection & recovery**). Le prime due cercano di evitare il deadlock, mentre la terza tecnica rileva la presenza di un deadlock e effettua un azione di ripristino.

Alternativamente si può ignorare il problema e confidare che non si verifichi (**ostrich algorithm**). Questa strategia funziona sufficientemente bene per i sistemi commerciali non critici in cui le risorse sono abbondanti, il deadlock è poco frequente e il costo di un eventuale prevenzione/rilevazione non sarebbe giustificato. Nel caso si verifichi una situazione di deadlock si esegue un reboot.

### Prevenire il deadlock

Queste strategie cercano di impedire che _almeno una_ delle condizioni necessarie al deadlock si verifichi.

#### Evitare mutua esclusione

Si esegue una distinzione fra risorse condivisibili e non. Si fa in modo che un processo non aspetti mai per ottenere una risorsa condivisibile.

Dato che molte risorse non sono condivisibili questo approccio spesso non è risolutivo.

#### Evitare possesso e attesa

Si fa in modo che un processo che _detiene_ risorse non ne possa _richiederne_ altre. Per fare ciò ogni processo deve richiedere tutte le risorse che serviranno prima di eseguire un'altra system call e ad ogni processo è permesso di richiedere risorse solo se non ne detiene.

Adottare questo tipo di soluzione porta ad un basso utilizzo delle risorse che tendono ad essere detenute più tempo del necessario. Inoltre può comportare starvation: un processo che chiede molte risorse contemporaneamente deve sperare che si rendano disponibili tutte nello stesso istante.

#### Evitare assenza di preemption

Se un processo richiede una risorsa non disponibile (per la quale dovrebbe attendere), gli si sottraggono tutte le risorse per evitare che la sua attesa blocchi risorse ad altri processi. Quando tutte le risorse richieste saranno disponibili, allora il processo sarà riavviato. Alternativamente le risorse possono essere sottratte al processo che già le detiene per assegnarle al processo richiedente.

Questo tipo di approccio può essere utilizzato solo per risorse _preemptable_.

#### Evitare attesa circolare

Con questa tecnica viene stabilito un ordinamento su tutti i tipi di risorsa esistenti e viene imposto che un processo richieda le risorse secondo una numerazione crescente o in alternativa si richiede che un processo non possa richiedere risorse di ordine $n$ se possiede qualche risorsa di tipo $m$ tale che $m > n$.

I difetti di questo approccio sono che tende a rallentare l'esecuzione sottoutilizzando le risorse e che deve essere il programmatore a sviluppare il programma in modo da richiedere le risorse in modo corretto.

### Evitare il deadlock

Queste tecniche comportano _decisioni dinamiche_ per le quali si presuppone l'avere a disposizione informazioni supplementari sui processi e sulle risorse da loro richieste. Utilizzando queste informazioni si possono applicare algoritmi che individuano le situazioni pericolose che **potrebbero** evolversi in deadlock per poi decidere se concedere o meno le risorse.

Di base si assume di conoscere a priori il numero massimo di richieste di ogni tipo di risorsa (ogni processo dichiara di quante e quali risorse avrà bisogno _al massimo_). Si tiene traccia dello stato del sistema con le metriche di: numero _risorse disponibili_, numero _risorse allocate_ e _massime richieste_ da parte di ogni processo.

Prima di concedere una risorsa il S.O. verifica che il sistema rimanga in uno **stato safe**.

#### Stati safe, unsafe e deadlock

Uno _stato_ è detto **safe** se esiste almeno una _sequenza safe di esecuzione_ di _tutti_ i processi. Se ciò non è possibile allora la lo stato è **unsafe** e _potrebbero_ verificarsi deadlock.

Una _sequenza_ di esecuzione di processi $\langle P_{1}, \dots , P_{n} \rangle$ è safe se per ogni $i$ le risorse che il processo $P_{i}$ può ancora richiedere non superano la somma delle risorse disponibili e di quelle detenute da tutti i processi $P_{j}$ per $j<i$.

Se le richieste di $P_{i}$ non possono essere soddisfatte, allora $P_{i}$ attenderà fino alla terminazione di tutti i processi $P_{j}$ (con $j<i$). $P_{j}$ otterrà le risorse solo quando tutti i $P_{j}$ saranno terminati.

Se il sistema si trova in uno _stato safe_ allora non c'è deadlock, se invece si trova in uno stato _unsafe_ allora _potrebbe esserci_ deadlock.

#### Algoritmo con grafo di assegnazione

Utile nel caso in cui le risorse esistano in istanze singole.

Ogni processo deve dichiarare all'inizio tutte le possibili risorse di cui avrà bisogno.

Si sfrutta il _grafo di allocazione delle risorse_ arricchito con il **claim edge**: un arco $P_{i} \dashrightarrow R_{j}$ indica che il processo $P_{i}$ in futuro potrebbe richiedere la risorsa $R_{j}$. Nel momento in cui la _richiesta_ avviene si trasforma in un _arco di richiesta_ $P_{i} \rightarrow R_{j}$. L'assegnazione avviene normalmente $R_{j} \rightarrow P_{i}$, ma l'arco di assegnamento viene riconvertito nell'arco di reclamo $P_{i} \dashrightarrow R_{j}$.

Una richiesta viene soddisfatta solo se la conversione da arco di reclamo a arco di assegnamento non introduce cicli nel grafo.

#### Algoritmo del banchiere

Utile nel caso di risorse con istanze multiple. I processi devono dichiarare inizialmente quali/quante risorse richiederanno al massimo. Prima di ottenere una risorsa si può verificare un attesa e una volta ottenute le risorse, queste devono essere rilasciate entro un tempo limitato.

##### Strutture dati

Le strutture dati introdotte sono:

- un vettore `available[m]`: `available[j]` indica il numero di istanze della risorsa di tipo `j` disponibili
- la matrice di reclamo `max[n.m]`: `max[i,j]` indica il massimo numero di risorse di tipo `j` che il processo `i` può richiedere
- la matrice `allocation[m,n]`: `allocation[i,j]` indica il numero di risorse di `j` che il processo `i` detiene
- la matrice `need[n,m]`: `need[i,j]` indica il massimo numero di risorse di `j` che il processo `i` _potrebbe_ ancora necessitare. ($need_{(i,j)} = max_{(i,j)} - allocation_{(i,j)}$)

##### Verifica di safeness

Testa la _safeness_ di uno stato.

Vengono definiti due vettori ausiliari `work[m]` e `finish[m]`

- `work[i] := available[i]`
- `finish[i] := false`

1. Ricerca di un processo $P_{i}$ idoneo tale che:

- `finish[i] == false`
- `need[i][j] <= work[j]` ($\forall j \in {1, \dots, m}$)

Se tale processo $P_{i}$ non esiste vai al passo 3
2. Simulazione del completamento:
  
- `work := work + allocation`
- `finish[i] == true`
  
Vai al passo 1
3. Verifica dello stato
Se `finish[i] == true` per ogni `i` allora lo stato è _safe_, altrimenti è _unsafe_.

##### Richiesta risorse

Ogni processo $P_{i}$ ha un vettore `request[i][m]`in cui vengono segnate le richieste che fa. `request[i][j] = k` indica che $P_{i}$ sta richiedendo `k` istanze di $R_{j}$.

Se $P_{i}$ richiede delle risorse il sistema:

- controlla che il processo non richieda più risorse del preventivato: `request[i][k] <= need[i][k]` ($\forall k \in {1, \dots, m}$)
- controlla che le risorse siano disponibili: `request[i][k] <= available[k]` ($\forall k \in {1, \dots, m}$)
- simula l'assegnamento delle risorse a $P_{i}$ modificando
  - `available[k] := available[k]-request[i][k]`
  - `allocation[i][k] := allocation[i][k] + request[i][k]`
  - `need[i][k] := need[i][k] - request[i][k]`
  
A questo punto il sistema determina la _safeness_: se lo stato è safe le risorse vengono effettivamente concesse a $P_{i}$ altrimenti lo stato viene ripristinato a quello precedente (quindi _safe_) e $P_{i}$ deve attendere.

### Rilevare il deadlock

Il sistema è lasciato libero di evolvere e raggiungere, eventualmente, uno stato non sicuro. Vengono poi utilizzati degli algoritmi per individuare la presenza di situazioni di deadlock e riportare il sistema in uno stato safe.

#### Risorse a istanza singola

Viene utilizzata una versione semplificata del grafo di allocazione: il **grafo di attesa**. In questo grafo i nodi sono i processi e vi è arco che collega un processo $P_{i}$ a $P_{j}$ se $P_{i}$ sta aspettando che $P_{j}$ rilasci una risorsa. Quando la risorsa viene rilasciata, l'arco cessa di esistere.

Il S.O. attiva periodicamente un algoritmo di rilevamento cicli (deadlock) che ha costo $O(n^{2})$.

#### Risorse a istanza multipla

In questo caso bisogna gestire lo stato delle varie istanze utilizzando:

- un vettore `available[m]`: `available[j]` indica il numero di istanze della risorsa di tipo `j` disponibili
- la matrice `allocation[m,n]`: `allocation[i,j]` indica il numero di risorse di `j` che il processo `i` detiene
- la matrice `request[n,m]`: `need[i,j]` indica il numero di istanze di `j` che il processo `i` sta richiedendo

Le risorse vengono allocate ottimisticamente ai processi assumendo che:

- non ne chiedano altre in futuro
- quelle richieste siano sufficienti
- le risorse richieste vengano rilasciate se non più utilizzate

Nel caso queste assunzioni vengano falsificate e si verifichi un deadlock, vengono attuate procedure per eliminarlo.

##### Algoritmo di rilevamento

Vengono introdotti due vettori:

- `work[m]:= available[i]`
- `finish[m]`:
  - se `allocation[i] = 0` $\rightarrow$  `finish[i] = true` (il processo è gia completato)
  - altrimenti `finish[i] = false`

1. Trova un processo $P_{i}$ tale che:

- `finish[i] = false` (non completato)
- `request[i][j] <= work[j]`  (tutte le risorse richieste possono essere assegnate)

Se nessun processo soddisfa queste condizioni vai a passo 3
2. Simulazione del completamento:

- `work[j] := work[j] + allocation[i][j]`
- `finish[i] == true`

Vai al passo 1
3. Verifica presenza deadlock

Se `finish[i] == false` per qualche `i` allora c'è un deadlock che coinvolge il processo $P_{i}$

Questo algoritmo ha complessità $O(m \times n^{2})$

#### Quando attivare algoritmo di rilevamento

Per capire quanto spesso attivare l'algoritmo di rilevamento bisogna considerare quanto spesso si verifica un deadlock, quanto sono i processi coinvolti e il costo dell'algoritmo.

Attivarlo troppo spesso causa eccessivo overhead, mentre attivandolo troppo sporadicamente risulta complesso capire quali sono i processi che hanno determinato il deadlock.

#### Tecniche di ripristino

Rilevato un deadlock il sistema può decidere di **terminare**:

- tutti i processi coinvolti: soluzione semplice, ma costosa in quanto devono poi essere riavviati
- un processo alla volta finché la situazione di deadlock persiste: soluzione più costosa in quanto dopo la terminazione di ogni processo bisogna rilevare se il deadlock persiste, inoltre bisogna decidere con che priorità terminare i processi (priorità di classe, mole di lavoro già eseguito, numero di risorse detenute, ...)

Generalmente queste opzioni si evitano in quanto si causa ulteriore overhead al sistema, possibile perdita di lavoro già fatto e possibili inconsistenze delle risorse modificate dal processo.

Una soluzione è fare **prelazione sulle risorse**: viene selezionato un processo _vittima_ a cui viene revocato l'uso di una risorsa. Questo può avvenire tramite _rollback_ il processo viene ripristinato ad uno stato consistente precedente oppure può avvenire tramite terminazione e successivo riavvio.

Adottare questa tecnica significa stabilire una politica per decidere quali risorse sottrarre a quali processi evitando che la vittima sia sempre la stessa (potrebbe causare starvation).

#### Approccio combinato

Per gestire i deadlock i tipi di risorsa vengono partizionati in gruppi (memoria secondaria, risorse per processi, memoria principale, ...) e per evitare attese cicliche fra gruppi viene impiegata la tecnica di _prevenzione_, mentre all'interno di ogni gruppo vengono adottate tecniche ad-hoc anti-deadlock.
