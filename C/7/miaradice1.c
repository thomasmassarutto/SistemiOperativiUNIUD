/*file miaradice1.c */
#include <stdio.h>// per le funzioni base di I/O
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> // per la exit()
// prototipi delle funzioni utilizzate:
int SquareRoot(int n);
void stampa(int intero, int radice);

int main(int argc, char *argv[]) {
    int x,s;
    printf("Sono il comando %s\n", argv[0]);
    printf("Scrivi un numero intero: ");
    scanf("%d",&x);
    s=SquareRoot(x);
    if (s != -1) {
        stampa(x,s);
    }
    exit(0);
}