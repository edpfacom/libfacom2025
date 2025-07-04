#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define TRUE 1
#define FALSE 0

typedef int tchave;

typedef struct _node{
    int folha;
    int n;
    tchave chaves[5];
    struct _node * c[6];
}tnode;

typedef struct _arv{
    tnode * raiz;
    int t;
}tarv;



tnode * aloca_node(tarv * parv){
    tnode * aux;
    int t = parv->t;
    aux = (tnode *)malloc(sizeof(tnode));
    if (aux != NULL){
      /*  aux->chaves = malloc(sizeof(int)*(2*t-1));
        aux->c = malloc(sizeof(tnode *)*(2*t)); */
        aux->n = 0;
        aux->folha=0;
    }
    return aux;
}

int menor(int a,int b){
    return a<b?a:b;
}
void imprime_arvore(tnode * x,int prof,int i){
    int anda = 3;
    int j;
    int k;
    if (x->folha==TRUE){
        for (j=0;j<prof;j++)
            printf(" ");
        for (j=0;j<x->n;j++)
            printf("%d ",x->chaves[j]);
        printf("\n");

    }else{
        imprime_arvore(x->c[0],prof+anda,0);
        for (k=0;k<x->n;k++){ 
            for (j=0;j<prof;j++)
                printf(" ");
            printf("%d\n",x->chaves[k]);
            imprime_arvore(x->c[k+1],prof+anda,k+1);
        }
        if (prof == 0)
            printf("------\n");
    }
}



void btree_constroi(tarv * parv, int t){
    parv->t = t;
    parv->raiz = aloca_node(parv);
    parv->raiz->n = 0;
    parv->raiz->folha = TRUE;
}


int procura_chave(tnode *x, tchave k){
    int i;
    int ret = -1;
    for(i=0;i<x->n;i++){
        if (x->chaves[i] == k){
            ret = i;
        }
    }
    return ret;
}

int procura_ic(tnode *x, tchave k){
    int i;
    i = x->n-1;
    while (k < x->chaves[i] && i >= 0){
        i-=1;
    }
    i+=1;
    return i;
}

int pega_irmao_maior(tnode *x, int cpos){
    int ant;
    int pos;
    int ret;
    ant = cpos -1;
    pos = cpos +1;
    if (ant < 0){
        ret = pos;
    }else if (pos > x->n){
        ret = ant;
    }else{
        if (x->c[ant]->n > x->c[pos]->n){
            ret = ant;
        }else{
            ret = pos;
        }
    }
    return ret;


}

void remove_chave(tnode *x, tchave ik){
    int i;
    for (i=ik;i<x->n-1;i++){
            x->chaves[i] = x->chaves[i+1];
    }
    x->n -= 1;
}

tchave sucessor(tnode *x, tchave k){
    int ik;
    tnode * aux;
    ik = procura_chave(x,k);
    aux = x->c[ik+1];
    while(!aux->folha)
        aux = x->c[0];
    return aux->chaves[0];
}

tchave predecessor(tnode *x, tchave k){
    int ik;
    tnode * aux;
    ik = procura_chave(x,k);
    aux = x->c[ik];
    while(!aux->folha)
        aux = x->c[x->n];
    return aux->chaves[aux->n-1];
}
void merge(tarv *parv, tnode *x, int cpos){
    tnode *y;
    tnode *z;
    int i;
    y = x->c[cpos];
    z = x->c[cpos+1];
    /*copia elementos e chaves de z em y */
    y->chaves[y->n] = x->chaves[cpos];
    for (i = 0;i<z->n;i++){
        y->chaves[y->n+1+i] = z->chaves[i];
        if (!y->folha){
            y->c[y->n+1+i] = z->c[i];
        }
    }
    if (!y->folha){
        for (i = 0;i<=z->n;i++)
            y->c[y->n+1+i] = z->c[i];
    }
    y->n = 2*(parv->t)-1;
    /*arruma x com o elemento a menos */
    for (i=cpos;i<x->n-1;i++){
        x->chaves[i] = x->chaves[i+1];
        x->c[i+1] = x->c[i+2];
    }
    x->n -= 1;
    if (x->n == 0){ /* trata o caso quando o merge apaga o faz join com o ultimo elemento de x*/
        for (i=0;i<y->n;i++){
            x->chaves[i] = y->chaves[i];
            x->c[i] = y->c[i];
        }
        x->c[i] = y->c[i];
        x->folha = y->folha;
        x->n = 2*(parv->t) -1;
        free(y);
    }
    free(z);
}

void pega_emprestado_irmao(tnode *x, int cpos,int irmao){
    int i;
    tnode *y,*z;
    if (cpos < irmao){ /* irmao a direita*/
        y = x->c[cpos];
        z = x->c[cpos+1];
        y->chaves[y->n] = x->chaves[cpos];
        x->chaves[cpos] = z->chaves[0];
        y->c[y->n+1] = z->c[0];
        for (i=0;i < z->n-1;i++){
            z->chaves[i] = z->chaves[i+1];
            z->c[i] = z->c[i+1];
        }
        z->c[i] = z->c[i+1];
        z->n-=1;
        y->n+=1;
    }else{ /*irmao a esquerda*/
        cpos -=1;
        y = x->c[cpos];
        z = x->c[cpos+1];
        for (i=z->n;i>0;i--){
            z->chaves[i] = z->chaves[i-1];
            z->c[i+1] = z->c[i];
        }
        z->chaves[0] = x->chaves[cpos];
        z->c[0] = y->c[y->n];
        x->chaves[cpos] = y->chaves[y->n-1];
        y->n-=1;
        z->n+=1;
    }

}
int _btree_remove(tarv *parv, tnode * x, tchave k){
    int ik; /* key position */
    int t;
    int i;
    int iirmao_maior;
    tnode *y,*z;
    tchave klinha;
    int ret;
    ret = 1;
    t = parv->t;
    ik = procura_chave(x,k); 
    if (ik >=0 ){ /* k in x*/
        if (x->folha){ /* Caso 1*/
            remove_chave(x,ik);
        }else{ /* Caso 2 */
            y = x->c[ik];
            z = x->c[ik+1];
            if (y->n >= t){ /* a */
                klinha = predecessor(x,k);
                x->chaves[ik] = klinha;
                ret = _btree_remove(parv,y,klinha);

            }else if (z->n >= t){ /* b */
                klinha = sucessor(x,k);
                x->chaves[ik] = klinha;
                ret = _btree_remove(parv,z,klinha);
            }else if ((y->n == t -1) && (z->n == t-1)){ /* c */
                merge(parv,x,ik);
                ret = _btree_remove(parv,x,k);
            }
        }
    }else{ /* Caso 3 - k not in x */
        if (x->folha == TRUE){
            ret = 0;
        }else{
            i = procura_ic(x,k);
            if (x->c[i]->n == t-1){
                iirmao_maior = pega_irmao_maior(x,i);
                if (x->c[iirmao_maior]->n >=t){            /* a */
                    pega_emprestado_irmao(x,i,iirmao_maior);
                   ret = _btree_remove(parv,x->c[i],k);
                }else if (x->c[iirmao_maior]->n == t-1){   /* b */
                    merge(parv,x,menor(i,iirmao_maior)); 
                   ret = _btree_remove(parv,x,k);
                }
            }else{
                ret = _btree_remove(parv,x->c[i],k);
            }
        }
    }
    return ret;
}

int btree_remove(tarv *parv, tchave k){
    printf("Removendo %d\n",k);
    return _btree_remove(parv,parv->raiz,k);
}



int btree_split(tarv * parv,tnode * x, int i){
    tnode * z;
    tnode * y;
    int t;
    int j;
    int ret;
    t = parv->t;
    y = x->c[i];
    z = aloca_node(parv);
    z->n = t-1;
    z->folha = y->folha;
    for (j=0;j<t-1;j++)
        z->chaves[j] = y->chaves[j+t];
    if (!z->folha){
        for(j=0;j<t;j++)
            z->c[j] = y->c[j+t];
    }
    y->n = t-1;

    /*prepara os ponteiros x*/
    for(j=x->n;j>=i+1;j--)
        x->c[j+1] = x->c[j];
    x->c[i+1] = z;

    for(j=x->n-1;j>=i;j--)
        x->chaves[j+1] = x->chaves[j];    
    x->chaves[i] = y->chaves[t-1];
    x->n +=1;


    return ret;
}

int btree_insere_naocheio(tarv *parv, tnode * x, tchave k){
    int i,t,ret;
    i = x->n;
    t = parv->t;
    ret = 1;
    if (x->folha == TRUE){
        while(i>0 && k < x->chaves[i-1]){
            x->chaves[i] = x->chaves[i-1];
            i = i - 1;
        }
        x->chaves[i] = k;
        x->n = x->n+1;
    }else{
        i -= 1;
        while(i>=0 && k < x->chaves[i]){
            i = i - 1;
        }
        i+=1;

        if (x->c[i]->n == 2*t -1){
            ret = btree_split(parv,x,i);
            if (k > x->chaves[i])
                i = i+1;
        }
        ret = btree_insere_naocheio(parv,x->c[i],k);
    }
    return ret;
}





int btree_insere(tarv *parv, tchave k){
    tnode * r;
    tnode * s;
    int t;
    int ret = 1;
    
    r = parv->raiz;
    t = parv->t;
    if (r->n == 2*t-1){
        s = aloca_node(parv);
        parv->raiz = s;
        s->folha = FALSE;
        s->n = 0;
        s->c[0] = r;
        ret = btree_split(parv,s,0);
        if (ret)
           ret = btree_insere_naocheio(parv,s,k);
    }else{
        ret = btree_insere_naocheio(parv,r,k);
    }
    return ret;
    
}


void print_node(tnode *x){
    int i;
    for (i=0;i<x->n;i++){
        printf("%d ",x->chaves[i]);
    }
    printf("\n");
}




void test_btree_split(){
    tarv arv;
    int t = 3;
    tnode * x;
    tnode * y;
    tnode * z;

    btree_constroi(&arv,t);
    x = arv.raiz;
    y = aloca_node(&arv);

    y->folha = TRUE;
    y->n = 5;
    y->chaves[0] = 10;
    y->chaves[1] = 20;
    y->chaves[2] = 30;
    y->chaves[3] = 40;
    y->chaves[4] = 50;
    
    x->c[0] = y;
    x->n = 0;
    x->folha = FALSE;
    arv.raiz = x;

    btree_split(&arv,x,0);

    x = arv.raiz;
    y = x->c[0];
    z = x->c[1];
    assert(x->chaves[0] == 30);
    assert(x->folha == FALSE);
    assert(x->n == 1);

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->n == 2);
    assert(y->folha  == TRUE);

    assert(z->chaves[0] == 40);
    assert(z->chaves[1] == 50);
    assert(z->n == 2);
    assert(z->folha  == TRUE);
}

void test_btree_insere_naocheio(){
    tarv arv;
    int t = 3;

    tnode * x;
    tnode * y;

    btree_constroi(&arv,t);
    x = arv.raiz;
    y = aloca_node(&arv);

    y->folha = TRUE;
    y->n = 5;
    y->chaves[0] = 10;
    y->chaves[1] = 20;
    y->chaves[2] = 30;
    y->chaves[3] = 40;
    y->chaves[4] = 50;
    
    x->c[0] = y;
    x->n = 0;
    x->folha = FALSE;
    arv.raiz = x;

    btree_split(&arv,x,0);

    btree_insere_naocheio(&arv,x,23);

    y = x->c[0];

    
    assert(y->chaves[2] == 23);
    assert(y->n == 3);

    btree_insere_naocheio(&arv,x,24);

    y = x->c[0];
    assert(y->chaves[3] == 24);
    assert(y->n == 4);

    btree_insere_naocheio(&arv,x,25);
    btree_insere_naocheio(&arv,x,26);

    x = arv.raiz; 


    assert(x->chaves[0] == 23);
    assert(x->chaves[1] == 30);
    assert(x->n == 2);
    assert(x->folha == FALSE);


    y = x->c[0];

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->n == 2);
    assert(y->folha == TRUE);

    y = x->c[1];

    assert(y->chaves[0] == 24);
    assert(y->chaves[1] == 25);
    assert(y->chaves[2] == 26);
    assert(y->n == 3);
    assert(y->folha == TRUE);

    y = x->c[2];

    assert(y->chaves[0] == 40);
    assert(y->chaves[1] == 50);
    assert(y->n == 2);
    assert(y->folha == TRUE);
}


void test_btree_insere(){
    tarv arv;
    tnode *x, *y;
    int t; 
    int i;
    t = 3;

    btree_constroi(&arv,t);

    for (i=10;i<=60;i+=10)
        btree_insere(&arv,i);

    x=arv.raiz;
    assert(x->chaves[0] == 30);
    assert(x->n == 1);
    assert(x->folha == FALSE);

    y = x->c[0];

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->n == 2);
    assert(y->folha==TRUE);

    y = x->c[1];

    assert(y->chaves[0] == 40);
    assert(y->chaves[1] == 50);
    assert(y->chaves[2] == 60);
    assert(y->n == 3);
    assert(y->folha==TRUE);


    
    for (i=70;i<100;i+=10)
        btree_insere(&arv,i);

    x=arv.raiz;
    assert(x->chaves[0] == 30);
    assert(x->chaves[1] == 60);
    assert(x->n == 2);
    assert(x->folha == FALSE);

    y = x->c[0];

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->n == 2);
    assert(y->folha==TRUE);

    y = x->c[1];

    assert(y->chaves[0] == 40);
    assert(y->chaves[1] == 50);
    assert(y->n == 2);
    assert(y->folha==TRUE);

    y = x->c[2];

    assert(y->chaves[0] == 70);
    assert(y->chaves[1] == 80);
    assert(y->chaves[2] == 90);
    assert(y->n == 3);
    assert(y->folha==TRUE);


    btree_insere(&arv,21);
    btree_insere(&arv,22);
    btree_insere(&arv,23);
    btree_insere(&arv,24);

    x=arv.raiz;
    assert(x->chaves[0] == 21);
    assert(x->chaves[1] == 30);
    assert(x->chaves[2] == 60);
    assert(x->n == 3);
    assert(x->folha == FALSE);

    y = x->c[0];

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->n == 2);
    assert(y->folha==TRUE);

    y = x->c[1];

    assert(y->chaves[0] == 22);
    assert(y->chaves[1] == 23);
    assert(y->chaves[2] == 24);
    assert(y->n == 3);
    assert(y->folha==TRUE);

    btree_insere(&arv,51);
    btree_insere(&arv,52);
    btree_insere(&arv,53);
    btree_insere(&arv,54);

    btree_insere(&arv,91);
    btree_insere(&arv,92);
    btree_insere(&arv,93);
    btree_insere(&arv,94);

    x=arv.raiz;
    assert(x->chaves[0] == 51);
    assert(x->n == 1);
    assert(x->folha == FALSE);

    y = x->c[0];

    assert(y->chaves[0] == 21);
    assert(y->chaves[1] == 30);
    assert(y->n == 2);
    assert(y->folha==FALSE);

    y = x->c[1];

    assert(y->chaves[0] == 60);
    assert(y->chaves[1] == 90);
    assert(y->n == 2);
    assert(y->folha==FALSE);

    imprime_arvore(arv.raiz,0,0);

}


void test_btree_merge(){
    tarv arv;
    tnode *x, *y;
    int t; 
    int i;
    t = 3;

    btree_constroi(&arv,t);

    for (i=10;i<=60;i+=10)
        btree_insere(&arv,i);


    for (i=70;i<100;i+=10)
        btree_insere(&arv,i);
    btree_insere(&arv,21);
    btree_insere(&arv,22);
    btree_insere(&arv,23);
    btree_insere(&arv,24);

    btree_insere(&arv,51);
    btree_insere(&arv,52);
    btree_insere(&arv,53);
    btree_insere(&arv,54);

    btree_insere(&arv,91);
    btree_insere(&arv,92);
    btree_insere(&arv,93);
    btree_insere(&arv,94);

    x = arv.raiz;
    merge(&arv,x,0);
    
    assert(x->chaves[0] == 21);
    assert(x->chaves[1] == 30);
    assert(x->chaves[2] == 51);
    assert(x->chaves[3] == 60);
    assert(x->chaves[4] == 90);
    assert(x->n == 5);
    assert(x->folha == FALSE);

    y = x->c[0];

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->n == 2);
    assert(y->folha == TRUE);

    y = x->c[1];

    assert(y->chaves[0] == 22);
    assert(y->chaves[1] == 23);
    assert(y->chaves[2] == 24);
    assert(y->n == 3);
    assert(y->folha == TRUE);

    y = x->c[2];

    assert(y->chaves[0] == 40);
    assert(y->chaves[1] == 50);
    assert(y->n == 2);
    assert(y->folha == TRUE);

    y = x->c[3];

    assert(y->chaves[0] == 52);
    assert(y->chaves[1] == 53);
    assert(y->chaves[2] == 54);
    assert(y->n == 3);
    assert(y->folha == TRUE);

    y = x->c[4];

    assert(y->chaves[0] == 70);
    assert(y->chaves[1] == 80);
    assert(y->n == 2);
    assert(y->folha == TRUE);


    y = x->c[5];

    assert(y->chaves[0] == 91);
    assert(y->chaves[1] == 92);
    assert(y->chaves[2] == 93);
    assert(y->chaves[3] == 94);
    assert(y->n == 4);
    assert(y->folha == TRUE);
}

void test_btree_merge2(){
    tarv arv;
    tnode *x, *y;
    int t; 
    int i;
    t = 3;

    btree_constroi(&arv,t);

    for (i=10;i<=60;i+=10)
        btree_insere(&arv,i);


    for (i=70;i<100;i+=10)
        btree_insere(&arv,i);
    btree_insere(&arv,21);
    btree_insere(&arv,22);
    btree_insere(&arv,23);
    btree_insere(&arv,24);

    btree_insere(&arv,51);
    btree_insere(&arv,52);
    btree_insere(&arv,53);
    btree_insere(&arv,54);

    btree_insere(&arv,91);
    btree_insere(&arv,92);
    btree_insere(&arv,93);
    btree_insere(&arv,94);

    x = arv.raiz->c[0];
    x->c[1]->n=2;
    merge(&arv,x,0);
    
    assert(x->chaves[0] == 30);
    assert(x->n == 1);
    assert(x->folha == FALSE);

    y = x->c[0];

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->chaves[2] == 21);
    assert(y->chaves[3] == 22);
    assert(y->chaves[4] == 23);
    assert(y->n == 5);
    assert(y->folha == TRUE);

    y = x->c[1];

    assert(y->chaves[0] == 40);
    assert(y->chaves[1] == 50);
    assert(y->n == 2);
    assert(y->folha == TRUE);

}
void test_btree_merge3(){
    tarv arv;
    tnode *x, *y; 
    int t; 
    int i;
    t = 3;

    btree_constroi(&arv,t);

    for (i=10;i<=60;i+=10)
        btree_insere(&arv,i);


    for (i=70;i<100;i+=10)
        btree_insere(&arv,i);
    btree_insere(&arv,21);
    btree_insere(&arv,22);
    btree_insere(&arv,23);
    btree_insere(&arv,24);

    btree_insere(&arv,51);
    btree_insere(&arv,52);
    btree_insere(&arv,53);
    btree_insere(&arv,54);

    btree_insere(&arv,91);
    btree_insere(&arv,92);
    btree_insere(&arv,93);
    btree_insere(&arv,94);

    x = arv.raiz->c[0];
    x->c[1]->n=2;
    merge(&arv,x,1);
    
    assert(x->chaves[0] == 21);

    y = x->c[0];

    assert(y->chaves[0] == 10);
    assert(y->chaves[1] == 20);
    assert(y->n == 2);
    assert(y->folha == TRUE);

    y = x->c[1];
    assert(y->chaves[0] == 22);
    assert(y->chaves[1] == 23);
    assert(y->chaves[2] == 30);
    assert(y->chaves[3] == 40);
    assert(y->chaves[4] == 50);
    assert(y->n == 5);
    assert(y->folha == TRUE);

}


void test_btree_empresta_irmao(){
    tarv arv;
    tnode *x, *y, *z;
    int t; 
    int i;
    t = 3;

    btree_constroi(&arv,t);

    for (i=10;i<=60;i+=10)
        btree_insere(&arv,i);


    for (i=70;i<100;i+=10)
        btree_insere(&arv,i);
    btree_insere(&arv,21);
    btree_insere(&arv,22);
    btree_insere(&arv,23);
    btree_insere(&arv,24);

    btree_insere(&arv,51);
    btree_insere(&arv,52);
    btree_insere(&arv,53);
    btree_insere(&arv,54);

    btree_insere(&arv,91);
    btree_insere(&arv,92);
    btree_insere(&arv,93);
    btree_insere(&arv,94);

    x = arv.raiz->c[0];
    pega_emprestado_irmao(x,0,1);

    y = x->c[0];
    z = x->c[1];

    assert(x->chaves[0]==22);
    assert(x->chaves[1]==30);
    assert(x->n == 2);
    assert(x->folha == FALSE);

    assert(y->chaves[0]==10);
    assert(y->chaves[1]==20);
    assert(y->chaves[2]==21);
    assert(y->n == 3);
    assert(y->folha == TRUE);


    assert(z->chaves[0]==23);
    assert(z->chaves[1]==24);
    assert(z->n == 2);
    assert(z->folha == TRUE);

   


}


void test_btree_empresta_irmao2(){
    tarv arv;
    tnode *x, *y, *z;
    int t; 
    int i;
    t = 3;

    btree_constroi(&arv,t);

    for (i=10;i<=60;i+=10)
        btree_insere(&arv,i);


    for (i=70;i<100;i+=10)
        btree_insere(&arv,i);
    btree_insere(&arv,21);
    btree_insere(&arv,22);
    btree_insere(&arv,23);
    btree_insere(&arv,24);

    btree_insere(&arv,51);
    btree_insere(&arv,52);
    btree_insere(&arv,53);
    btree_insere(&arv,54);

    btree_insere(&arv,91);
    btree_insere(&arv,92);
    btree_insere(&arv,93);
    btree_insere(&arv,94);

    x = arv.raiz->c[0];
    pega_emprestado_irmao(x,0,1);

    y = x->c[0];
    z = x->c[1];

    assert(x->chaves[0]==22);
    assert(x->chaves[1]==30);
    assert(x->n == 2);
    assert(x->folha == FALSE);

    assert(y->chaves[0]==10);
    assert(y->chaves[1]==20);
    assert(y->chaves[2]==21);
    assert(y->n == 3);
    assert(y->folha == TRUE);


    assert(z->chaves[0]==23);
    assert(z->chaves[1]==24);
    assert(z->n == 2);
    assert(z->folha == TRUE);

   


}


void test_btree_remove(){
    tarv arv;
    int t;
    tnode * x;
    t=3;




    btree_constroi(&arv,t);
    btree_insere(&arv,10);
    btree_insere(&arv,20);
    btree_insere(&arv,30);
    btree_insere(&arv,40);
    btree_insere(&arv,50);
    btree_insere(&arv,60);

    x = arv.raiz;


    assert(x->chaves[0] == 30);

    btree_remove(&arv,10);
    btree_remove(&arv,20);
    btree_remove(&arv,30);
    btree_remove(&arv,40);
    btree_remove(&arv,50);

    assert(x->chaves[0] == 60);
    assert(x->n == 1);
    assert(x->folha == TRUE);

    btree_insere(&arv,10);
    btree_insere(&arv,20);
    btree_insere(&arv,30);
    btree_insere(&arv,40);
    btree_insere(&arv,50);

    x = arv.raiz;
    
    imprime_arvore(arv.raiz,0,0);
    assert(x->chaves[0] == 30);
    btree_remove(&arv,10);
    imprime_arvore(arv.raiz,0,0);
    assert(x->chaves[0] == 40);
    btree_remove(&arv,40);
    imprime_arvore(arv.raiz,0,0);
    assert(x->chaves[0] == 20);
}

void test_btree_remove2(){
    tarv arv;
    int t; 
    int i;
    t = 3;

    btree_constroi(&arv,t);

    for (i=10;i<=60;i+=10)
        btree_insere(&arv,i);

   
    for (i=70;i<100;i+=10)
        btree_insere(&arv,i);

    btree_insere(&arv,21);
    btree_insere(&arv,22);
    btree_insere(&arv,23);
    btree_insere(&arv,24);


    btree_insere(&arv,51);
    btree_insere(&arv,52);
    btree_insere(&arv,53);
    btree_insere(&arv,54);

    btree_insere(&arv,91);
    btree_insere(&arv,92);
    btree_insere(&arv,93);
    btree_insere(&arv,94);


    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,24);

    imprime_arvore(arv.raiz,0,0);
    
    btree_remove(&arv,50);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,51);
    imprime_arvore(arv.raiz,0,0);
    
    btree_remove(&arv,54);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,60);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,30);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,21);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,22);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,23);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,80);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,90);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,91);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,92);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,52);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,40);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,20);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,53);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,10);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,70);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,93);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,94);
    imprime_arvore(arv.raiz,0,0);

    btree_remove(&arv,94);
    imprime_arvore(arv.raiz,0,0);


}

int main(void){
    test_btree_split();
    test_btree_insere_naocheio();
    test_btree_insere();

    test_btree_merge();
    test_btree_merge2();
    test_btree_merge3();
    test_btree_empresta_irmao();
    test_btree_empresta_irmao2();


    test_btree_remove();
    test_btree_remove2();

    return 0;
}
