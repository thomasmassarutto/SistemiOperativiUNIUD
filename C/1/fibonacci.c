#include <stdio.h>
#include <stdlib.h>

#define MAX 32
#define MIN 0
#define ERR -1
/*
* Ricorsione Fibonacci
*/
int fibonacci_recursive(int x, int *counter){
    (*counter)++;
    if (x == 0){
        return 0;
    }else if (x == 1){
        return 1;
    }else{
        return fibonacci_recursive(x-1, counter) + fibonacci_recursive(x-2, counter); 
    }

}

/*
* Memoization Fibonacci 
*/

int fibonacci_mem(int x){
    int res= ERR;

    int mem[x];
    mem[0]= 0;
    mem[1]= 1;
    
    for (int i = 2; i <= x; i++){
        int a= mem[i-2];
        int b= mem[i-1];
        mem[i]= a+b;
    }

    res= mem[x];

    return res;
}

// variante con i puntatori
int fibonacci_mem_ptr(int x){
    int res= ERR;

    int mem[x];
    mem[0]= 0;
    mem[1]= 1;
    
    int *ptr = &mem[2];

    int i = 2;
    while(i <= x){
        int a= *(ptr-2);
        int b= *(ptr-1);
        *ptr= a+b;
        *ptr++;
        i++;
    }

    res= mem[x];

    return res;
}

int main() {

    //numero di cui vogliamo conoscere Fibonacci
    int n= 3;
    int rec_res= ERR;
    int mem_res= ERR;
    int mem_res_ptr= ERR;

    int fibonacci_recursive_counter = 0;

    if (n <= MAX){

        rec_res= fibonacci_recursive(n, &fibonacci_recursive_counter);
        mem_res= fibonacci_mem(n);
        mem_res_ptr= fibonacci_mem_ptr(n);

        printf("F(%d) -> %d\trecursion, %d chiamate \n", n, rec_res, fibonacci_recursive_counter);
        printf("F(%d) -> %d\tmemoization\n", n, mem_res);
        printf("F(%d) -> %d\tmemoization variante puntatori\n", n, mem_res_ptr);
  
    }else{
        printf("n < 32");
    }

        

    return (EXIT_SUCCESS);

}
