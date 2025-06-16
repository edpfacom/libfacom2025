#ifndef ARVORE_H
#define ARVORE_H

typedef int tchave;
typedef struct{
    tchave chave;
    int idade;
}treg;

typedef struct node * pnode;

typedef struct node{
    treg reg;
    struct node * dir;
    struct node * esq;
    struct node * pai;     
    int h;
}tnode;

typedef struct {
    treg v[100];
    int last;
}tpilha;

void cria_arvore(tnode ** parv);
void insere_arvore(tnode ** parv,treg reg);
void pre_ordem(tnode **parv, treg * (*myfunc)(treg));
void in_ordem(tnode **parv, treg * (*myfunc)(treg));
void pos_ordem(tnode **parv, treg * (*myfunc)(treg));
tnode ** busca_no(tnode **parv, treg reg);
void apaga_arvore(tnode **parv);
int apaga_node(tnode **parv,treg reg);


void insere_avl(tnode ** parv,treg reg);
tnode ** apaga_avl(tnode ** parv,treg reg);
#endif 
