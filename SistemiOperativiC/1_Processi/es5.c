#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){

    char comando[16];
    char flag[256];

    char stopstring[]= "basta";

    pid_t processoFiglio;
    pid_t terminato;

    int valoreFiglio;

    do{
    
    printf("comando:");
    scanf("%s", comando);

    if (strcmp(comando, stopstring) == 0){
        break;
    }

    printf("flag:");
    scanf("%s", flag);

    processoFiglio= fork();

    if (processoFiglio < 0) {
        perror("Err: no figlio");
    }else if(processoFiglio == 0){// sono il figlio

        printf("comando: %s\n", comando);
        printf("flag: %s\n", flag);

        char *args[] = {comando, flag, NULL};

        int syscallresult= execvp(comando, args);

        exit(syscallresult);
    }else{// sono il padre
        terminato = wait(&valoreFiglio);
    }

    }while(1);

    return(EXIT_SUCCESS);
}