#include<stdio.h>
#include<stdlib.h>

int main(){
    unsigned char c = 'z' ; // char ha size 8 bit. In z ASCII: 1111010 (usa 7 bit)
    unsigned char d;
    d = c;
    printf("sizeof unsigned char: %ld byte\n", sizeof(unsigned char) * 8);

    printf("c:\t%c -> \t%d -> \t%b\n",c,c,c);

    d= (d & ~(1<<4));   // 1 << 4: shifta a sx i bit di 1 di 4 posizioni: 00000001 -> 00010000
                        // ~ (...): complementa cio' che sta dentro le parentesi: 00010000 -> 11101111
                        // c & ...: esegue maschera and: 01111010 & 11101111 -> 01101010
                        // Nb: ASCII hanno 7 bit quindi 01101010 -> 1101010

    printf("d:\t%c -> \t%d -> \t%b\n",d,d,d);

    return(EXIT_SUCCESS);
}