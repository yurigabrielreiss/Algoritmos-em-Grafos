#include <stdio.h>
#include <stdlib.h>

typedef struct elemento elemento;
typedef struct linhas linhas;
typedef struct matriz_espa matriz_espa;

struct elemento {
    int item; 
    int coluna; // coluna na qual o elemento esta
    elemento *next; // aponta para o prox elemento da linha
};

struct linhas {
    int linha; // indice da linha
    linhas *next; // aponta para a prox linha
    elemento *inicio; // primeiro elemento da linha
};

struct matriz_espa {
    int n_linhas; // numero de linhas da matriz
    int n_colunas; // numero de colunas da matriz
    linhas *inicio; // aponta para a primeira linha da matriz
};

// cria uma matrz esparsa com 'l' linhas e 'c' colunas
matriz_espa* criaMatriz(int l, int c) {
    matriz_espa *matriz = (matriz_espa*) malloc(sizeof(matriz_espa)); // cria e aloca uma nova matriz
    matriz->n_colunas = c;
    matriz->n_linhas = l;
    matriz->inicio = NULL; // cabeça da lista vazia inicialmente

    return matriz;
}

// insere um novo elemento 'item' na matriz
void inserirElemento(matriz_espa *matriz, int linha, int coluna, int item) {
    if (item == 0) // nao insere zeros em uma matriz esparsa
        return;

    linhas *linha_atual = matriz->inicio;
    linhas *linha_anterior = NULL;

    // procura a linha correta/prox
    while (linha_atual != NULL && linha_atual->linha < linha) {
        linha_anterior = linha_atual;
        linha_atual = linha_atual->next;
    }

    // caso a linha nao exista, cria uma nova
    if (linha_atual == NULL || linha_atual->linha != linha) {
        linhas *linha_nova = (linhas*) malloc(sizeof(linhas));
        linha_nova->linha = linha; // indice da linha
        linha_nova->inicio = NULL; // lista de elementos
        linha_nova->next = linha_atual; // adiciona a nova linha a lista de linhas

        if (linha_anterior == NULL) // se e a primeira lista
            matriz->inicio = linha_nova; // a linha criada vira o inicio da lista
        else
            linha_anterior->next = linha_nova; // conecta linha anterior a nova linha

        linha_atual = linha_nova; // itera para a prox linha
    }

    elemento *elemento_atual = linha_atual->inicio; // elemento no inicio da linha
    elemento *elemento_anterior = NULL;

    // procura a posicao do elemento na linha
    while (elemento_atual != NULL && elemento_atual->coluna < coluna) {
        elemento_anterior = elemento_atual;
        elemento_atual = elemento_atual->next;
    }

    // se o elemento ja existe naquela posicao, so atualiza o valor
    if (elemento_atual != NULL && elemento_atual->coluna == coluna)
        elemento_atual->item = item; // item_anterior = item_novo
    else {
        elemento *elemento_novo = (elemento*) malloc(sizeof(elemento));
        elemento_novo->item = item;
        elemento_novo->coluna = coluna;
        elemento_novo->next = elemento_atual; // adiciona o novo elemento a lista de elementos

        if (elemento_anterior == NULL) // se for o primeiro elemento da linha
            linha_atual->inicio = elemento_novo; // define o novo como inicio da linha
        else
            elemento_anterior->next = elemento_novo; // senao, conecta elemento anterior ao novo elemento
    }
}

// funcao que procura um item em uma matriz
int buscaElemento(matriz_espa *matriz, int linha, int coluna) {
    linhas *linha_atual = matriz->inicio;

    while (linha_atual != NULL && linha_atual->linha != linha)
        linha_atual = linha_atual->next;

    if (linha_atual == NULL)
        return 0; // linha nao existe

    elemento *elemento_atual = linha_atual->inicio;

    // procura o elemento em uma coluna
    while (elemento_atual != NULL && elemento_atual->coluna != coluna)
        elemento_atual = elemento_atual->next;

    if (elemento_atual != NULL) // se o elemento existe
        return elemento_atual->item; // entao retorna o item
    else
        return 0; // elemento nao existe
}

// funcao que imprime uma matriz esparsa completa
void imprimirMatriz(matriz_espa *matriz) {
    for (int i = 0; i < matriz->n_linhas; i++){ // percorre cada linha
        for (int j = 0; j < matriz->n_colunas; j++) // percorre cada coluna
            printf(" %d ", buscaElemento(matriz, i, j)); // mostra o elemento encontrado pela funcao buscaElemento
        printf("\n");
    }
}

// funcao que apaga uma matriz e libera a memoria alocada nesta
void apagaMatriz(matriz_espa *matriz) {
    linhas *linha_atual = matriz->inicio;

    while (linha_atual != NULL) { // percorre todas as linhas ate que linha_atual seja vazia
        elemento *elemento_atual = linha_atual->inicio; // primeiro elemento

        while (elemento_atual != NULL) { // percorre todos os elementos ate que o elemento_atual seja vazio
            elemento *prox_elemento = elemento_atual->next;
            free(elemento_atual);
            elemento_atual = prox_elemento; // itera o prox elemento
        }

        linhas *prox_linha = linha_atual->next;
        free(linha_atual);
        linha_atual = prox_linha;
    }
    free(matriz); // libera a estrutura da matriz
}



// funcao que cria a transposta de uma matriz esparsa  (linha e coluna invertida)
matriz_espa* criaMatrizTransposta(matriz_espa* mat) {
    matriz_espa *C = criaMatriz(mat->n_colunas, mat->n_linhas);

    linhas* linha_atual = mat->inicio; // percorre as linhas da matriz original
    while (linha_atual != NULL) {
        elemento* elemento_atual = linha_atual->inicio; // percorre os elementos da linha
        while (elemento_atual != NULL) {
            inserirElemento(C, elemento_atual->coluna, linha_atual->linha, elemento_atual->item); // insere um elemento n posicao (coluna, linha), invertendo
            elemento_atual = elemento_atual->next; // itera para o prox elemento
        }
        linha_atual = linha_atual->next; // itera para prox linha
    }

    return C;  // retorna a matriz transposta
}

// funcao que soma duas matrizes
matriz_espa* somaMatrizes(matriz_espa *A, matriz_espa *B, matriz_espa *C) {
    if ((A->n_linhas != B->n_linhas) || (A->n_colunas != B->n_colunas)) // para somar matrizes, A e B devem srr da mesma ordem
        return NULL;

    linhas *linha1 = A->inicio; // percorre as linhas da matriz A inicialmente
    while (linha1 != NULL) {
        elemento *elem1 = linha1->inicio; // percorre os elementos da linha
        while (elem1 != NULL) {
            inserirElemento(C, linha1->linha, elem1->coluna, elem1->item); // insere um elemento de A em C
            elem1 = elem1->next; // prox elemento
        }
        linha1 = linha1->next; // prox linha
    }

    linhas *linha2 = B->inicio; // percorre as linhas da matriz B
    while (linha2 != NULL) {
        elemento *elem2 = linha2->inicio; // percorre os elementos da linha
        while (elem2 != NULL) {
            int valorExistente = buscaElemento(C, linha2->linha, elem2->coluna); // verifica se o valor do elemento ja presente em C esta na posicao atual da matriz B
            inserirElemento(C, linha2->linha, elem2->coluna, valorExistente + elem2->item); // soma o valor do elemento de B ao valor existente em C e atualiza o valor em C
            elem2 = elem2->next; // prox elemento na linha
        }
        linha2 = linha2->next; // prox linha 
    }

    return C;
}

// funcao que multiplica duas matrizes
matriz_espa* multiplicaMatrizes(matriz_espa *A, matriz_espa *B, matriz_espa *C) {
    if (A->n_colunas != B->n_linhas) // na multiplicacao de matrizes, o numero de colunas de A deve ser igual ao numero de linhas de B
        return NULL;

    for (int i = 0; i < A->n_linhas; i++)// percorre as linhas de A
        for (int j = 0; j < B->n_colunas; j++) { // percorre as colunas de B
            int soma = 0;
            for (int k = 0; k < A->n_colunas; k++) {
                int a_ij = buscaElemento(A, i, k); // procura o elemento na posicao A[i][k]
                int b_kj = buscaElemento(B, k, j); // procura o elemento na posicao B[k][j]
                soma = soma + (a_ij * b_kj); // faz o somatorio dos produtos entre os elementos
            }
            if (soma != 0) // nao insere zeros em uma matriz esparsa
                inserirElemento(C, i, j, soma); // insere o valor do somatorio dos produtos em C
        }

    return C;
}
