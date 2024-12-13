# CPU scheduling

## Scheduling dei processi

Risolvere un problema di scheduling consiste nel mappare un certo numero di compiti su un (diverso) numero di esecutori in modo da realizzare gli obiettivi globali del sistema rispettando un prefissato insieme di vincoli. Questo problema deve essere risolto per tutte le risorse del sistema, CPU, device, dischi, ...

In un sistema questo compito è delegato allo scheduler e al dispatcher.

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

Sia lo scheduler che il dispatcher rappresentano dei _bottleneck_ per il S.O.

### Burst

Generalmente un processo alterna sequenze di operazioni di elaborazione (**CPU burst**) a sequenze di operazioni I/O (**I/O burst**). Se un processo è caratterizzato da pochi CPU burst lunghi è detto _CPU-bound_, altrimenti è detto _I/O-bound_.

### Preemptive vs non-preemptive scheduling

Una politica di scheduling _non-preemptive_ prevede che il processo running continui a detenere la CPU fino a quando termina ed è tipico dei sistemi batch.

Una politica di scheduling _preemptive_ prevede invece che un processo running possa essere interrotto a seguito di un evento esterno. Questo tipo di approccio evita che processi poco importanti ostacolino il sistema, ma comporta un overhead di gestione.

Se la preemption è effettuata in modalità kernel potrebbero verificarsi inconsistenze del kernel.

## Algoritmi di scheduling dei processi

### Lessico

- **Starvation**: fenomeno che si verifica quando un processo _P_, pur essendo nella _ready queue_, non ottiene mai l'uso della CPU. Puo essere mitigato tramite **aging**.
- **Aging**: meccanismo per cui un processo migliora la priorità col passare del tempo.
- **Tempo di attesa**: il tempo totale che un processo trascorre nella ready queue in attesa di essere eseguito
- **Tempo di risposta**: l'intervallo di tempo tra l'arrivo del processo nella ready queue e la prima volta che inizia ad essere eseguito
- **Convoy effect**:situazione in cui processi brevi rimangono bloccati in attesa dell'esecuzione di un processo lungo

### First come first served (FCFS)

- **Selezione**: Viene selezionato il processo che è nella _ready queue_ da più tempo.
- **Decisione**: non preemptive

Algoritmo semplice da implementare, non genera _starvation_.

Privilegia i processi CPU bound a scapito dei processi I/O bound. Questo genera il _convoy effect_. Ha un tempo di attesa medio legato all'ordine di arrivo dei burst e può avere un tempo di risposta molto alto.

Per migliorare le performance dei processi I/O bisogna che questi abbiano una priorità superiore.

### Shortest Job First (SJF)

- **Selezione**: Viene selezionato il processo che nella _ready queue_ avrà il prossimo CPU burst più breve.
- **Decisione**: non preemptive

Rappresenta lo scheduling ottimale in quanto realizza il tempo di attesa medio migliore, ma soffre di starvation: i processi con CPU burst più lunghi tendono a rimanere in coda a lungo.

### Shortest Remaining Time First (SRTF)

- **Selezione**: Viene selezionato il processo _P1_ che nella _ready queue_ avrà il prossimo CPU burst più breve, ma se giunge in _ready queue_ un processo _P2_ con durata rimanente inferiore al tempo restante di _P1_, allora _P2_ sottrae la CPU a _P1_.
- **Decisione**: preemptive

Soffre di starvation per quanto riguarda i processi con CPU burst più lunghi.

### Caratteristiche SJF e SRTF

Questi algoritmi richiedono di conoscere in anticipo la durata dei CPU burst di ogni processo: non sempre è possibile.

#### Stima CPU burst

La stima della durata del prossimo CPU burst di un processo avviene in base alla media esponenziale dei CPU burst passati.

$$
\tau_{n+1}= \alpha t_{n} +(1 - \alpha) \tau_{n}
$$

in cui:

- $t_n$: durata del n-esimo CPU burst
- $\tau_{n}$: rappresenta il contributo della storia remota
- $\alpha$: costante compresa fra 0 e 1 che permette di modulare il peso della storia recente/remota

### Highest Response Ratio First (HRRF)

- **Selezione**: Si selezione il processo che ha l'indice Response Ratio $R$ maggiore.
- **Decisione**: non preemptive

$$
R = \frac{t_{a} + t_{s}}{t_{s}}
$$

in cui:

- $t_{a}$: il tempo che il processo ha passato in attesa della CPU: "età del processo"
- $t_{s}$: durata prevista del CPU burst

Con questo metodo i processi con CPU burst lunghi sono sfavoriti, ma man mano che invecchiano la loro priorità aumenta: questo meccanismo è detto **aging**.

Anche questo algoritmo richiede stime sul tempo di utilizzo della CPU.

### Scheduling con priorità

- **Selezione**: Ogni processo ha una priorità e la CPU viene allocata al processo ready con priorità superiore.
- **Decisione**: preemptive o non preemptive

Le priorità sono indici (solitamente interi) influenzati da diversi fattori sia interni che esterni e possono essere **statiche** o **dinamiche**.

Per quanto riguarda le **priorità statiche** queste sono fisse per un processo. Sono facili da implementare e richiedono un overhead minimo di gestione, ma non si adattano a cambiamenti di carico.

Le **priorità dinamiche** permettono maggiore reattività del sistema rispetto a mutazioni di carico, ma introducono un maggiore overhead per la gestione.

### Round Robin (RR)

- **Selezione**: Viene selezionato il processo che è nella _ready queue_ da più tempo.
- **Decisione**: preemptive utilizzando un timer che dura un **quanto** di tempo (**time slice**)

Di primaria importanza è la scelta del quanto di tempo utilizzabile dai processi. Deve essere abbastanza lungo da permettere di gestire _interruzioni_ e _switch di contesto_ (e permettere un minimo di esecuzione...), ma non troppo lungo per evitare tempi morti, specialmente per i processi I/O bound. Sperimentalmente un quanto di tempo è settato per essere maggiore dell'80% delle durate dei CPU burst.

Con questo algoritmo i processi I/O bound sono penalizzati in quanto il loro quanto potrebbe finire prima che abbiano finito l'operazione I/O. I processi CPU bound sfruttano tendenzialmente tutto il quanto di tempo a loro assegnato.

Nonostante questi difetti, questo algoritmo ha una politica _fair_ che evita la _starvation_.

### Scheduling a Code Multiple

Vengono introdotte più code in cui inserire **permanentemente** i processi e ogni coda è gestita con un algoritmo di scheduling opportuno. Si deve stabilire uno scheduling fra code, solitamente viene implementato in maniera **preemptive** assegnando ad ogni coda una priorità, oppure, per evitare _starvation_ si adotta un algoritmo di _time slicing_ a livello di code.

### Scheduling a Code Multiple con Retroazione

L'inserimento di un processo **non è più permanente** in una coda: la migrazione fra code evita _starvation_ introducendo criteri di _aging_. Processi che usano per molto tempo la CPU vengono spostati in code a bassa priorità, mentre quelli I/O bound vengono spostati in code ad alta priorità. Generalmente un processo che attende da molto tempo viene promosso ad una coda ad alta priorità.

In questo caso l'hoverhead di gestione aumenta, ma il sistema è in grado di adattarsi meglio al carico di lavoro.

In fase di progettazione ci sono diversi parametri di considerare:

- numero di code
- quali algoritmi per scheduling interno alle code
- come spostate processo da una coda ad un altra e seguendo che politica
- in che coda inserire un processo appena nato

### Fair Share Scheduling (FSS)

I processi vengono partizionati in _gruppi_ e lo scheduling mira a garantire l'equità di trattamento rispetto ai _gruppi_.

Questo tipo di algoritmo è adatto a sistemi multi utente, quando sono presenti applicazioni multi thread e lo scheduling avviene sui thread.

Può trattare diversamente i processi appartenenti a diversi utenti.

## Scheduling dei thread

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

Sono present due tipologie di multi elaborazione:

- **multielaborazione asimmetrica**: le attività di sistema sono affidate ad un core specifico detto _master server_ che gestisce gli altri processori. Questo però può diventare un bottleneck.
- **multielaborazione simmetrica**: ogni processore ha uno scheduler che attinge alla _ready queue_ che può essere:
  - _di sistema_, quindi comune fra i processori e che necessita di sistemi per evitare _race condition_
  - _per processor_: ogni processore ha una propria ready queue, ma deve essere implementato un sistema in grado di bilanciare il carico fra code di processori diversi

### Scheduling per multiprocessori multithread

Un processore può presentare più core nello stesso chip fisico, in questo caso il S.O. vede ogni core come un processore logico separato.

In casi del genere c'è bisogno di uno scheduling sia a livello hardware dei thread, sia dei thread software.

Il CPU scheduler del S.O. può sfruttare la presenza di multiprocessori con multithreading hardware a livello di core per distribuire i thread software in modo bilanciato.

### Bilanciamento del carico

Per bilanciare i carico si possono adottare due tecniche principali (non mutualmente esclusive):

- **Push migration**: un processo dedicato controlla periodicamente il carico di lavoro dei processori ed esegue in caso il bilanciamento
- **Pull migration**: quando un processore risulta inattivo, preleva in automatico un task da uno dei processori in overload

### Architetture NUMA

Con le architetture ad accesso condiviso della memoria il CPU scheduler deve tener conto che le CPU hanno accesso veloce alla propria memoria, ma accesso lento alla memoria condivisa. Generalmente lo scheduler assegna le task alle CPU in base alla vicinanza della memoria, ma in caso di load balancing questo criterio potrebbe venire meno.

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

Ad ogni processo il sistema assegna una _priorità_ base con lo scopo di partizionare i processi in _bande_ per ottimizzare l'uso dei devices di I/O. Le bande sono: Swapper, Block
I/O device control, File manipulation, Character I/O device control, User processes.

La priorità viene "aggiustata" all'interno della banda di appartenenza rispetto all'uso della CPU e alle preferenze dell'utente. Tutte le priorità vengono ricalcolate ogni secondo.

### Scheduling O(1) di Linux

Questo è uno scheduling con preemption basato sulle priorità. Ad ogni processo viene assegnato un valore di priorità appartenete a due gamme: _real time_ con i valori da 0 a 99 e _nice_ con i valori da 100 a 140. La durata di tempo CPU concessa dipende dalla priorità: una priorità numerica bassa ha una priorità relativa alta nel sistema e quindi processi del genere avranno a disposizione quanti di tempo più lunghi.

Un task _ready_ viene classificato come **eseguibile** se durante l'ultima esecuzione non ha consumato tutto il tempo di CPU assegnatogli, **scaduto** altrimenti. Un task scaduto diventa tale se esaurisce il suo quanto di tempo e non può essere rieseguito se vi sono altri task classificati come **eseguibili**.

Il sistema gestisce due array: uno contenente i processi _ready scaduti_ e uno per i processi _ready eseguibili_. La CPU viene assegnata al task _ready eseguibile_ con la priorità massima, se questo utilizza tutto il quanto di tempo assegnatogli viene spostato nell'array dei _ready scaduti_. Quando l'ultimo task _ready eseguibile_ diventa scaduto gli array si scambiano i ruoli.

Questo tipo di scheduling è stato usato fino al kernel 2.6.23 quando è stato sostituito dal _Completely Fair Scheduling_.
