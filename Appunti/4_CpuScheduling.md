# CPU scheduling

## Scheduling dei processi

Risolvere un problema di scheduling consiste nel mappare un certo numero di compiti su un (diverso) numero di esecutori in modo da realizzare gli obiettivi globali del sistema rispettando un prefissato insieme di vincoli. Questo problema deve essere risolto per tutte le risorse del sistema, CPU, device, dischi, ...

In un sistema questo compito è delegato allo _scheduler_ e al _dispatcher_.

### Scheduler e dispatcher

Lo **scheduler** sceglie quale processo eseguire e successivamente il **dispatcher** effettua il context switch, inizializza il Program Counter ed effettua il passaggio alla modalità utente. Il tempo impiegato per fare ciò è detto **latenza di dispatch**.

Lo scheduler della CPU si attiva quando avvengono eventi che  possono causare uno switch di contesto come scadenza di timer, interruzioni I/O, invocazioni system call, segnali, ...

### Obiettivi

Gli obiettivi da ottimizzare sono spesso antitetici:

- lunghezza dello scheduler
- tempo massimo di risposta
- tempo medio di completamento
- tempo medio di risposta
- tempo di attesa
- numero minimo di CPU in utilizzo
- produttività (throughput)
- utilizzo massimo della CPU
- massimo ritardo rispetto a deadline

### Criteri

Dal punto di vista dell'utente si vuole minimizzare il tempo di risposta, il tempo di completamento e rispettare le deadline.

Dal punto di vista del sistema si vuole massimizzare l'utilizzo della CPU, garantire _fairness_, massimizzare il throughput e in generale rispettare gli obiettivi.

Sia lo _scheduler_ che il _dispatcher_ rappresentano dei _bottleneck_ per il S.O.

### Burst

Generalmente un processo alterna sequenze di operazioni di elaborazione (**CPU burst**) a sequenze di operazioni I/O (**I/O burst**). Se un processo è caratterizzato da pochi _CPU burst_ lunghi è detto _CPU-bound_, altrimenti è detto _I/O-bound_.

### Preemptive vs non-preemptive scheduling

Una politica di scheduling _non-preemptive_ prevede che il processo running continui a detenere la CPU fino a quando termina ed è tipico dei sistemi batch.

Una politica di scheduling _preemptive_ prevede invece che un processo running possa essere interrotto a seguito di un evento esterno. Questo tipo di approccio evita che processi poco importanti ostacolino il sistema, ma comporta un overhead di gestione.

Se la _preemption_ è effettuata in modalità kernel potrebbero verificarsi inconsistenze del kernel.

## Algoritmi di scheduling dei processi

### Lessico

- **Starvation**: fenomeno che si verifica quando un processo _P_, pur essendo nella _ready queue_, non ottiene mai l'uso della CPU. Puo essere mitigato tramite **aging**.
- **Aging**: meccanismo per cui un processo migliora la priorità di diventare _running_ col passare del tempo.
- **Tempo di attesa**: il tempo totale che un processo trascorre nella _ready queue_ in attesa di essere eseguito
- **Tempo di risposta**: l'intervallo di tempo tra l'arrivo del processo nella _ready queue_ e la prima volta che inizia ad essere eseguito
- **Convoy effect**: situazione in cui molti processi brevi rimangono bloccati uno dopo l'altro in attesa dell'esecuzione di un processo lungo

### First come first served (FCFS)

- **Selezione**: Viene selezionato il processo che è nella _ready queue_ da più tempo.
- **Decisione**: _non preemptive_

Algoritmo semplice da implementare, non genera _starvation_.

Privilegia i processi CPU bound a scapito dei processi I/O bound e questo genera il _convoy effect_. Ha un _tempo di attesa_ medio legato all'ordine di arrivo dei _burst_ e può avere un _tempo di risposta_ molto alto.

Per migliorare le performance dei processi I/O bisogna che questi abbiano una priorità superiore.

### Shortest Job First (SJF)

- **Selezione**: Viene selezionato il processo che nella _ready queue_ avrà il prossimo _CPU burst_ più breve.
- **Decisione**: _non preemptive_

Rappresenta lo scheduling ottimale in quanto realizza il _tempo di attesa_ medio migliore, ma soffre di _starvation_: i processi con _CPU burst_ più lunghi tendono a rimanere in coda a lungo.

### Shortest Remaining Time First (SRTF)

- **Selezione**: Viene selezionato il processo $P1$ che nella _ready queue_ avrà il prossimo CPU burst più breve, ma se giunge in _ready queue_ un processo $P2$ con durata rimanente inferiore al tempo restante di $P1$, allora $P2$ sottrae la CPU a $P1$.
- **Decisione**: _preemptive_

Soffre di _starvation_ per quanto riguarda i processi con _CPU burst_ più lunghi.

### Caratteristiche SJF e SRTF

Questi algoritmi richiedono di conoscere in anticipo la durata dei _CPU burst_ di ogni processo, ma ciò non sempre è possibile.

#### Stima CPU burst

La stima della durata del prossimo _CPU burst_ di un processo avviene in base alla media esponenziale dei _CPU burst_ passati.

$$
\tau_{n+1}= \alpha t_{n} +(1 - \alpha) \tau_{n}
$$

in cui:

- $t_n$: durata del n-esimo _CPU burst_
- $\tau_{n}$: rappresenta il contributo della storia remota
- $\alpha$: costante compresa fra 0 e 1 che permette di modulare il peso della storia recente/remota

### Highest Response Ratio First (HRRF)

- **Selezione**: Si selezione il processo che ha l'indice _response ratio_ $R$ maggiore.
- **Decisione**: _non preemptive_

$$
R = \frac{t_{a} + t_{s}}{t_{s}}
$$

in cui:

- $t_{a}$: il tempo che il processo ha passato in attesa della CPU: "età del processo"
- $t_{s}$: durata prevista del _CPU burst_

Con questo metodo i processi con _CPU burst_ lunghi sono sfavoriti, ma man mano che invecchiano la loro priorità aumenta: questo meccanismo è detto **aging**.

Anche questo algoritmo richiede stime sul tempo di utilizzo della CPU che però non sempre sono affidabili o corrette.

### Scheduling con priorità

- **Selezione**: Ogni processo ha una priorità e la CPU viene allocata al processo _ready_ con priorità superiore.
- **Decisione**: _preemptive_ o _non preemptive_

Le priorità sono indici (solitamente interi) influenzati da diversi fattori sia interni che esterni e possono essere **statiche** o **dinamiche**.

Per quanto riguarda le **priorità statiche** queste sono fisse per un processo. Sono facili da implementare e richiedono un overhead minimo di gestione, ma non si adattano a cambiamenti di carico.

Le **priorità dinamiche** permettono maggiore reattività del sistema rispetto a mutazioni di carico, ma introducono un maggiore overhead per la gestione.

### Round Robin (RR)

- **Selezione**: Viene selezionato il processo che è nella _ready queue_ da più tempo.
- **Decisione**: _preemptive_ utilizzando un timer che dura un **quanto di tempo** (**time slice**)

Di primaria importanza è la scelta del _quanto di tempo_ utilizzabile dai processi. Deve essere abbastanza lungo da permettere di gestire _interruzioni_ e _switch di contesto_ (e permettere un minimo di esecuzione...), ma non troppo lungo per evitare tempi morti, specialmente per i processi I/O bound. Sperimentalmente un _quanto di tempo_ è settato per essere maggiore dell'80% delle durate dei _CPU burst_.

Con questo algoritmo i processi _I/O bound_ sono penalizzati in quanto il loro slot temporale potrebbe finire prima che abbiano finito l'operazione I/O. I processi _CPU bound_ sfruttano tendenzialmente tutto il tempo a loro assegnato.

Nonostante questi difetti, questo algoritmo ha una politica _fair_ che evita la _starvation_.

### Scheduling a Code Multiple

Vengono introdotte più code in cui inserire **permanentemente** i processi e ogni coda è gestita con un algoritmo di scheduling opportuno. Si deve poi stabilire uno scheduling fra code, solitamente viene implementato in maniera **preemptive** assegnando ad ogni coda una priorità, oppure, per evitare _starvation_, si adotta un algoritmo di _time slicing_ a livello di code.

### Scheduling a Code Multiple con Retroazione

L'inserimento di un processo **non è più permanente** in una coda: la migrazione fra code evita _starvation_ introducendo una meccanica di _aging_. Processi che usano per molto tempo la CPU vengono spostati in code a bassa priorità, mentre quelli _I/O bound_ vengono spostati in code ad alta priorità. Generalmente un processo che attende da molto tempo viene promosso ad una coda ad alta priorità.

In questo caso l'hoverhead di gestione aumenta, ma il sistema è in grado di adattarsi meglio al carico di lavoro.

In fase di progettazione ci sono diversi parametri di considerare:

- numero di code
- quali algoritmi per scheduling usare all'interno delle code
- come spostare un processo da una coda ad un altra e con che politica
- in che coda inserire un processo appena nato

### Fair Share Scheduling (FSS)

I processi vengono partizionati in _gruppi_ e lo scheduling mira a garantire l'equità di trattamento rispetto ai _gruppi_.

Questo tipo di algoritmo è adatto a sistemi multi utente, quando sono presenti applicazioni multi thread e lo scheduling avviene sui thread.

Può trattare diversamente i processi appartenenti a diversi utenti.

## Scheduling dei thread

Esistono due tipologie di thread su cui è possibile fare scheduling:

- **ULT**: _User Level Thread_
- **KLT**: _Kernel Level Thread_

Gli _ULT_ sono gestiti da apposite librerie che intervengono per stabilire l'associazione tra ULT e KLT. La competizione  fra thread è interna, ristretta all'ambito del processo: si tratta di **process contention scope**.

I _KLT_ sono schedulati direttamente dal kernel e in questo caso si parla di **system contention scope**.

Gli algoritmi utilizzati per schedulare thread sono simili a quelli utilizzati per schedulare processi.

## Scheduling per sistemi multiprocessore

Per _sistema multiprocessore_ si intende un sistema che può avere varie architetture:

- CPU multicore
- Core multithread
- Architetture NUMA (Non Uniform Memory access: più CPU a memoria condivisa)
- Sistemi multiprocessore

Quando sono presenti più unità di elaborazione è possibile utilizzare le tecniche di scheduling viste per il caso di processori singoli, tuttavia è necessario bilanciare il carico fra le varie unità di elaborazione.

### Scheduling per multiprocessori

Sono presenti due tipologie di multi elaborazione:

- **multielaborazione asimmetrica**: le attività di sistema sono affidate ad un core specifico detto _master server_ che gestisce gli altri processori. Questo però può diventare un bottleneck.
- **multielaborazione simmetrica**: ogni processore ha uno _scheduler_ che attinge alla _ready queue_ che può essere:
  - _di sistema_, quindi comune fra i processori e che necessita di sistemi per evitare _race condition_
  - _per processor_: ogni processore ha una propria ready queue, ma deve essere implementato un sistema in grado di bilanciare il carico fra code di processori diversi

### Scheduling per multiprocessori multithread

Un processore può presentare più core nello stesso chip fisico, in questo caso il S.O. vede ogni core come un processore logico separato.

In casi del genere c'è bisogno di uno scheduling sia a livello hardware dei thread, sia dei thread software.

Il _CPU scheduler_ del S.O. può sfruttare la presenza di multiprocessori con multithreading hardware a livello di core per distribuire i thread software in modo bilanciato.

### Bilanciamento del carico

Per bilanciare i carico si possono adottare due tecniche principali (non mutualmente esclusive):

- **Push migration**: un processo dedicato controlla periodicamente il carico di lavoro dei processori ed esegue in caso il bilanciamento
- **Pull migration**: quando un processore risulta inattivo, preleva in automatico un task da uno dei processori in overload

### Architetture NUMA

Con le architetture ad accesso condiviso della memoria il _CPU scheduler_ deve tener conto che le CPU hanno accesso veloce alla propria memoria, ma accesso lento alla memoria condivisa. Generalmente lo scheduler assegna le task alle CPU in base alla vicinanza della memoria, ma in caso di load balancing questo criterio potrebbe venire meno.

### Sistemi eterogenei

Alcuni processori hanno diverse tipologie di core al loro interno, ad esempio core ad alte prestazioni e core a risparmio energetico.

Se lo scheduler conosce l'architettura sottostante, può schedulare i task urgenti sui core veloci e i task meno urgenti sui core a basso consumo.

## Scheduling real time (cenni)

L'obiettivo dei sistemi real time è quello di rispettare le scadenze e i vincoli di reazione. Esistono principalmente due tipologie:

- **real time soft**: i task critici hanno la precedenza, ma si possono violare le scadenze
- **real time hard**: tutti i task devono necessariamente rispettare le scadenze

I sistemi realtime sono solitamente guidati da eventi a cui devono rispondere con minor _latenza_ possibile. Ci sono due tipi di latenze:

- **latenza di interruzione**: tempo trascorso tra la ricezione dell'interruzione e l'inizio della procedura di gestione
- **latenza di dispatch**: tempo impiegato dal dispatcher per eseguire lo switch di contesto

### Algoritmi per sistemi real time

- **Priorità**: scheduling con prelazione il cui scopo è garantire deadline hard e gestire task periodici
- **priorità proporzionale a frequenza**: come lo scheduling a priorità, ma assegna le priorità statisticamente in base alla frequenza del task periodico
- **Earliest deadline first (EDF)**: il task con scadenza più vicina ha la priorità
- **A quote proporzionali**: ogni applicazione viene assegnata una percentuale del tempo di CPU e ad ogni richiesta di utilizzo della CPU si valuta se ammetterla o meno

## Scheduling in UNIX e Linux (cenni)

### Scheduling UNIX tradizionale

Lo scheduling tradizionale di UNIX prevede l'esistenza di _code multiple_ con _priorità dinamiche_. Ogni coda è gestita secondo _round robin_ in cui ad un processo viene eseguita _preemption_ se utilizza la CPU per 1 secondo continuato.

Ad ogni processo il sistema assegna una _priorità_ base con lo scopo di partizionare i processi in _bande_ per ottimizzare l'uso dei devices di I/O. Le bande sono: Swapper, Block, I/O device control, File manipulation, Character I/O device control, User processes.

La priorità viene "aggiustata" all'interno della _banda_ di appartenenza rispetto all'uso della CPU e alle preferenze dell'utente. Tutte le priorità vengono ricalcolate ogni secondo.

### Scheduling O(1) di Linux

Questo è uno scheduling con _preemption_ basato sulle priorità. Ad ogni processo viene assegnato un valore di priorità appartenete a due gamme: _real time_ con i valori da 0 a 99 e _nice_ con i valori da 100 a 140. La durata di tempo CPU concessa dipende dalla priorità: una priorità numerica bassa ha una priorità relativa alta nel sistema e quindi processi del genere avranno a disposizione quanti di tempo più lunghi.

Un task _ready_ viene classificato come **eseguibile** se durante l'ultima esecuzione non ha consumato tutto il tempo di CPU assegnatogli, **scaduto** altrimenti. Un task scaduto diventa tale se esaurisce il suo _quanto di tempo_ e non può essere rieseguito se vi sono altri task classificati come **eseguibili**.

Il sistema gestisce due array: uno contenente i processi _ready scaduti_ e uno per i processi _ready eseguibili_. La CPU viene assegnata al task _ready eseguibile_ con la priorità massima, se questo utilizza tutto il quanto di tempo assegnatogli viene spostato nell'array dei _ready scaduti_. Quando l'ultimo task _ready eseguibile_ diventa scaduto gli array si scambiano i ruoli.

Questo tipo di scheduling è stato usato fino al kernel 2.6.23 quando è stato sostituito dal _Completely Fair Scheduling_.

\pagebreak
