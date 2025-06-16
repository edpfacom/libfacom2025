#include<stdio.h>
#include<stdlib.h>
#include"avl.h"

int cmpfunc(const void *a, const void *b){
    return (*(treg *)a).chave - (*(treg *)b).chave ;
}

tnode ** percorre_esq(tnode ** arv){
    tnode * aux = *arv;
    if (aux->esq == NULL){
        return arv;
    }else{
        while (aux->esq->esq != NULL)
            aux = aux->esq;
        return &(aux->esq);
    }
}

void cria_arvore(tnode ** parv){
    *parv = NULL;
}
void insere_arvore(tnode ** parv,treg reg){
    if (*parv == NULL){
        *parv = (tnode *) malloc(sizeof(tnode));
        (*parv)->reg = reg;
        (*parv)->esq = NULL;
        (*parv)->dir = NULL;
    }else if(cmpfunc(&((*parv)->reg), &reg)>0){
        insere_arvore(&(*parv)->esq,reg);
    }else{
        insere_arvore(&((*parv)->dir),reg);
    }
}
int max(int a,int b){
    return a>b?a:b;
}

int altura(tnode *arv){
    int ret;
    if (arv==NULL){
        ret = -1;
    }else{
        ret = arv->h;
    }
    return ret;
}
void RE(tnode **parv){
    tnode * y = *parv;
    tnode * x = y->dir;
    tnode * A = y->esq;
    tnode * B = x->esq;
    tnode * C = x->dir;

    y->dir = B; 
    y->esq = A;
    x->dir = C;
    x->esq = y;
    *parv = x;
    y->h = max(altura(A),altura(B)) +1;
    x->h = max(altura(y),altura(C))+1;

}
void RD(tnode **parv){
    tnode * x = *parv;
    tnode * y = x->esq;
    tnode * A = y->esq;
    tnode * B = y->dir;
    tnode * C = x->dir;

    x->dir = C; 
    x->esq = B;
    y->esq = A;
    y->dir = x;
    *parv = y;

    x->h = max(altura(B),altura(C)) +1;
    y->h = max(altura(A),altura(x)) +1;
}


void rebalancear(tnode **parv){
    int fb;
    int fbf;
    tnode *filho;


    fb = altura((*parv)->esq) - altura((*parv)->dir);
    if (fb == -2){ /*--> -->*/
        filho = (*parv)->dir;
        fbf = altura(filho->esq) - altura(filho->dir); 
        if (fbf <= 0){ /* ---> */
            RE(parv);
        }else{ /* <-- */
            RD(&(*parv)->dir);
            RE(parv);
        }
    }else if (fb == 2){ /*<-- <--*/
        filho = (*parv)->esq;
        fbf = altura(filho->esq) - altura(filho->dir); 
        if (fbf >= 0){ /* <-- */
            RD(parv);
        }else{ /* ->*/
            RE(&(*parv)->esq);
            RD(parv);
        }
    }

}


void insere_avl(tnode ** parv,treg reg){
    if (*parv == NULL){
        *parv = (tnode *) malloc(sizeof(tnode));
        (*parv)->reg = reg;
        (*parv)->esq = NULL;
        (*parv)->dir = NULL;
        (*parv)->h = 0;

    }else if(cmpfunc(&((*parv)->reg), &reg)>0){
        insere_avl(&(*parv)->esq,reg);
    }else{
        insere_avl(&(*parv)->dir,reg);
    }
    (*parv)->h = max(altura((*parv)->esq),altura((*parv)->dir)) + 1;
    rebalancear(parv);
}


void pre_ordem(tnode **parv, treg * (*myfunc)(treg)){
    if (*parv != NULL){
        myfunc((*parv)->reg);
        pre_ordem(&(*parv)->esq,myfunc);
        pre_ordem(&(*parv)->dir,myfunc);
    }
}
void in_ordem(tnode **parv, treg * (*myfunc)(treg)){
    if (*parv != NULL){
        in_ordem(&(*parv)->esq,myfunc);
        myfunc((*parv)->reg);
        in_ordem(&(*parv)->dir,myfunc);
    }
}
void pos_ordem(tnode **parv, treg * (*myfunc)(treg)){
    if (*parv != NULL){
        pos_ordem(&(*parv)->esq,myfunc);
        pos_ordem(&(*parv)->dir,myfunc);
        myfunc((*parv)->reg);
    }
}


tnode ** apaga_avl(tnode **parv, treg reg){
    tnode ** ret;
    tnode *aux;
    tnode **sucessor;
    int cmp;
    if (*parv == NULL)
        ret = NULL;
    else{
        cmp = cmpfunc(&(*parv)->reg,&reg);
        if (cmp > 0){
            ret = apaga_avl(&((*parv)->esq),reg);
        }else if (cmp < 0){
            ret = apaga_avl(&((*parv)->dir),reg);
        }else{ /*achou!! remove o registro*/
            /*node folha*/
            if ((*parv)->esq == NULL && (*parv)->dir == NULL){
                free(*parv);
                *parv = NULL;
            }else if ((*parv)->esq == NULL || (*parv)->dir == NULL){
                /*tem um filho*/
                aux = *parv;
                if ((*parv)->esq == NULL){
                    *parv = (*parv)->dir;
                }else{
                    *parv = (*parv)->esq;
                }
                free(aux);
            }else{
                /*tem dois filhos*/
                sucessor = percorre_esq(&(*parv)->dir);
                (*parv)->reg = (*sucessor)->reg;
                ret = apaga_avl(&(*parv)->dir,(*sucessor)->reg); 
            }
        }
        if (*parv != NULL){
            (*parv)->h = max(altura((*parv)->esq),altura((*parv)->dir)) + 1;
            rebalancear(parv);
        }
        ret = parv;

    }
    return ret;
}




tnode ** busca_no(tnode **parv, treg reg){
    tnode ** ret;
    int cmp;
    if (*parv == NULL)
        ret = NULL;
    else{
        cmp = cmpfunc(&(*parv)->reg,&reg);
        if (cmp == 0){
            ret = parv;
        }else if (cmp > 0){
            ret = busca_no(&((*parv)->esq),reg);
        }else{
            ret = busca_no(&((*parv)->dir),reg);
        }
    }
    return ret;
}

void apaga_arvore(tnode **parv){
    if (*parv != NULL){
        apaga_arvore(&(*parv)->esq);
        apaga_arvore(&(*parv)->dir);
        free(*parv);
        *parv = NULL;
    }
}



int apaga_node(tnode **parv,treg reg){
    int ret = 0;
    tnode ** raiz;
    tnode * aux;
    tnode * auxraiz;
    tnode ** filho;
    /*busca node*/
    raiz = busca_no(parv,reg);
    
    if (raiz == NULL){
        ret = 0;
    }else if ((*raiz)->dir == NULL){
        /*trata caso degenerado esq*/
        aux = *raiz;
        *raiz = aux->esq;
        free(aux);
    }else{
        /*remove direita esquerda*/
        filho = percorre_esq(&(*raiz)->dir);
        aux = *filho;
        *filho = aux->dir;
        aux->esq = (*raiz)->esq;
        aux->dir = (*raiz)->dir;
        auxraiz = *raiz;
        *raiz = aux;
        free(auxraiz);
    }
    return ret;

}
