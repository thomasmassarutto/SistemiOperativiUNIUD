# Appunti di sistemi operativi

Appunti del corso di sistemi operativi A.A. 2023/2024

## Conversione di un singolo file

Per convertire con Pandoc:

`pandoc -s input.md -o output.pdf --mathml`

Documentazione Pandoc: <https://pandoc.org/MANUAL.html>

Nella repo è presente un file `.yaml` per personalizzare le esportazioni.
`pandoc -s input.md -o output.pdf --metadata-file=config.yaml ----mathml`

## Creazione di un file unico 

Creazione di un file unico:

`pandoc -s 1_Introduzione.md 2_StrutturaDeiSO.md 3.1_Processi.md 3.2_Threads.md 4_CpuScheduling.md 5_Sincronizzazione.md 6_Deadlock.md 7_MemoriaCentrale.md 8_MemoriaVirtuale.md 9_DiskScheduling.md 10_FileSystem.md 10.1RealizzazioneDelFileSystem.md -o SistemiOperativi.pdf --metadata-file=config.yaml --mathml`

Per ora da warning, non capisco in che documento ci deve essere un errore di sintassi $\LaTeX$.

## TODO

- ⚠️ completare esempi in `5_Sincronizzazione.md`
- verificare dove l'esportazione genera warnings
- capire come far funzionare `config.yaml`

## Formati output testati

Questi formati generano un file leggibile:

- `pdf`
  - ⚠️ genera warning alla linea 4232
- `epub` 
  - ⚠️ necessita flag `--mathml` altrimenti i tag \frac{}{} non vengono convertiti
  - ⚠️ necessita del flag --metadata title="...". 
- `docx` 
  - ⚠️ non genera la Table Of Contents