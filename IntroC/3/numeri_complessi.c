#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct compl {
    float reale;
    float immaginaria;
} compl;

enum tipo_di_numero { INTERO, REALE, REALEDOPPIO, COMPLESSO } ;

struct numero {
    enum tipo_di_numero tipo;
        union {
        int intero;
        double reale;
        long double realelong;
        compl complesso;
    };
};

void compl_print(compl x){
    if (x.immaginaria < 0){
        printf("%f -i(%f)\n", x.reale, x.immaginaria*(-1));
    } else {
        printf("%f +i(%f)\n", x.reale, x.immaginaria);
    }
}

bool compl_is_equal(compl a, compl b){
    if (a.reale == b.reale && a.immaginaria == b.immaginaria){
        return true;
    }else{
        return false;
    }
}

compl compl_conjugate(compl a){
    compl res;

    res.reale= a.reale;
    res.immaginaria= -a.immaginaria;
    
    return res;
}

compl compl_sum(compl a, compl b){
    compl res;

    res.reale = a.reale + b.reale;
    res.immaginaria = a.immaginaria + b.immaginaria;

    return res;
}

compl compl_product(compl a, compl b){
    compl res;

    res.reale = (a.reale * b.reale) - (a.immaginaria * b.immaginaria);
    res.immaginaria = (a.reale * b.immaginaria) +  (a.immaginaria * b.reale);

    return res;
}

void numero_print(struct numero *x){

    if (x->tipo == INTERO){
        printf("intero: %d\n", x->intero);
    }else if(x->tipo == REALE){
        printf("reale: %f\n", x->reale);
    }else if(x->tipo == REALEDOPPIO){
        printf("reale doppio: %Lf\n", x->realelong);
    }else if(x->tipo == COMPLESSO){
        printf("complesso:");
        compl_print(x->complesso);
    }else{
        printf("Formato non riconosciuto");
    }
}

int main(){

    compl a;
    a.reale=2.0;
    a.immaginaria=5.0;

    compl b;
    b.reale=1.0;
    b.immaginaria=-3.0;
    
    printf("a: ");
    compl_print(a);
    printf("b: ");
    compl_print(b);

    printf("a e b sono uguali: %c\n",compl_is_equal(a,b) ? 'T': 'F');

    compl c= compl_conjugate(a);
    printf("Il coniugato di a: ");
    compl_print(c);

    compl s= compl_sum(a, b);
    printf("a+b= ");
    compl_print(s);

    compl p= compl_product(a,b);
    printf("a*b= ");
    compl_print(p);

    /* */

    struct numero x;

    x.tipo = INTERO;
    x.intero = 3;

    x.tipo = COMPLESSO;
    x.complesso = a;

    numero_print(&x);

    return(EXIT_SUCCESS);
}