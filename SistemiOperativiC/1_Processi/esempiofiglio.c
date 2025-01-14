#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    // pid_type
    pid_t processoFiglio;
    pid_t terminato;

    int valoreFiglio; //conterra il valore di ritorno del figlio

    printf("Sono il padre, PID: %d\n", getgid());

    // generazione figlio
    processoFiglio= fork();

    if (processoFiglio < 0){
        printf("Errore generazione figlio");
    }else if(processoFiglio == 0){// se valoreFiglio = 0 -> allora vuol dire che è stato inizializzato
        printf("Sono il figlio, PID: %d, mio padre ha PID: %d\n", getgid(), getppid());

        exit(5);
    }else {// valoreFiglio non e assegnato, sono il padre
        printf("Sono il padre, PID: %d.\nAttendo 3 secondi che mio figlio termini\n", getgid());
        sleep(3);
        
        // a questo punto il figlio dovrebbe aver finito
        terminato = wait(&valoreFiglio);
        if (terminato == -1){// gestione errore del figlio
            printf("Il figlio ha generato un errore\n");
            exit(-1);
        }

        printf("Wait eseguita.\nMio figlio %d ha restituito %d\n",terminato, WEXITSTATUS(valoreFiglio));

    }

    return(EXIT_SUCCESS);
}