/* file miaradice.c */
#include <stdio.h> // per le funzioni base di I/O
 #include <stdlib.h> // per la exit()
// prototipi delle funzioni utilizzate:
int SquareRoot(int n);
void stampa(int intero, int radice);
int main() {
    int x,s;
    printf("Scrivi un numero intero: ");
    scanf("%d",&x);
    
    s=SquareRoot(x);

    if (s != -1) {
        stampa(x,s);
    }

    exit(0);
}
