#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"avl.h"
treg * imprime(treg reg){
    printf("%d\n",reg.chave);
}



void test_remove(){
    tnode * arv;
    treg reg;
    int i;
    cria_arvore(&arv);
    for (i=10;i>0;i--){
        reg.chave =i;
        insere_avl(&arv,reg);
    }
    in_ordem(&arv,imprime);
    assert(arv->reg.chave == 7);
    assert(arv->esq->reg.chave == 3);
    assert(arv->dir->reg.chave == 9);

    assert(arv->dir->dir->reg.chave == 10);
    assert(arv->dir->esq->reg.chave == 8);

    assert(arv->esq->esq->reg.chave == 2);
    assert(arv->esq->dir->reg.chave == 5);

    assert(arv->esq->esq->esq->reg.chave == 1);
    assert(arv->esq->dir->dir->reg.chave == 6);
    assert(arv->esq->dir->esq->reg.chave == 4);

    reg.chave = 7;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 8);
    assert(arv->esq->reg.chave == 3);
    assert(arv->dir->reg.chave == 9);

    assert(arv->dir->dir->reg.chave == 10);
    assert(arv->dir->esq == NULL);

    assert(arv->esq->esq->reg.chave == 2);
    assert(arv->esq->dir->reg.chave == 5);

    assert(arv->esq->esq->esq->reg.chave == 1);
    assert(arv->esq->dir->dir->reg.chave == 6);
    assert(arv->esq->dir->esq->reg.chave == 4);


    reg.chave = 8;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 3);
    assert(arv->esq->reg.chave == 2);
    assert(arv->dir->reg.chave == 9);

    assert(arv->dir->dir->reg.chave == 10);
    assert(arv->dir->esq->reg.chave == 5);

    assert(arv->esq->esq->reg.chave == 1);
    assert(arv->esq->dir == NULL);


    assert(arv->dir->esq->dir->reg.chave == 6);
    assert(arv->dir->esq->esq->reg.chave == 4);

    reg.chave = 3;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 4);
    assert(arv->esq->reg.chave == 2);
    assert(arv->dir->reg.chave == 9);

    assert(arv->dir->dir->reg.chave == 10);
    assert(arv->dir->esq->reg.chave == 5);

    assert(arv->esq->esq->reg.chave == 1);
    assert(arv->esq->dir == NULL);

    assert(arv->dir->esq->dir->reg.chave == 6);
    assert(arv->dir->esq->esq == NULL);

    reg.chave = 4;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 5);
    assert(arv->esq->reg.chave == 2);
    assert(arv->dir->reg.chave == 9);

    assert(arv->dir->dir->reg.chave == 10);
    assert(arv->dir->esq->reg.chave == 6);

    assert(arv->esq->esq->reg.chave == 1);
    assert(arv->esq->dir == NULL);

    reg.chave = 5;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 6);
    assert(arv->esq->reg.chave == 2);
    assert(arv->dir->reg.chave == 9);

    assert(arv->dir->dir->reg.chave == 10);
    assert(arv->esq->esq->reg.chave == 1);


    reg.chave = 6;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 9);
    assert(arv->esq->reg.chave == 2);
    assert(arv->dir->reg.chave == 10);

    assert(arv->esq->esq->reg.chave == 1);

    reg.chave = 9;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 2);
    assert(arv->esq->reg.chave == 1);
    assert(arv->dir->reg.chave == 10);

    reg.chave = 2;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 10);
    assert(arv->esq->reg.chave == 1);

    reg.chave = 10;
    apaga_avl(&arv,reg);

    assert(arv->reg.chave == 1);

    reg.chave = 1;
    apaga_avl(&arv,reg);

    assert(arv == NULL);



}


int main(void){
    test_remove();
    printf("SUCCESS!!\n");
    return EXIT_SUCCESS;
}
