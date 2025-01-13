#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv){
    
    char *basepath= "/bin/";
    
    if (argc < 2){
        printf("Err: no args\n");
    }

    int cmdlength = strlen(argv[1]);
    int basepathlength= strlen(basepath);

    int totallength= cmdlength + basepathlength +1;

    char *fullpath= malloc(totallength);

    strcpy(fullpath, basepath);
    strcat(fullpath, argv[1]);
    
    printf("%s\n", fullpath);

    if (access(fullpath, F_OK) != 0){// se il percorso non esiste
        perror("Comando non esiste");
        exit(EXIT_FAILURE);
    }

    int syscallresult = execv(fullpath, &argv[1]);

    if (syscallresult<0){
        perror("syscall error");
        exit(EXIT_FAILURE);

    }
    return(EXIT_SUCCESS);
}