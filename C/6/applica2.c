#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

double applica(double x, double y, double (func) (double, double)){
    return func(x, y);
}

double sum(double x, double y){
    return x + y;
}

double subtraction(double x, double y){
    return x - y;
}

double product(double x, double y){
    return x * y;
}

double division(double x, double y){
    if (y == 0){
        printf("y = 0");
        return(EXIT_FAILURE);
    }

    return x / y;
}

double (*select_function(char operator, bool* is_valid_operator)) (double, double){ // select_function ha come argomento un char e restituisce un puntatore ad una funzione fun(double, double)

    if (operator == '+'){
            return sum;

    }else if (operator == '-'){
            return subtraction;

    }else if (operator == '*'){
            return product;

    }else if (operator == '/'){
       return division;

     }else{
        *is_valid_operator = false;
        return(NULL);
    }

}

int main(int argc, char **argv){

    double x;
    double y;
    char operator;
    double res;

    bool is_valid_operator= true;
    
    double (*ptrfunc) (double, double);

    

    // controllo sugli argomenti
    if (argc == 4){
        char *endptr;

        x= strtod(argv[1], &endptr); // strtod()
        y= strtod(argv[3], &endptr);
        operator= *argv[2];

        ptrfunc = select_function(operator, &is_valid_operator);
        
        if (!is_valid_operator) {
            printf("Invalid operator\n");
            return(EXIT_FAILURE);
        }

        res= applica(x, y, ptrfunc);
        printf("%f %c %f = %f\n",x, operator, y, res);

    }else if (argc == 1){
        while (is_valid_operator){

            printf("Equation:\t");
            scanf("%lf", &x);
            scanf("%c",&operator);
            scanf("%lf",&y);

            ptrfunc = select_function(operator, &is_valid_operator);

            if (!is_valid_operator) {
                printf("invalid op\n");
                return(EXIT_FAILURE);
            }

            res= applica(x, y, ptrfunc);

            printf("%f %c %f = %f\n",x, operator, y, res);
        
        }
  
    }

    return(EXIT_SUCCESS);
}