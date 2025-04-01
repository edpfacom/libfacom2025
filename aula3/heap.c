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

void constroi_max_heap(theap * heap){
    int i;
    for (i=pai(heap->n-1);i>=0;i--)
        desce(heap,i);

}

int acessa_max(theap * heap){
    return heap->v[0];
}


int heap_extrai_max(theap * heap){
    int max = heap->v[0];
    int i = heap->n -1;
    heap->v[0] = heap->v[i];
    heap->n = i;
    desce(heap,0);
    return max;
}

void sobe(theap * heap,int i){
    int p;
    if ((i > 0) && (i < heap->n)){
        p = pai(i);
        while (heap->v[p] < heap->v[i]){
            troca(&heap->v[p],&heap->v[i]);
            i = p; 
            p = pai(i);
             
        }
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
void test_constroi_max_heap(){

    /*inicializacao*/

    theap heap;
    int max = 20;
    int vi[] = {9,4,8,6,26,16,14,18,12,11};
    int vo[] = {26,18,16,12,11,8,14,6,9,4};
    int i;
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    heap.v = memcpy(heap.v,vi,sizeof(vi));
    heap.n = 10;
    constroi_max_heap(&heap);
    /*testes*/
    for (i=0;i<heap.n;i++)
        assert(heap.v[i]==vo[i]);
}

void test_acessa_max(){
    /*inicializacao*/

    theap heap;
    int max = 20;
    int v1[] = {9,4,8,6,26,16,14,18,12,11};
    int v2[] = {26,18,16,12,11,8,14,6,9,27};
    int v3[] = {26,18,16,12,11,8,14,28,9,2};
    int v4[] = {26,18,16,12,11,29,14,6,9,4};
    int i;
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    heap.n = 10;
    /*test1*/
    heap.v = memcpy(heap.v,v1,sizeof(v1));
    constroi_max_heap(&heap);
    max = acessa_max(&heap);
    assert(max == 26);

    /*test2*/
    heap.v = memcpy(heap.v,v2,sizeof(v1));
    constroi_max_heap(&heap);
    max = acessa_max(&heap);
    assert(max == 27);


    /*test3*/
    heap.v = memcpy(heap.v,v3,sizeof(v1));
    constroi_max_heap(&heap);
    max = acessa_max(&heap);
    assert(max == 28);


    /*test4*/
    heap.v = memcpy(heap.v,v4,sizeof(v1));
    constroi_max_heap(&heap);
    max = acessa_max(&heap);
    assert(max == 29);

}
void test_extrai_max(){
   /*inicializacao*/

    theap heap;
    int max = 20;
    int vi[] = {26,18,16,12,11,8,14,6,9,4};
    int i;
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    heap.v = memcpy(heap.v,vi,sizeof(vi));
    heap.n = 10;
    max = heap_extrai_max(&heap);
    assert(max == 26);
    assert(heap.n == 9);
    assert(heap.v[0]==18);
    assert(heap.v[1]==12);
    assert(heap.v[3]==9);
    assert(heap.v[8]==4);
}
void test_sobe(){
   /*inicializacao*/

    theap heap;
    int max = 20;
    int vi[] = {26,18,16,12,11,8,14,6,9,19};
    int i;
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    heap.v = memcpy(heap.v,vi,sizeof(vi));
    heap.n = 10;
    sobe(&heap,0);
    sobe(&heap,1);
    sobe(&heap,2);
    sobe(&heap,9);

    /*teste*/
    assert(heap.v[1]==19);
    assert(heap.v[4]==18);
    assert(heap.v[9]==11);

}
void test_insere(){
   /*inicializacao*/

    theap heap;
    int max = 20;
    int vi[] = {26,18,16,12,11,8,14,6,9,4};
    int i;
    
    /*chamada funcoes*/
    constroi_heap(&heap,max);
    heap.v = memcpy(heap.v,vi,sizeof(vi));
    heap.n = 10;
    /*heap_insere(&heap,30);*/

    /*teste*/
    assert(heap.v[1]==19);
    assert(heap.v[4]==18);
    assert(heap.v[9]==11);

}


int main(void){
    test_constroi_heap();
    test_destroi_heap();
    test_pai();
    test_filhos();
    test_desce();
    test_constroi_max_heap();
    test_acessa_max();
    test_extrai_max();
    test_sobe();
    /*test_insere();*/
    printf("SUCESSO!\n");
    return EXIT_SUCCESS;
}


