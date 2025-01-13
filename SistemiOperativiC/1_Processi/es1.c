#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(){

    printf("pid: %d\n", getpid());
    printf("pid: %d\n", getgid());
    printf("pid: %d\n", getuid());

    return(EXIT_SUCCESS);
}