#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#define BUFFSIZE 1024

int main(int argc, char *argv[]){

    int fdin;
    int fdout;

    ssize_t numbytes;
    char buf[BUFFSIZE];

    printf("ciao, sono \"%s\" \n", argv[0]);

    printf("Argc %d\n", argc);

    if (argc != 3){
        printf("No 3 args given\n");
        exit(EXIT_FAILURE);
    }

    fdin = open(argv[1],O_RDONLY);

    if (fdin < 0){
        perror("Errore apertura file in");
    }

    fdout = open(argv[2], O_WRONLY | O_CREAT, 0600);
    if (fdout < 0){
        perror("Errore apertura file out");
    }
    

    do{
        numbytes = read(fdin, buf, BUFFSIZE);// riempie un buffer e dice quanti bytes ha letto

        if (numbytes > 0){
            int written_bytes= write(fdout, buf, numbytes);

            if (written_bytes != numbytes){
                perror("errore scrittura");
            }
        }else if (numbytes < 0){
            perror("errore lettura");
        }


    }while(numbytes > 0);// se non ci sono più byte da leggere si ferma


    return(EXIT_SUCCESS);

}