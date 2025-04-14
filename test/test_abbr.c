#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

/*Definições desenvolvedor usuario*/
typedef struct _reg{
    int id;
    int idade;
}treg;

void * aloca_reg(int id, int idade){
    treg * reg;
    reg = malloc(sizeof(treg));
    reg->id = id;
    reg->idade = idade;
}

int comparador(void *a, void *b){
    return ((treg *)a)->id - ((treg *)b)->id;
}



/*Definições desenvolvedor da biblioteca*/
typedef struct _node{
    void * key;
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
void test_constroi(){
    /* declaracao de variaveis */
    tarv arv;
    tnode node1;
    tnode node2;

    node1.key = aloca_reg(4,1);
    node2.key = aloca_reg(1,2);


    /* chamada de funções */
    abb_constroi(&arv,comparador);
    
    /* testes */
    assert(arv.raiz == NULL);
    assert(arv.cmp(node1.key,node2.key) == 3);
    free(node1.key);
    free(node2.key);
}

void _abb_insere(tnode **raiz, void * key, int (*cmp)(void *a, void *b)){
    if(*raiz == NULL){
        *raiz = malloc(sizeof(tnode));
        (*raiz)->key = key;
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
    }else if (cmp( (*(*raiz)).key , key ) <0){
        _abb_insere( &((*(*raiz)).dir),key,cmp);
    }else{
        _abb_insere( &((*raiz)->esq),key,cmp);
    }
}

void abb_insere(tarv *arv, void *key){
    _abb_insere(&(arv->raiz),key,arv->cmp);
}


void _abb_destroi(tnode * node){
    if (node!=NULL){
        _abb_destroi(node->esq);
        _abb_destroi(node->dir);
        free(node->key);
        free(node);
    }
}

void abb_destroi(tarv *arv){
    _abb_destroi(arv->raiz);
}


tnode ** _abb_busca(tarv *arv, tnode ** atual, void * key){
}

tnode ** abb_busca(tarv *arv, void * key){
    return _abb_busca(arv,&(arv->raiz),key);

}


void _remove_raiz(tnode ** raiz){
}

int abb_remove(tarv *arv, void * key){
    tnode ** node = abb_busca(arv,key);
    int ret = EXIT_FAILURE;
    if (node != NULL){
        _remove_raiz(node);
        ret = EXIT_SUCCESS;
    }
    return ret;
}

void test_insere(){
    tarv arv;
    abb_constroi(&arv,comparador);
    abb_insere(&arv,aloca_reg(10,1));
    abb_insere(&arv,aloca_reg(20,1));
    abb_insere(&arv,aloca_reg(5,1));
    abb_insere(&arv,aloca_reg(3,1));
    abb_insere(&arv,aloca_reg(7,1));
    tnode * raiz;
    raiz = arv.raiz;
    assert(((treg *)raiz->key)->id == 10);
    assert(((treg *)raiz->dir->key)->id == 20);
    assert(((treg *)raiz->esq->key)->id == 5);
    assert(((treg *)raiz->esq->esq->key)->id == 3);
    assert(((treg *)raiz->esq->dir->key)->id == 7);
    abb_destroi(&arv);
}

void test_busca(){
    tarv arv;
    tnode ** ppnode;
    treg * reg;
    abb_constroi(&arv,comparador);
    abb_insere(&arv,aloca_reg(10,11));
    abb_insere(&arv,aloca_reg(20,21));
    abb_insere(&arv,aloca_reg(5,6));
    abb_insere(&arv,aloca_reg(3,4));
    abb_insere(&arv,aloca_reg(7,8));
    abb_insere(&arv,aloca_reg(15,16));
    abb_insere(&arv,aloca_reg(17,18));
    
    reg = aloca_reg(17,10);
    ppnode = abb_busca(&arv,reg);
    assert(((treg *)((tnode *) *ppnode)->key)->id == 17);
    assert(((treg *)((tnode *) *ppnode)->key)->idade == 18);

    reg->id = 10;
    ppnode = abb_busca(&arv,reg);
    assert(((treg *)((tnode *) *ppnode)->key)->id == 10);
    assert(((treg *)((tnode *) *ppnode)->key)->idade == 11);

    reg->id = 20;
    ppnode = abb_busca(&arv,reg);
    assert(((treg *)((tnode *) *ppnode)->key)->id == 20);
    assert(((treg *)((tnode *) *ppnode)->key)->idade == 21);

    reg->id = 5;
    ppnode = abb_busca(&arv,reg);
    assert(((treg *)((tnode *) *ppnode)->key)->id == 5);
    assert(((treg *)((tnode *) *ppnode)->key)->idade == 6);


    abb_destroi(&arv);
}

void test_remove(){
    tarv arv;
    abb_constroi(&arv,comparador);
    abb_insere(&arv,aloca_reg(10,1));
    abb_insere(&arv,aloca_reg(20,1));
    abb_insere(&arv,aloca_reg(5,1));
    abb_insere(&arv,aloca_reg(3,1));
    abb_insere(&arv,aloca_reg(7,1));
    abb_insere(&arv,aloca_reg(15,1));
    abb_insere(&arv,aloca_reg(17,1));
    tnode * raiz;
    raiz = arv.raiz;
    treg * reg = aloca_reg(20,1);

    abb_remove(&arv,reg);

    assert(((treg *)raiz->key)->id == 10);
    assert(((treg *)raiz->dir->key)->id == 17);
    assert(((treg *)raiz->dir->esq->key)->id == 15);
    assert(((treg *)raiz->esq->key)->id == 5);
    assert(((treg *)raiz->esq->esq->key)->id == 3);
    assert(((treg *)raiz->esq->dir->key)->id == 7);

    reg->id = 17;
    abb_remove(&arv,reg);
    assert(((treg *)raiz->key)->id == 10);
    assert(((treg *)raiz->dir->key)->id == 15);
    assert(raiz->dir->dir == NULL);
    assert(raiz->dir->esq == NULL);
    assert(((treg *)raiz->esq->key)->id == 5);
    assert(((treg *)raiz->esq->esq->key)->id == 3);
    assert(raiz->esq->esq->esq == NULL);
    assert(raiz->esq->esq->dir == NULL);
    assert(((treg *)raiz->esq->dir->key)->id == 7);
    assert(raiz->esq->dir->esq == NULL);
    assert(raiz->esq->dir->dir == NULL);

    reg->id = 10;
    abb_remove(&arv,reg);
    raiz = arv.raiz;
    assert(((treg *)raiz->key)->id == 15);
    assert(((treg *)raiz->esq->key)->id == 5);
    assert(((treg *)raiz->esq->esq->key)->id == 3);
    assert(((treg *)raiz->esq->dir->key)->id == 7);

    reg->id = 15;
    abb_remove(&arv,reg);
    raiz = arv.raiz;
    assert(((treg *)raiz->key)->id == 7);
    assert(((treg *)raiz->esq->key)->id == 5);
    assert(((treg *)raiz->esq->esq->key)->id == 3);

    reg->id = 7;
    abb_remove(&arv,reg);
    raiz = arv.raiz;
    assert(((treg *)raiz->key)->id == 5);
    assert(((treg *)raiz->esq->key)->id == 3);

    reg->id = 3;
    abb_remove(&arv,reg);
    raiz = arv.raiz;
    assert(((treg *)raiz->key)->id == 5);
    assert((treg *)raiz->esq == NULL);
    assert((treg *)raiz->dir == NULL);

    reg->id = 5;
    abb_remove(&arv,reg);
    raiz = arv.raiz;
    assert((treg *)raiz == NULL);

}



int main(void){
    test_constroi();
    test_insere();
   /* test_busca();
    test_remove();*/
    printf("SUCCESS!!\n");
    return EXIT_SUCCESS;
}
