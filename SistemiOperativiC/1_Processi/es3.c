#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    pid_t processoFiglio;

    processoFiglio = fork();

    if (processoFiglio < 0){
        printf("Err: generazione figlio non riuscita");

    }else if(processoFiglio == 0){  //FIGLIO
        int syscallresult= execl("/home/thomas/Scrivania/SO/SistemiOperativiUNIUD/SistemiOperativiC/1_Processi/es1", "chisonoio", NULL);
        //int syscallresult= execl("/bin/nome_bin", "arg[0]","arg[1]", NULL);

        if (syscallresult < 0){
            perror("syscall error\n");
            exit(EXIT_FAILURE);
        }

    }else{ //PADRE
        sleep(3);
    }

    return(EXIT_SUCCESS);
}