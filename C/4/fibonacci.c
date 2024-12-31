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

    int array[n];

    fibonacci(n, array);

    int res= *(array + n);

    printf("F(%d) -> %d", n, res);

    return (EXIT_SUCCESS);
}