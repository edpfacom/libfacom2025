#ifndef _ARVORE_B_
#define _ARVORE_B_

typedef int tchave;

typedef struct _node{
    int folha;
    int n;
/*    tchave * chaves;*/
/*    struct _node ** c;*/
    tchave chaves[5];
    struct _node * c[6];
}tnode;

typedef struct _arv{
    tnode * raiz;
    int t;
}tarv;

void btree_constroi(tarv * parv, int t);
int btree_insere(tarv *parv, tchave k);


#endif
