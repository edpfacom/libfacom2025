#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

/* DECLARACOES */
typedef struct {
    int *v;
    int n;
    int max;
}theap;
/* FUNCOES HEAP */
void constroi_heap(theap * heap, int max){
    heap->v = malloc(sizeof(int)*max);
    heap->n = 0;
    heap->max = max;
}
void destroi_heap(theap * heap){
    free(heap->v);
    heap->v = NULL;
    heap->n = 0;
    heap->max = 0;
}
int pai(int i){
    int ret = 0;
    if (i>0)
        ret = (i-1)/2;
    return ret;
}
int filho_esq(int i){
    return i*2 +1;
}
int filho_dir(int i){
    return i*2 +2;
}

void troca(int *a,int *b){
    int aux = *a;
    *a  = *b;
    *b  = aux;
}
void desce(theap * heap,int i){
    int e = filho_esq(i);
    int d = filho_dir(i);
    int imax = i;
    if (e < heap->n && heap->v[e] > heap->v[imax])
        imax = e;
    if (d < heap->n && heap->v[d] > heap->v[imax])
        imax = d;
    if (imax != i){
        troca(&(heap->v[i]),&(heap->v[imax]));
        desce(heap,imax);
    }
}

/* FUNCOES TESTE*/
void test_constroi_heap(){
    /*inicializacao*/
    theap heap;
    int max = 20;
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    
    /*testes*/
    assert(heap.v != NULL);
    assert(heap.n == 0);
    assert(heap.max == 20);
    free(heap.v);
}

void test_destroi_heap(){
    /*inicializacao*/
    theap heap;
    int max = 20;
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    destroi_heap(&heap);
    
    /*testes*/
    assert(heap.v == NULL);
    assert(heap.n == 0);
    assert(heap.max == 0);
}

void test_pai(){
    assert(pai(0)==0);
    assert(pai(1)==0);
    assert(pai(2)==0);
    assert(pai(3)==1);
    assert(pai(4)==1);
}
void test_filhos(){
    assert(filho_esq(0)==1);
    assert(filho_dir(0)==2);
    assert(filho_esq(1)==3);
    assert(filho_dir(1)==4);
    assert(filho_esq(2)==5);
    assert(filho_dir(2)==6);
}
void test_desce(){
    /*inicializacao*/

    theap heap;
    int max = 20;
    int vi[] = {3,20,10,15,17,9,7,4,5,8,2,11,12,13,14};
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    heap.v = memcpy(heap.v,vi,sizeof(vi));
    heap.n = 15;
    desce(&heap,0); 
    /*testes*/
    assert(heap.v[0]==20);
    assert(heap.v[1]==17);
    assert(heap.v[4]==8);
    assert(heap.v[9]==3);
}

int main(void){
    test_constroi_heap();
    test_destroi_heap();
    test_pai();
    test_filhos();
    test_desce();
    printf("SUCESSO!\n");
    return EXIT_SUCCESS;
}


