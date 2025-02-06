# Appunti e esercizi di sistemi operativi

Appunti e esercizi del corso di sistemi operativi A.A. 2023/2024

## Appunti 

Gli appunti del corso si trovano nella cartella `Appunti`

### Conversione di un singolo file

Per convertire con Pandoc:

`pandoc -s input.md -o output.pdf --mathml`

Documentazione Pandoc: <https://pandoc.org/MANUAL.html>

Nella repo è presente un file `.yaml` per personalizzare le esportazioni.
`pandoc -s input.md -o output.pdf --metadata-file=config.yaml --mathml`

### Creazione di un file unico 

Creazione di un file unico:

`pandoc -s 1_Introduzione.md 2_StrutturaDeiSO.md 3.1_Processi.md 3.2_Threads.md 4_CpuScheduling.md 5_Sincronizzazione.md 6_Deadlock.md 7_MemoriaCentrale.md 8_MemoriaVirtuale.md 9_DiskScheduling.md 10_FileSystem.md 10.1_RealizzazioneDelFileSystem.md ProgrammazioneDiSistema.md -o SistemiOperativi.pdf --metadata-file=config.yaml --mathml`

Sono presenti più file per i metadati, ognuno è ottimizzato per un uso specifico:

- `config.yaml`: file di configurazione basico
- `configPcVersion.yaml`: versione ottimizza l'uso su computer
- `configPrintVersion.yaml`: versione ottimizzata per la stampa e la rilegatura

### TODO

- capire come far funzionare `config.yaml`

### Formati output testati

Questi formati generano un file leggibile:

- `pdf` sono stati aggiunti in testa ad ogni file dei tag `\newline` per andare a capo
- `epub` 
  - ⚠️ necessita flag `--mathml` altrimenti i tag \frac{}{} non vengono visualizzati bene
  - ⚠️ necessita del flag --metadata title="..."
- `docx` 
  - ⚠️ non genera la Table Of Contents

## Esercizi di introduzione a `C`

Gli esercizi di introduzione a `C` sono presenti nella cartella `IntroC`.

## Esercizi di `C` per sistemi operativi

Gli esercizi di `C` per sistemi operativi sono presenti nella cartella `SistemiOperativiC`