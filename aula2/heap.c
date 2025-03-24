#include<stdio.h>
#include<stdlib.h>
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
}

int main(void){
    test_constroi_heap();
    printf("SUCESSO!\n");
    return EXIT_SUCCESS;
}

