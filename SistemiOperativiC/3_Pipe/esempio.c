#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Questo programma simula un processo padre che comunica con
*  un processo figlio tramite una pipe
*/

int main(int argc, char *argv[]) {

    // la pipe è vista come un array di due puntatori
    int pipefd[2]; // 0: lettura, 1: scrittura
    pid_t cpid;
    char buf;

    /*controlla che ci siano 2 argomenti*/
    assert(argc == 2);

    /*crea la pipe*/

    if (pipe(pipefd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    /*crea un child*/
    cpid= fork();
    if (cpid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /*UTILIZZO DELLA PIPE*/

    if (cpid==0){// se è il figlio
        close(pipefd[1]); // deve solo leggere, chiude la pipe in scrittura

        printf("Il figlio: aspetta che il padre riempia la pipe\n");
        /*legge li file e lo scrive a schermo*/
        while(read(pipefd[0], &buf, 1)>0){// legge un bite alla volta dall'estremo della pipe
                                          // finche la pipe non viene chiusa 
            write(STDOUT_FILENO, &buf, 1); // stampa il bite letto a terminale...
        }
        //la pipe e stata chiusa dal padre

        write(STDOUT_FILENO, "\n", 1);
        printf("Il figlio ha smesso di parlare\n");
        exit(EXIT_SUCCESS);

    }else{// se e' il parent
        close(pipefd[0]); // chiude l'estremo di lettura, deve solo scrivere nella pipe
        printf("Il padre dorme...\n");
        sleep(5);
        printf("Il padre si sveglia\n");
        /*scrive sull'estremo della pipe qualcosa per il figlio*/
        printf("Il padre: scrive su pipe\n");
        int argv1_length= strlen(argv[1]); // calcola quanto è lungo il messaggio per la write()...
        write(pipefd[1], argv[1], argv1_length);// scrive sull'estremo della pipe

        // chiude la pipe
        printf("Il padre: chiude la pipe\n");
        close(pipefd[1]); // chiude la pipe cosi il figlio smette di leggere

        // aspetta il figlio per chiuderlo
        printf("Il padre attende il figlio\n");
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}

