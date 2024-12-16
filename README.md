# Appunti di sistemi operativi

per convertire con pandoc:

`pandoc -s input.md -o output.pdf --mathml`

Documentazione pandoc: <https://pandoc.org/MANUAL.html>

Nella repo è presente un file `.yaml` per personalizzare le esportazioni.
`pandoc -s input.md -o output.pdf --metadata-file=config.yaml`

Per convertire tutti i documenti:

`pandoc -s 1_Introduzione.md 2_StrutturaDeiSO.md 3.1_Processi.md 3.2_Threads.md 4_CpuScheduling.md 5_Sincronizzazione.md 6_Deadlock.md 7_MemoriaCentrale.md 8_MemoriaVirtuale.md 9_DiskScheduling.md 10_FileSystem.md 10.1RealizzazioneDelFileSystem.md -o SistemiOperativi.pdf --metadata-file=config.yaml`

Per ora da warning, non capisco in che documento ci deve essere un errore di sintassi $\LaTeX$.
