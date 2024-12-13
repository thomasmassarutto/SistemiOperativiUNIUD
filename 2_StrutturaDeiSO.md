# Struttura e servizi dei sistemi operativi

## Servizi offerti dal sistema operativo

Il S.O. offre dei servizi di base quali:

- interfaccia utente (tramite interfaccia grafica o interpretando comandi testuali forniti alla shell)
- esecuzione di programmi
- operazioni I/O
- gestione del file system
- comunicazioni fra processi
- rilevamento e gestione errori
- assegnazione delle risorse
- protezione e sicurezza

## Le system call

Sono l'interfaccia fra i processi e il S.O., ad ognuna di esse è associato un numero che viene utilizzato come identificatore per determinare il codice kernel da eseguire. Solitamente il compilatore di un linguaggio include una _runtime support interface_, ovvero l'interfaccia che invoca le system call e, una volta eseguite, restituisce il return status e i return values. In questo modo i dettagli implementativi sono nascosti al chiamante.

Le system call eseguono in modalità kernel, ci sono 3 metodi per passar loro parametri.

- tramite registri
- tramite memoria
- tramite stack

### Passaggio tramite memoria

Si imposta l'indirizzo dei parametri in un registro e si invoca la system call. Questa deve controllare la legittimità dell'indirizzo che contiene i parametri e che si trova solitamente nello spazio utente.

### Passaggio parametri sullo stack

Si imposta i parametri per la system call sullo stack e si genera una trap. Il S.O. passa in modalità kernel per eseguire la system call e una volta eseguita ritorna in modalità utente.

### Tipi di system call

- controllo processi: creazione/terminazione, allocazione/deallocazione memoria
- gestione file
- gestione dispositivi: allocazione e rilascio
- accesso a informazioni di sistema
- comunicazione: creazione e chiusura di connessioni, invio/ricezione messaggi

### Programmi di sistema

Spesso i **programmi di sistema** forniscono un ambiente per lo sviluppo/esecuzione di programmi utente.

## Struttura di un sistema operativo

### Struttura monolitica

Tutte le componenti del S.O. risiedono nel kernel e sono implementate da procedure in grado di comunicare in maniera diretta le une con le altre.

Questa struttura massimizza l'efficienza, ma la progettazione, realizzazione e il debugging diventano complessi. Dato che tutte le componenti operano in modalità kernel, il sistema diventa vulnerabile a errori, malfunzionamenti e attacchi.

### Struttura stratificata

Il S.O. è stratificato a livelli che offrono funzionalità omogenee. Ogni livello dispone di in interfaccia con cui i livelli dello strato superiore possono interagire. Ogni livello può invocare le funzionalità dei livelli inferiori.

Questa configurazione predilige la modularità delle parti, ma le performance degradano a causa dell'uso forzato delle interfacce.

### Struttura a microkernel

In questi sistemi il kernel è reso il più essenziale possibile e include memory management di basso livello, primitive di sincronizzazione e comunicazione inter processo. Le altre funzionalità sono mantenute separate per garantire modularità e portabilità.

Sistemi del genere offrono una maggiore sicurezza, ma le interfacce fra componenti ne degradano le prestazioni.

### Struttura modulare

Questo genere di sistemi adottano una filosofia object oriented in cui il kernel implementa le funzionalità di base, mentre dei moduli esterni forniscono tutte le altre funzionalità.

I moduli possono comunicare l'uno con l'altro e solo le funzionalità essenziali sono accessibili in modo protetto.

### Sistemi operativi in uso

Nella realtà si adottano soluzioni ibride:

- Linux e Windows presentano una struttura monolitica "arricchita" da moduli.
- Mac OS, iOS combinano microkernel, stratificata e modulare
- Android è simile a iOS, ma le applicazioni eseguono in macchina virtuale Java

## Macchine virtuali (V.M.)

Applicazioni che emulano le funzionalità di un sistema.

Le V.M. forniscono in un interfaccia identica alla macchina fisica di cui ne utilizzano (parte delle) risorse. Il S.O. ha il compito di gestire le V.M. in maniera separata, ognuna con il proprio hardware virtuale.

I vantaggi sono l'uso di hardware comune protetto grazie alle proprietà di isolamento delle V.M.
