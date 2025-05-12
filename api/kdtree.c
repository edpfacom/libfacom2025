#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<string.h>
#include<assert.h>

/*Definições desenvolvedor usuario*/
typedef struct _reg{
    int lat;
    int lon;
    char nome[100];
}treg;

void * aloca_reg(int lat, int lon, char nome[]){
    treg * reg;
    reg = malloc(sizeof(treg));
    reg->lat = lat;
    reg->lon = lon;
    strcpy(reg->nome,nome);
    return reg;
}

int comparador(void *a, void *b, int pos){
    int ret;
    if (pos == 0){
        ret = ((treg *)a)->lat - ((treg *)b)->lat;
    }else if (pos == 1){
        ret = ((treg *)a)->lon - ((treg *)b)->lon;
    }
    return ret;
}

double distancia(void * a, void *b){
        double ret_lat = ((treg *)a)->lat - ((treg *)b)->lat;
        double ret_lon = ((treg *)a)->lon - ((treg *)b)->lon;
        return ret_lat*ret_lat + ret_lon*ret_lon;
}


/*Definições desenvolvedor da biblioteca*/
typedef struct _node{
    void * key;
    struct _node * esq;
    struct _node * dir;
}tnode;

typedef struct _arv{
    tnode * raiz;
    int (*cmp)(void *, void *, int);
    double (*dist) (void *, void *);
    int k;
}tarv;



/*funções desenvolvedor da biblioteca*/

void kdtree_constroi(tarv * arv, int (*cmp)(void *a, void *b, int ),double (*dist) (void *, void *),int k){
    arv->raiz = NULL;
    arv->cmp = cmp;
    arv->dist = dist;
    arv->k = k;
}

/*teste*/
void test_constroi(){
    /* declaracao de variaveis */
    tarv arv;
    tnode node1;
    tnode node2;

    node1.key = aloca_reg(2,3,"Dourados");
    node2.key = aloca_reg(1,1,"Campo Grande");


    /* chamada de funções */
    kdtree_constroi(&arv,comparador,distancia,2);
    
    /* testes */
    assert(arv.raiz == NULL);
    assert(arv.k == 2);
    assert(arv.cmp(node1.key,node2.key,0) == 1);
    assert(arv.cmp(node1.key,node2.key,1) == 2);
    assert(strcpy(((treg *)node1.key)->nome,"Dourados"));
    assert(strcpy(((treg *)node2.key)->nome,"Campo Grande"));
    free(node1.key);
    free(node2.key);
}

void _kdtree_insere(tnode **raiz, void * key, int (*cmp)(void *a, void *b, int),int profund, int k){
    if(*raiz == NULL){
        *raiz = malloc(sizeof(tnode));
        (*raiz)->key = key;
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
    }else{
        int pos = profund % k;
        if (cmp( (*(*raiz)).key , key ,pos) <0){
            _kdtree_insere( &((*(*raiz)).dir),key,cmp,profund + 1,k);
        }else{
            _kdtree_insere( &((*raiz)->esq),key,cmp,profund +1,k);
        }
    }
}

void kdtree_insere(tarv *arv, void *key){
    _kdtree_insere(&(arv->raiz),key,arv->cmp,0,arv->k);
}


void _kdtree_destroi(tnode * node){
    if (node!=NULL){
        _kdtree_destroi(node->esq);
        _kdtree_destroi(node->dir);
        free(node->key);
        free(node);
    }
}

void kdtree_destroi(tarv *arv){
    _kdtree_destroi(arv->raiz);
}


void _kdtree_busca(tarv *arv, tnode ** atual, void * key, int profund, double *menor_dist, tnode **menor){
    tnode ** lado_principal; 
    tnode ** lado_oposto;    
    if (*atual != NULL){
        double dist_atual = arv->dist((*atual)->key, key);
        if (dist_atual < *menor_dist){
            *menor_dist = dist_atual;
            *menor = *atual;
        }
        int pos = profund % arv->k;
        int comp = arv->cmp(key, (*atual)->key, pos);

        printf("%s dist %4.3f menor_dist %4.3f comp %d\n", ((treg *)((tnode *)*atual)->key)->nome,dist_atual,*menor_dist,comp);

        /* define lado principal para buscar */
        if (comp < 0){
            lado_principal =  &((*atual)->esq);
            lado_oposto    =  &((*atual)->dir); 
        }else{
            lado_principal =  &((*atual)->dir);
            lado_oposto    =  &((*atual)->esq); 
        }

        _kdtree_busca(arv, lado_principal, key, profund + 1, menor_dist, menor);

        /* Verifica se deve buscar também no outro lado*/

        if (comp*comp < *menor_dist) {
            printf("tentando do outro lado %f\n",comp*comp);
            _kdtree_busca(arv, lado_oposto, key, profund + 1, menor_dist, menor);
        }
    }
}


tnode * kdtree_busca(tarv *arv, void * key){
    tnode * menor = NULL;
    double menor_dist = DBL_MAX;
    _kdtree_busca(arv,&(arv->raiz),key,0,&menor_dist,&menor);
    return menor;
}

treg buscar_mais_proximo(tarv *arv, treg query) {
    double menor_dist = 1e20;
    tnode *menor = NULL;
    _kdtree_busca(arv, &(arv->raiz), &query, 0, &menor_dist, &menor);
    return *((treg *)(menor->key));
}


tarv arvore_global;

tarv* get_tree() {
    return &arvore_global;
}

void inserir_ponto(treg p) {
    treg *novo = malloc(sizeof(treg));
    *novo = p;  // cópia de estrutura
    kdtree_insere(&arvore_global,novo);
}
void kdtree_construir() {
    arvore_global.k = 2;
    arvore_global.dist = distancia;
    arvore_global.cmp = comparador;
    arvore_global.raiz = NULL;
}

void test_busca(){
    tarv arv;
    kdtree_constroi(&arv,comparador,distancia,2);
    kdtree_insere(&arv,aloca_reg(10,10,"a"));
    kdtree_insere(&arv,aloca_reg(20,20,"b"));
    kdtree_insere(&arv,aloca_reg(1,10,"c"));
    kdtree_insere(&arv,aloca_reg(3,5,"d"));
    kdtree_insere(&arv,aloca_reg(7,15,"e"));
    kdtree_insere(&arv,aloca_reg(4,11,"f"));
    tnode * raiz = arv.raiz;
    assert(strcmp(((treg *)raiz->dir->key)->nome, "b")==0);
    assert(strcmp(((treg *)raiz->esq->key)->nome, "c")==0);
    assert(strcmp(((treg *)raiz->esq->esq->key)->nome, "d")==0);
    assert(strcmp(((treg *)raiz->esq->dir->key)->nome, "e")==0);

    printf("\n");
    treg  * atual = aloca_reg(7,14,"x");
    tnode * mais_proximo = kdtree_busca(&arv,atual);
    assert(strcmp(((treg *)mais_proximo->key)->nome,"e") == 0);

    printf("\n");
    atual->lat = 9;
    atual->lon = 9;
    mais_proximo = kdtree_busca(&arv,atual);
    assert(strcmp(((treg *)mais_proximo->key)->nome,"a") == 0);

    printf("\n");
    atual->lat = 4;
    atual->lon = 5;
    mais_proximo = kdtree_busca(&arv,atual);
    assert(strcmp(((treg *)mais_proximo->key)->nome,"d") == 0);

    printf("\n");
    atual->lat = 4;
    atual->lon = 9;
    mais_proximo = kdtree_busca(&arv,atual);
    assert(strcmp(((treg *)mais_proximo->key)->nome,"f") == 0);



    free(atual);
    kdtree_destroi(&arv);
}


int main(void){
    test_constroi();
    test_busca();
    printf("SUCCESS!!\n");
    return EXIT_SUCCESS;
}
