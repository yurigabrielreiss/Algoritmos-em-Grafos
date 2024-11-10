#include <stdio.h>
#include <stdlib.h>

typedef struct elemento elemento;
typedef struct linhas linhas;
typedef struct matriz_espa matriz_espa;

struct elemento {
    int item;
    int coluna;
    elemento *next;
};

struct linhas {
    int linha;
    linhas *next;
    elemento *inicio;
};

struct matriz_espa {
    int n_linhas;
    int n_colunas;
    linhas *inicio;
};

// funcoes basicas para gerenciar as matrizes
matriz_espa* criaMatriz(int l, int c) {
    matriz_espa *matriz = (matriz_espa*) malloc(sizeof(matriz_espa));
    matriz->n_colunas = c;
    matriz->n_linhas = l;
    matriz->inicio = NULL;
    return matriz;
}

void inserirElemento(matriz_espa *matriz, int linha, int coluna, int item) {
    if (item == 0) 
        return;

    linhas *linha_atual = matriz->inicio;
    linhas *linha_anterior = NULL;

    while (linha_atual != NULL && linha_atual->linha < linha) {
        linha_anterior = linha_atual;
        linha_atual = linha_atual->next;
    }

    if (linha_atual == NULL || linha_atual->linha != linha) {
        linhas *linha_nova = (linhas*) malloc(sizeof(linhas));
        linha_nova->linha = linha;
        linha_nova->inicio = NULL;
        linha_nova->next = linha_atual;

        if (linha_anterior == NULL)
            matriz->inicio = linha_nova;
        else
            linha_anterior->next = linha_nova;

        linha_atual = linha_nova;
    }

    elemento *elemento_atual = linha_atual->inicio;
    elemento *elemento_anterior = NULL;

    while (elemento_atual != NULL && elemento_atual->coluna < coluna) {
        elemento_anterior = elemento_atual;
        elemento_atual = elemento_atual->next;
    }

    if (elemento_atual != NULL && elemento_atual->coluna == coluna)
        elemento_atual->item = item;
    else {
        elemento *elemento_novo = (elemento*) malloc(sizeof(elemento));
        elemento_novo->item = item;
        elemento_novo->coluna = coluna;
        elemento_novo->next = elemento_atual;

        if (elemento_anterior == NULL)
            linha_atual->inicio = elemento_novo;
        else
            elemento_anterior->next = elemento_novo;
    }
}

int buscaElemento(matriz_espa *matriz, int linha, int coluna) {
    linhas *linha_atual = matriz->inicio;

    while (linha_atual != NULL && linha_atual->linha != linha)
        linha_atual = linha_atual->next;

    if (linha_atual == NULL)
        return 0;

    elemento *elemento_atual = linha_atual->inicio;

    while (elemento_atual != NULL && elemento_atual->coluna != coluna)
        elemento_atual = elemento_atual->next;

    if (elemento_atual != NULL)
        return elemento_atual->item;
    else
        return 0;
}

void imprimirMatriz(matriz_espa *matriz) {
    for (int i = 0; i < matriz->n_linhas; i++) {
        for (int j = 0; j < matriz->n_colunas; j++) {
            printf(" %d ", buscaElemento(matriz, i, j));
        }
        printf("\n");
    }
}

// operacoes com matrizes
void apagaMatriz(matriz_espa *matriz) {
    linhas *linha_atual = matriz->inicio;
    while (linha_atual != NULL) {
        elemento *elemento_atual = linha_atual->inicio;
        while (elemento_atual != NULL) {
            elemento *prox_elemento = elemento_atual->next;
            free(elemento_atual);
            elemento_atual = prox_elemento;
        }
        linhas *prox_linha = linha_atual->next;
        free(linha_atual);
        linha_atual = prox_linha;
    }
    free(matriz);
}

matriz_espa* criaMatrizTransposta(matriz_espa* mat) {
    matriz_espa *C = criaMatriz(mat->n_colunas, mat->n_linhas);

    linhas* linha_atual = mat->inicio;
    while (linha_atual != NULL) {
        elemento* elem_atual = linha_atual->inicio;
        while (elem_atual != NULL) {
            inserirElemento(C, elem_atual->coluna, linha_atual->linha, elem_atual->item);
            elem_atual = elem_atual->next;
        }
        linha_atual = linha_atual->next;
    }

    return C;  // retorna a matriz transposta
}

matriz_espa* somaMatrizes(matriz_espa *A, matriz_espa *B, matriz_espa *C) {
    if ((A->n_linhas != B->n_linhas) || (A->n_colunas != B->n_colunas))
        return NULL;

    linhas *linha1 = A->inicio;
    while (linha1 != NULL) {
        elemento *elem1 = linha1->inicio;
        while (elem1 != NULL) {
            inserirElemento(C, linha1->linha, elem1->coluna, elem1->item);
            elem1 = elem1->next;
        }
        linha1 = linha1->next;
    }

    linhas *linha2 = B->inicio;
    while (linha2 != NULL) {
        elemento *elem2 = linha2->inicio;
        while (elem2 != NULL) {
            int valorExistente = buscaElemento(C, linha2->linha, elem2->coluna);
            inserirElemento(C, linha2->linha, elem2->coluna, valorExistente + elem2->item);
            elem2 = elem2->next;
        }
        linha2 = linha2->next;
    }

    return C;
}

matriz_espa* multiplicaMatrizes(matriz_espa *A, matriz_espa *B, matriz_espa *C) {
    if (A->n_colunas != B->n_linhas)
        return NULL;

    for (int i = 0; i < A->n_linhas; i++) {
        for (int j = 0; j < B->n_colunas; j++) {
            int soma = 0;
            for (int k = 0; k < A->n_colunas; k++) {
                int a_ij = buscaElemento(A, i, k);
                int b_kj = buscaElemento(B, k, j);
                soma += a_ij * b_kj;
            }
            if (soma != 0)
                inserirElemento(C, i, j, soma);
        }
    }

    return C;
}