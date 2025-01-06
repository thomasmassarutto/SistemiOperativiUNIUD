/* file miaradice2.c
* questa versione accetta un parametro sulla
* linea di comando.
* Se non ci sono parametri chiede di scriverne uno.
*/
#include <stdio.h>// per le funzioni base di I/O
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> // per la exit()
// prototipi delle funzioni utilizzate:
int SquareRoot(int n);
void stampa(int intero, int radice);

int main(int argc, char *argv[]) {
    int x,s;

    /* Se sulla linea di comando sono forniti due o piu’ parametri: */
    if (argc > 2) {
        printf("Usage: %s [intero]\n", argv[0]);
        printf("Se non vengono forniti parametri");
        printf("%s chiede di digitare un intero\n", argv[0]);

        for (int i = 1; i< argc ;i++){
            char *endptr;
            int nbr= strtol(argv[i], &endptr, 10);
            
            s= SquareRoot(nbr);
            
            if (s != -1) 
                stampa(nbr, s);

        }

        exit(0);
    }
    /* caso di uno o due parametri: */
    if (argc == 1) { /* non c’e’ un parametro: digitare il numero */
        printf("Scrivi un numero intero: ");
        scanf("%d",&x);
    } else {
        /* uso il parametro */
        sscanf(argv[1],"%d",&x);
    }

    /* calcolo: */
    s=SquareRoot(x);

    if (s != -1)
        stampa(x,s);
    exit(0);
}