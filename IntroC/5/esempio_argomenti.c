#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){

    printf("Sono il comando %s\n", argv[0]);

    if ( argc > 1){
        printf("Gli argomenti sono: %d\n", argc-1); // il primo è sempre il nome del programma
        
        for (int i= 1;i<argc;i++){
            printf("Argomento nr %d: %s\n", i, argv[i]);
        }
    }else{
        printf("Non ci sono argomenti\n");
    }


    return(EXIT_SUCCESS);
}