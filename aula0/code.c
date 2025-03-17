#include<stdio.h>
#include<stdlib.h>
#include<assert.h>


typedef struct _node{
    int reg;
    struct _node * prox;
}pnode;

void cria_pilha(pnode ** pilha){
    printf("cria_pilha &pilha %p\n",&pilha);
    printf("cria_pilha pilha %p\n",pilha);
    printf("cria_pilha *pilha %p\n",*pilha);
    *pilha = NULL;
    printf("cria_pilha depois *pilha %p\n",*pilha);
}
void alterai(int * i){
    *i = 10;
}

int main(void){
    pnode * pilha;
    pnode temp;
    pilha = &temp;
    printf("&pilha %p\n",&pilha);
    printf("pilha %p\n",pilha);
    cria_pilha(&pilha);
    assert(pilha==NULL);

    int i;
    alterai(&i);
    assert(i==10);
    return EXIT_SUCCESS;
}
