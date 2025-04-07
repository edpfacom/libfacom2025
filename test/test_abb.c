#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

/*Definições desenvolvedor usuario*/
/*Definições desenvolvedor da biblioteca*/
typedef struct _node{
    int key;
    struct _node * esq;
    struct _node * dir;
}tnode;

typedef struct _arv{
    tnode * raiz;
    int (*cmp)(void *, void *);
}tarv;



/*funções desenvolvedor da biblioteca*/

void abb_constroi(tarv * arv, int (*cmp)(void *a, void *b)){
    arv->raiz = NULL;
    arv->cmp = cmp;
}

/*teste*/
int comparador(void *a, void *b){
    return a-b;
}
void test_constroi(){
    /* declaracao de variaveis */
    tarv arv;
    tnode node1;
    tnode node2;

    node1.key = 10;
    node2.key = 13;

    /* chamada de funções */
    abb_constroi(&arv,comparador);
    
    /* testes */
    assert(arv.raiz == NULL);
    assert(arv.cmp(&node1,&node2) == 3);
}

void abb_insere(tnode **raiz, int key){
    if(*raiz == NULL){
        *raiz = malloc(sizeof(tnode));
        (*raiz)->key = key;
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
    }
}

void test_insere(){
    tnode * raiz;
    raiz = NULL;
    
    abb_insere(&raiz,10);
    abb_insere(&raiz,20);
    abb_insere(&raiz,5);
    abb_insere(&raiz,3);
    abb_insere(&raiz,7);

    assert(raiz->key == 10);
   /* assert(raiz->esq->key == 5);
    assert(raiz->dir->key == 20);
    assert(raiz->esq->esq->key == 3);
    assert(raiz->esq->dir->key == 7);*/
}


int main(void){
    test_insere();
    return EXIT_SUCCESS;
}
