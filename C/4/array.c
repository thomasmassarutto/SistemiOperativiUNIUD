#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_in(int x, int length, int *array){
    bool res = false;

    for (int i= 0; i<length; i++){
        if (array[i] == x){
            res= true;
            break;
        }
    } 

    return res;
}

int in_position(int x, int length, int* array){
    int res=-1;
    
    for (int i=0;i<length;i++){
        if (x == array[i]){
            res= i;
            break;
        }

    }

    return res;
}

int sum_array(int length, int* array){
    int res=0;

    for (int i= 0;i<length;i++){
        res= res+ array[i];
    }
    
    return res;
}


bool equals_array(int length1, int* array1, int length2, int* array2){
    bool res= true;

    if (length1 != length2){
        res= false;
    }else{
        for (int i=0;i<length1;i++){
            if (array1[i] != array2[i]){
                res= false;
                break;
            }
        }
    }

    return res;
}

int main(){

    int array [20]={76, 30, 29, 41, 40, 18, 27, 91, 32, 24, 11, 6, 68, 1, 82, 93, 95, 49, 88, 69};
    int array_length = sizeof(array) / sizeof(array[0]);
    int n= 69;

    int array2 [20]={76, 30, 29, 41, 40, 18, 27, 91, 32, 24, 11, 6, 68, 1, 82, 93, 95, 49, 88, 69};
    int array2_length = sizeof(array) / sizeof(array[0]);

    int array3 [20]={76, 30, 29, 41, 40, 18, 27, 91, 32, 24, 11, 6, 68, 1, 13, 93, 95, 49, 88, 69};
    int array3_length = sizeof(array) / sizeof(array[0]);

    printf("trovato? : %c\n",is_in(n, array_length, array) ? 'T': 'F');

    printf("in posizione: %d\n",in_position(n, array_length, array));

    printf("somma degli elementi: %d\n",sum_array(array_length, array));


    printf("array sono uguali: %c\n",equals_array(array_length, array, array3_length, array3) ? 'T': 'F');

    return(EXIT_SUCCESS);
}