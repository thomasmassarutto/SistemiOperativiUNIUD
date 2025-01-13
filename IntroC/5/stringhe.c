#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void insertionSort(char *strings[], int n) {
    for (int i = 1; i < n; i++) {
        char *key = strings[i];
        int j = i - 1;
        
        while (j >= 0 && strlen(strings[j]) > strlen(key)) {
            strings[j + 1] = strings[j];
            j = j - 1;
        }
        
        strings[j + 1] = key;
    }
}


int main(int argc, char **argv){

    int nr_of_args= 3;

    if (argc != nr_of_args +1){
        printf("%d args only!\n", nr_of_args);
        return(EXIT_FAILURE);
    }

   
    insertionSort(&argv[1], nr_of_args);

    /* stampa le stringhe in ordine crescente */
    for (int i = 1; i <= nr_of_args; i++) {
        printf("arg %d: %s\n",i, argv[i]);
    }

    /* concatenazione delle stringhe */ 
    int total_length=0;
    char *allocation;

    for (int i = 1; i <= nr_of_args; i++) {
        total_length= total_length + strlen(argv[i]);
    }

    allocation = (char*) calloc(total_length +1, sizeof(char));

    for (int i = 1; i <= nr_of_args; i++) {
        strcat(allocation, argv[i]);
    }

    printf("Stringhe concatenate: %s\n",allocation);

    free(allocation);

    /* stampa solo le stringhe con lettera maiuscola */
    printf("Argomenti maiuscoli: ");

    for (int i = 1; i<=nr_of_args; i++){
        char first_letter = argv[i][0];

        if (65 <= first_letter && first_letter <= 90){
            printf("%s\t",argv[i]);
        }
    }
    printf("\n");

    return(EXIT_SUCCESS);
}