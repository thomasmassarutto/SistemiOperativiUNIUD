#include<stdio.h>
#include<stdlib.h>

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

int main(int argc, char **argv){

    double x;
    double y;
    char operator;

    double (*ptrfunc) (double, double);

    double res;

    // controllo sugli argomenti
    if (argc == 4){
        char *endptr;

        x= strtod(argv[1], &endptr); // strtod()
        y= strtod(argv[3], &endptr);
        operator= *argv[2];

    }else if (argc == 1){
        printf("Empty args\n");

        printf("Equation:\t");
        scanf("%lf%c%lf", &x, &operator, &y);
    
    }
    
    // applicazione della logica
    if (operator == '+'){
        ptrfunc = sum;

    }else if (operator == '-'){
        ptrfunc = subtraction;
    
    }else if (operator == '*'){
        ptrfunc = product;
    
    }else if (operator == '/'){
        ptrfunc = division;

    }else{
        printf("Invalid operator\n");
        return(EXIT_FAILURE);
    }

    res= applica(x, y, ptrfunc);

    printf("%f %c %f = %f\n",x, operator, y, res);

    return(EXIT_SUCCESS);
}