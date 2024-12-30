#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct compl {
    float reale;
    float immaginaria;
};

void compl_print(struct compl x){
    if (x.immaginaria < 0){
        printf("%f -i(%f)\n", x.reale, x.immaginaria*(-1));
    } else {
        printf("%f +i(%f)\n", x.reale, x.immaginaria);
    }
}

bool compl_is_equal(struct compl a, struct compl b){
    if (a.reale == b.reale && a.immaginaria == b.immaginaria){
        return true;
    }else{
        return false;
    }
}

struct compl compl_conjugate(struct compl a){
    struct compl res;

    res.reale= a.reale;
    res.immaginaria= -a.immaginaria;
    
    return res;
}

struct compl compl_sum(struct compl a, struct compl b){
    struct compl res;

    res.reale = a.reale + b.reale;
    res.immaginaria = a.immaginaria + b.immaginaria;

    return res;
}

struct compl compl_product(struct compl a, struct compl b){
    struct compl res;

    res.reale = (a.reale * b.reale) - (a.immaginaria * b.immaginaria);
    res.immaginaria = (a.reale * b.immaginaria) +  (a.immaginaria * b.reale);

    return res;
}

int main(){

    struct compl a;
    a.reale=2.0;
    a.immaginaria=5.0;

    struct compl b;
    b.reale=1.0;
    b.immaginaria=-3.0;
    
    printf("a: ");
    compl_print(a);
    printf("b: ");
    compl_print(b);

    printf("a e b sono uguali: %c\n",compl_is_equal(a,b) ? 'T': 'F');

    struct compl c= compl_conjugate(a);
    printf("Il coniugato di a: ");
    compl_print(c);

    struct compl s= compl_sum(a, b);
    printf("a+b= ");
    compl_print(s);

    struct compl p= compl_product(a,b);
    printf("a*b= ");
    compl_print(p); 

    return(EXIT_SUCCESS);
}