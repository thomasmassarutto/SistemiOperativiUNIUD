#include <stdio.h>
#include <stdlib.h>

#define MAX 32
#define ERR -1

// variante for
void fib1(int x, int *array){
    array[0]=0;
    array[1]=1;
    
    for (int i=2;i<=x;i++){
        int a = array[i-2];
        int b = array[i-1];
        array[i]= a+b;
    } 

}

// variante while 
void fib2(int x, int *array){
    array[0]=0;
    array[1]=1;
    
    int i= 2;
    while (i<=x){
        array[i]= array[i-2] + array[i-1];
        i++;
    }
}

// variante do-while e' la stessa roba dai... 

int main(){

    int n = 7;

    int array[MAX];
    int *first= &array[0];

    if (n <= MAX){
        fib2(n, first);

        printf("F(%d)->%d\n",n, array[n]);

        for (int i=0; i<=5 ; i++){
            printf("F(%d)->%d\n",i, array[i]);
        }
    }else {
        printf("n <= 32");
    }
     
    return(EXIT_SUCCESS);
}