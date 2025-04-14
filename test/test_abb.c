#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

/*Definições desenvolvedor usuario*/
typedef struct _reg{
    int id;
    int idade;
    char nome[30];
}treg;

/*Definições desenvolvedor da biblioteca*/
typedef struct _node{
    treg key;
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
int comparador_inteiro(int a, int b){
    return a-b;
}

int comparador_treg(treg a, treg b){
    return a.idade - b.idade;
}


void test_constroi(){
    /* declaracao de variaveis */
    tarv arv;
    tnode node1;
    tnode node2;


    /* chamada de funções */
    abb_constroi(&arv,comparador);
    
    /* testes */
    assert(arv.raiz == NULL);
    assert(arv.cmp(&node1,&node2) == 3);
}

void abb_insere(tnode **raiz, treg key){
    if(*raiz == NULL){
        *raiz = malloc(sizeof(tnode));
        (*raiz)->key = key;
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
    }else if ( comparador_treg( (*(*raiz)).key , key ) <0){
        abb_insere( &((*(*raiz)).dir),key);
    }else{
        abb_insere( &((*raiz)->esq),key);
    }
}

void test_insere_id(){
    tnode * raiz;
    raiz = NULL;
    treg reg;
    reg.id = 10;abb_insere(&raiz,reg);
    reg.id = 20;abb_insere(&raiz,reg);
    reg.id = 5 ;abb_insere(&raiz,reg);
    reg.id = 3 ;abb_insere(&raiz,reg);
    reg.id = 7 ;abb_insere(&raiz,reg);

    assert(raiz->key.id == 10);
    assert(raiz->dir->key.id == 20);
    assert(raiz->esq->key.id == 5);
    assert(raiz->esq->esq->key.id == 3);
    assert(raiz->esq->dir->key.id == 7);
}
void test_insere_idade(){
    tnode * raiz;
    raiz = NULL;
    treg reg;
    reg.idade = 10;abb_insere(&raiz,reg);
    reg.idade = 20;abb_insere(&raiz,reg);
    reg.idade = 5 ;abb_insere(&raiz,reg);
    reg.idade = 3 ;abb_insere(&raiz,reg);
    reg.idade = 7 ;abb_insere(&raiz,reg);

    assert(raiz->key.idade == 10);
    assert(raiz->dir->key.idade == 20);
    assert(raiz->esq->key.idade == 5);
    assert(raiz->esq->esq->key.idade == 3);
    assert(raiz->esq->dir->key.idade == 7);
}





int main(void){
    test_insere_idade();
    printf("SUCCESS!!\n");
    return EXIT_SUCCESS;
}
