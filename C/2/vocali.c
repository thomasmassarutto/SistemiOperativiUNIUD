#include<stdio.h>
#include <stdlib.h>

int main(){
    char vocali[6];

    vocali[0]='a';
    vocali[1]='e';
    vocali[2]='i';
    vocali[3]='o';
    vocali[4]='u';
    vocali[5]='\0';
    
    printf("Le vocali: %s", vocali);
    
    return(EXIT_SUCCESS);
}
