#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"arvoreb.h"




void test_insere(){
    tarv arv;
    int t = 3;
    int i;
    
    btree_constroi(&arv,t);
    assert(arv.raiz == NULL);
    assert(arv.t == t);
    for (i = 10;i<200;i+=10)
        btree_insere(&arv,i)
    
}


int main(void){
    test_insere();
    return 0;
}


