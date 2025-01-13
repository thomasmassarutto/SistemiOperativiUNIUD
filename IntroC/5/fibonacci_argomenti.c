#include<stdio.h>
#include<stdlib.h>

#define MAX 32

void fibonacci(int x, long int *array){
    *array =0;
    *(array +1)=1;

    long int* i= array +2;
    long int count = 2;

    while (count <= x ){
        long int a= *(i -2);
        long int b= *(i -1);
        *i= a+b;
        count ++;
        i++;
    }
}

/*  IDEA:
*   Calcola Fibonacci solo del numero più grande.
*   I numeri intermedi sono sottoproblemi del numero più grande.
*/

int main(int argc, char **argv){

    printf("Sono il comando %s\n", argv[0]);

    // allocazione dinamica della memoria
    long int *allocation;

    long int max_arg=0;

    if ( argc > 1){
        char *endptr;
        long int temp_arg_value;

        for (int i= 1;i<argc;i++){
            temp_arg_value = strtol(argv[i], &endptr, 10);

            if (*endptr != '\0'){ // controllo che non sia una stringa con char
                printf("Argv %d not a number\n", i);
                return(EXIT_FAILURE);
            }

            if (temp_arg_value > MAX){// controllo che non vada oltre il max
                printf("Argv %d > 32\n", i);
                return(EXIT_FAILURE);
            }

            if (temp_arg_value < 0){// controllo che non sia negativo
                printf("Argv %d < 0\n", i);
                return(EXIT_FAILURE);
            }

            if (temp_arg_value > max_arg){// aggiorna il max
                max_arg= temp_arg_value;
            }         
        }

        // allocazione memoria
        allocation = (long int*) calloc(temp_arg_value, sizeof(long int)); 

        // calcola fibonacci
        fibonacci(max_arg, allocation);

        for (int i = 1; i<argc; i++){
            temp_arg_value = strtol(argv[i], &endptr, 10);

            printf("F(%ld) -> %ld\n", temp_arg_value, *(allocation + temp_arg_value)); // allocation[temp_arg_value]
        }

        free(allocation);

    }else{
        printf("Non ci sono argomenti\n");
    }


    return(EXIT_SUCCESS);
}