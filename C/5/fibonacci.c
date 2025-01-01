#include <stdio.h>
#include <stdlib.h>

void fibonacci(int x, int *array){
    *array =0;
    *(array +1)=1;

    int* i= array +2;
    int count = 2;

    while (count <= x ){
        int a= *(i -2);
        int b= *(i -1);
        *i= a+b;
        count ++;
        i++;
    }
}

int main(){

    int n= 7;

    int *ptr;

    int res= 0;

    /*malloc*/
    ptr= (int*) malloc(n* sizeof(int));

    fibonacci(n, ptr);

    res= *(ptr + n);

    printf("F(%d) -> %d\n", n, res);

    free(ptr);

    /*calloc*/
    ptr= (int*) calloc(n, sizeof(int));

    fibonacci(n, ptr);

    res= *(ptr + n);

    printf("F(%d) -> %d\n", n, res);

    free(ptr);

    return (EXIT_SUCCESS);
}