
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/wait.h>

#define PERMISSIONS 0666
#define MAXMESSAGE 30

int main(){
    printf("Hello, world!\n");
    
    char buffer[MAXMESSAGE]; // tanto poi il padre stampa solo 4...
    char *punt1;
    int shmid;
    pid_t cpid;

    // creo un blocco di memoria per contenere il buffer
    shmid = shmget(IPC_PRIVATE, sizeof(buffer[MAXMESSAGE]), PERMISSIONS);

    if (shmid < 0){
        perror("no shared memory");
        exit(1);
    }

    // lego il blocco di memoria al puntatore
    punt1 = (char *)shmat(shmid, NULL, 0);

    if ( punt1 == NULL){
        perror("no punt mem");
        exit(1);
    }

    cpid= fork();
    if (cpid < 0){
        perror("child error");
        exit(1);
    }

    if (cpid == 0){
        printf("Sono il figlio\n");
        char message[MAXMESSAGE] = "ciao dal figlio";

        // scrive in memoria condivisa
        int i= 0;
        while (message[i] != '\0'){
            punt1[i]= message[i];
            i++;
        }

    }else{
        printf("Sono il padre\n");
        wait(NULL);
        //legge da memoria condivisa
        write(STDOUT_FILENO, punt1, 4);
        printf("\n");

    }

    shmdt(punt1);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}