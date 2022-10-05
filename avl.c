#include <stdio.h>
#include <stdlib.h>
#include "ArvoreAVL.h"

struct NO {
    int info;
    int h;
    struct NO* esq;
    struct NO* dir;
};

ArvAVL* cria_arvore(){
    ArvAVL* raiz = (ArvAVL*) malloc (sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void libera_no(struct NO* no){
    if(no == NULL) return;

    libera_no(no->esq);
    libera_no(no->dir);
    free(no);
    no = NULL;
}

void libera_arvore(ArvAVL* raiz){
    if(raiz == NULL) return;
    libera_no(*raiz);
    free(raiz);
}

int altura_no (struct NO* no) {
    if (no == NULL) return -1;
    else return no->h;
}

int fator_balanceamento_no (struct NO* no) {
    return labs(altura_no(no->esq) - altura_no(no->dir));
}

int maior (int x, int y) {
    if (x > y) return x;
    else return y;
}

void rotacao_ll (ArvAVL* raiz) {
    struct NO* no = (*raiz)->esq;

    (*raiz)->esq = no->dir;
    no->dir = *raiz;

    (*raiz)->h = maior(altura_no((*raiz)->esq), altura_no((*raiz)->dir)) + 1;
    no->h = maior(altura_no(no->esq), (*raiz)->h) + 1;

    *raiz = no;
}

void rotacao_rr (ArvAVL* raiz) {
    struct NO* no = (*raiz)->dir;

    (*raiz)->dir = no->esq;
    no->esq = *raiz;

    (*raiz)->h = maior(altura_no((*raiz)->esq), altura_no((*raiz)->dir)) + 1;
    no->h = maior(altura_no(no->dir), (*raiz)->h) + 1;

    *raiz = no;
}

void rotacao_lr (ArvAVL* raiz) {
    rotacao_rr(&(*raiz)->esq);
    rotacao_ll(raiz);
}

void rotacao_rl (ArvAVL* raiz) {
    rotacao_ll(&(*raiz)->dir);
    rotacao_rr(raiz);
}

int insere_arvore (ArvAVL* raiz, int valor) {
    int res;

    if (*raiz == NULL) {
        struct NO* novo = (struct NO*) malloc (sizeof (struct NO));

        if(novo == NULL) return 0;

        novo->info = valor;
        novo-> h = 0;
        novo->esq = NULL;
        novo->dir = NULL;

        *raiz = novo;
        return 1;
    }

    struct NO* atual = *raiz;

    if (valor < atual->info) {
        if ((res = insere_arvore(&(atual->esq), valor)) == 1) {
            if (fator_balanceamento_no(atual) >= 2) {
                if (valor < (*raiz)->esq->info) 
                    rotacao_ll(raiz);
                else   
                    rotacao_lr(raiz);
            }
        }
    }
    else {
        if ((res = insere_arvore(&(atual->dir), valor)) == 1) {
            if (fator_balanceamento_no(atual) >= 2) {
                if ((*raiz)->dir->info < valor) 
                    rotacao_rr(raiz);
                else   
                    rotacao_rl(raiz);
            }
        }
        else {
            printf ("Valor duplicado!\n");
            return 0;
        }
    }

    atual->h = maior(altura_no(atual->esq), altura_no(atual->dir)) + 1;
    return res;
}

struct NO* procura_menor (struct NO* atual) {
    struct NO* no1 = atual;
    struct NO* no2 = atual->esq;

    while(no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }

    return no1;
}

int remove_arvore (ArvAVL* raiz, int valor) {
    if (*raiz == NULL) {
        printf ("Valor não existe!\n");
        return 0;
    }

    int res;

    if (valor < (*raiz)->info) {
        if ((res = remove_arvore(&(*raiz)->esq, valor)) == 1) {
            if (fator_balanceamento_no(*raiz) >= 2) {
                if (altura_no((*raiz)->dir->esq) <= altura_no((*raiz)->dir->dir))
                    rotacao_rr(raiz);
                else   
                    rotacao_rl(raiz);
            }
        }
    }

    if ((*raiz)->info < valor) {
        if ((res = remove_arvore(&(*raiz)->dir, valor)) == 1) {
            if (fator_balanceamento_no(*raiz) >= 2) {
                if (altura_no((*raiz)->esq->dir) <= altura_no((*raiz)->esq->esq))
                    rotacao_ll(raiz);
                else   
                    rotacao_lr(raiz);
            }
        }
    }

    if ((*raiz)->info == valor) {
        if (((*raiz)->esq == NULL || (*raiz)->dir == NULL)) {
            struct NO* oldNode = (*raiz);

            if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;

            free(oldNode); 
        }
        else {
            struct NO* temp = procura_menor((*raiz)->dir);
            (*raiz)->info = temp->info;
            remove_arvore(&(*raiz)->dir, (*raiz)->info);

            if(fator_balanceamento_no(*raiz) >= 2) {
                if(altura_no((*raiz)->esq->dir) <= altura_no((*raiz)->esq->esq))
                    rotacao_ll(raiz);
                else   
                    rotacao_lr(raiz);
            }
        }
        return 1;
    }
    return res;
} 

void preOrdem (ArvAVL* raiz){
    if(raiz == NULL) return;

    if(*raiz != NULL){
        printf("%d\n",(*raiz)->info);
        preOrdem(&((*raiz)->esq));
        preOrdem(&((*raiz)->dir));
    }
}

void emOrdem(ArvAVL* raiz){
    if(raiz == NULL) return;

    if(*raiz != NULL){
        emOrdem(&((*raiz)->esq));
        printf("%d\n",(*raiz)->info);
        emOrdem(&((*raiz)->dir));
    }
}

void posOrdem(ArvAVL* raiz){
    if(raiz == NULL) return;

    if(*raiz != NULL){
        posOrdem(&((*raiz)->esq));
        posOrdem(&((*raiz)->dir));
        printf("%d\n",(*raiz)->info);
    }
}
