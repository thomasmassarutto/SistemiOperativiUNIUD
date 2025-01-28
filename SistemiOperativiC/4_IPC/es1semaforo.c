
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/wait.h>

#define SEMPERM 0600

typedef union _semun {
    int val;
    struct semid_ds *buf;
    ushort *array; } semun;

// init del semaforo
int initsem(key_t semkey) {
    int status = 0; 
    int semid;

    semid = semget(semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL);

    if (semid == -1) {
        if (errno == EEXIST) {  
            semid = semget(semkey, 1, 0); 
            }

        } else {
        semun arg;
        arg.val = 0; // valore inizializzazione semaforo
        status = semctl(semid, 0, SETVAL, arg);
    }

    if (semid == -1 || status == -1) {
        perror("initsem fallita");
        return(-1);
    }

    return(semid);
}

// per rimuovere il semaforo. necessario alla fine del programma
int removeSem(key_t semkey) {
    int semid = semget(semkey, 1, 0);
    if (semid == -1) {
        perror("semget fallita durante la rimozione del semaforo");
        return -1;
    }

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("rimozione del semaforo fallita");
        return -1;
    }

    printf("Semaforo con key %x rimosso con successo\n", semkey);
    return 0;
}

// implementazione di wait
int waitSem(int semid) {
    struct sembuf wait_buf;

    wait_buf.sem_num = 0;
    wait_buf.sem_op = -1;

    if(semop(semid, &wait_buf, 1) == -1) {
        perror("waitSem fallita");
        exit(1);
    }

    return(0);
}

// implementazione di signal
int signalSem(int semid) {

    struct sembuf signal_buf;
    signal_buf.sem_num = 0;
    signal_buf.sem_op = 1;

    if(semop(semid, &signal_buf, 1) == -1) {
        perror("signalSem fallita");
        exit(1);
    }
    return(0);
}


int main(){
    printf("Hello, world!\n");

    key_t semkey = 0x200;
    int semid;
    pid_t cpid;

    semid= initsem(semkey);
    if ( semid< 0){
        perror("Init semaforo");
    }

    cpid= fork();
    if (cpid == -1){
        perror("fork()");
        exit(1);
    }

    if (cpid == 0){
        waitSem(semid);
        sleep(5);
        printf("Ciao, sono il processo figlio, PID: %d\n", getpid());
    }else{
        printf("Ciao, sono il processo padre, PID: %d\n", getpid());
        signalSem(semid);
        wait(NULL);
        if (removeSem(semkey) == 0) {
            printf("Semaforo esistente rimosso con successo\n");
        }
    }

    exit(0);
}