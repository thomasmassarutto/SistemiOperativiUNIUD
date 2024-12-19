# Disk scheduling

## Struttura di un disco

Un disco è formato da **settori**, ovvero le unità più piccole di informazioni che possono essere lette/scritte sul disco. Per efficienza sono raggruppati dal punto di vista logico in **cluster** e un file occupa sempre almeno un _cluster_.

Per accedere ad un settore bisogna specificare la _superficie_, la _traccia_ e il _settore_.

La **struttura logica** di un disco rappresenta il modo in cui i dati sono organizzati e gestiti. Il disco viene visto come un **vettore lineare di blocchi logici** in cui ogni _blocco logico_ è l'unità minima di trasferimento tra il disco e la memoria e generalmente corrisponde a un _settore fisico_ o a un _cluster_. Un _blocco_ (o _cluster_) è l'unità minima di trasferimento.

Il _vettore lineare di blocchi logici_ è mappato sequenzialmente sui _settori_ del disco e la numerazione procede gerarchicamente:

1. **Settori**: segmenti più piccoli all'interno di una _traccia_
2. **Tracce**: ciascun piatto del disco è diviso in _tracce_ circolari
3. **Cilindri**: tracce con lo _stesso raggio_, presenti su _diversi_ piatti, formano un _cilindro_

Il S.O. usa la _vista logica_ del disco per operare di esso.

Una richiesta I/O su disco prevede:

- una sequenza di accessi = sequenza indici del vettore del disco
- tipo di accesso
- indirizzo di memoria destinazione
- quantità di dati da trasferire

## Tempo di accesso

Sono presenti 3 componenti temporali:

- **Seek time**: tempo necessario a spostare la testina sulla _traccia_
- **Latency time**: tempo necessario a posizionare il _settore_ desiderato sotto la testina
- **Transfer time**: tempo necessario al _settore_ per passare sotto la testina, si tratta della lettura vera e propria.

$\text{Tempo di accesso}= \text{SeekTime} + \text{Latency} + \text{Transfer Time}$

### Esempio tempo di accesso

- velocità trasferimento: 40MB/s
- velocità di rotazione: 10000 rpm $\approx$ 166 rps
- rotazione media: $\frac{1}{2}$ traccia
- dimensione blocco: 512 Byte
- $T_{seek}$: 5 ms

$$
\begin{aligned}
    T_{accesso} &= T_{seek} + \frac{rotazione\ media}{velocità\ rotazione} + \frac{dimensione\ blocco}{velocità\ trasferimento}\\
    &= 5_{[ms]} + \frac{\frac{1}{2}}{166_{[rps]}} + \frac{512_{[Byte]}}{40_{[MB/s]}}\\
    &= 5_{[ms]} + 3_{[ms]} + 0.0000125_{[ms]} = 8.0000125_{[ms]}\\
\end{aligned}
$$

Il _seek time_ è il parametro dominante: ottimizzando questo si raggiungono performance migliori.

## Algoritmi di Disk scheduling

Esistono algoritmi di scheduling per i processi che accedono al disco. Dato che il _seek time_ è il parametro dominate, per minimizzare il tempo di accesso totale conviene ridurre il tempo dello spostamento della testina.

Come parametro indice di bontà per l'algoritmo si usa la **banda**: $Banda = \frac{nr \ byte \ trasferiti}{tempo}$.

Negli esempi successivi verrà presa in considerazione una sequenza di accessi esempio:

`98` `183` `37` `122` `14` `124` `65` `67`

In cui inizialmente la testina si trova sulla traccia `53`.

L'intervallo dei valori è `[1,199]`.

### First Come First Served (FCFS)

Tramite **FCFS** le richieste vengono processate nell'ordine d'arrivo. Nella sequenza d'esempio la testina si sposta per 640 tracce.

### Shortest Seek Time First (SSTF)

L'algoritmo di **SSTF** Seleziona la richiesta con il minimo spostamento della testina rispetto alla posizione attuale. Migliora **FCFS**, ma non è ottimo e vi è possibilità di starvation.

`65` `67` `37` `14` `98` `122` `124` `183`

Lo spostamento della testina è di 236 tracce.

### SCAN

Con **SCAN** la testina parte da un estremità del disco e si sposta verso l'altra servendo tutte le richieste correnti. Arrivata nell'altra estremità, riparte in direzione opposta servendo tutte le richieste rimanenti. Viene detto anche _algoritmo dell'ascensore_.

`37` `14` `*0*` `65` `67` `98` `122` `124` `183`

Lo spostamento della testina è di 236 tracce.

### C-SCAN

**C-SCAN** o **Circular Scan** attua la stessa politica di _SCAN_, ma, quando la testina arriva ad un estremità, riparte immediatamente da 0. Il disco viene visto come una lista circolare che rende il tempo di attesa più uniforme rispetto a _SCAN_. L'analogia è quella di uno spalatore di neve.

`65` `67` `98` `122` `124` `183` `*199*` `14` `37`

Lo spostamento della testina è di 382 tracce.

### LOOK, C-LOCK

Gli algoritmi **LOOK** e **C-LOOK** sono varianti di **SCAN** in cui la testina non arriva fino all'estremità opposta, ma si ferma non appena non ci sono più richieste in quella direzione.

Nel caso di **C-LOOK** la sequenza è:

`65` `67` `98` `122` `124` `183` `14` `37`

Lo spostamento della testina è di 322 tracce.

### N-step-SCAN

La coda viene partizionata in più code di lunghezza massima $N$. Quando una coda viene processata gli accessi in arrivo riempiono le altre code che verranno servite in uno _scan_ successivo.

Quando $N$ tende ad un numero molto grande, l'algoritmo degenera in **SCAN**, invece  se $N = 1$, l'algoritmo degenera in **FCFS**.

L'algoritmo **FSCAN** funziona similmente, ma con 2 sole code.

### Last In First Out (LIFO)

Gli accessi vengono schedulati in base all'ordine inverso di arrivo. Questo approccio è utile nel caso di accessi con _elevata località_, molto vicini tra loro, ma soffre i starvation.
