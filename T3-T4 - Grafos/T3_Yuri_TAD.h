#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TAM_MAX 100

typedef struct grafo grafo;
typedef struct vertice vertice;
typedef struct aresta aresta;

typedef struct grafo
{
    vertice *inicio; // Ponteiro para o primeiro vértice do grafo
} grafo;

typedef struct vertice
{
    aresta *a;     // Ponteiro para a lista de adjacencias (arestas) do vertice
    vertice *next; // Ponteiro para o proximo vertice na lista
    int id;        // Identificador do vertice
} vertice;

typedef struct aresta
{
    int id_destino; // Identificador do vertice de destino
    int peso;
    aresta *next;   // Ponteiro para a proxima aresta na lista de adjacencias
} aresta;

grafo *criar_grafo(int x)
{
    grafo *g = (grafo *)calloc(1, sizeof(grafo));  // Aloca memória para o grafo
    g->inicio = (vertice *)calloc(1, sizeof(vertice)); // Aloca o vertice inicial
    g->inicio->id = x; // Define o identificador do primeiro vertice
    g->inicio->a = NULL; // Inicializa lista de adjacências vazia
    g->inicio->next = NULL; // Inicializa próximo vértice como NULL
    return g;
}

void inserir_vertice(int x, grafo *g){
    if (!busca_vertice(x, g)){    // Verifica se o vertice ja nao existe no grafo
        vertice *aux = g->inicio; // Ponteiro auxiliar para percorrer a lista de vertices
        while (aux->next != NULL) // Percorre ate o ultimo vertice da lista
            aux = aux->next;

        // Aloca memoria para o novo vertice e inicializa seus campos
        aux->next = (vertice*)calloc(1, sizeof(vertice));
        // Todos os campos (a, next, id) já estão inicializados:
            // vertice->a = NULL;
            // vertice->next = NULL;
            // vertice->id = 0;
        aux->next->id = x; // Define o identificador do vertice
    }
    else
        printf("Vertice ja existente.\n");
}

void inserir_aresta(int v1, int v2, int peso, grafo *g)
{
    if (!busca_aresta(v1, v2, g)){ // Verifica se a aresta ja nao existe
        vertice *vertice1 = g->inicio;
        vertice *vertice2 = g->inicio;

        // Localiza os vertices v1 e v2 na lista de vertices
        while (vertice1 != NULL && vertice1->id != v1)
            vertice1 = vertice1->next;

        while (vertice2 != NULL && vertice2->id != v2)
            vertice2 = vertice2->next;

        if (vertice1 != NULL && vertice2 != NULL){ // Se ambos os vertices existem
            // Insere v2 na lista de adjacencia de v1
            inserir_na_lista(v2, peso, &(vertice1->a));
            // Não precisa inserir v1 na lista de adjacencia de v2 pois o grafo é orientado

            printf("Aresta (%d -> %d), peso = %d inserida com sucesso.\n", v1, v2, peso);
        }
        else
            printf("Um ou ambos vertices nao existem no grafo.\n");
    }
    else
        printf("Aresta (%d -> %d) ja existe.\n", v1, v2);
}

void inserir_na_lista(int key, int peso, aresta **lista){
    aresta *novaAresta = (aresta *)malloc(sizeof(aresta)); // Aloca memoria para a nova aresta
    novaAresta->id_destino = key;                          // Define o identificador do vertice de destino
    novaAresta->next = NULL;                               // Inicializa o proximo como NULL
    novaAresta->peso = peso;

    if (*lista == NULL){
        novaAresta->next = *lista; // Insere no início da lista
        *lista = novaAresta;
    }
    else{
        aresta *aux = *lista; // Ponteiro que percorrera a lista
        while (aux->next != NULL) // Procura a posicao correta
            aux = aux->next;

        novaAresta->next = aux->next; // Insere o novo elemento na posicao correta
        aux->next = novaAresta;
    }
}

void mostra_grafo(grafo *g){
    vertice *auxV = g->inicio;
    while (auxV != NULL){          // Percorre a lista de vertices
        printf("%d ->", auxV->id); // Imprime o identificador do vertice

        aresta *auxA = auxV->a;

        while (auxA != NULL){                   // Percorre a lista de adjacencias do vertice
            printf(" (%d |%d|) ->", auxA->id_destino, auxA->peso); // Imprime o vertice final e o peso da aresta formada
            auxA = auxA->next;
        }

        printf(" NULL\n"); // Fim da lista de adjacencias do vertice
        auxV = auxV->next;
    }
}

int busca_vertice(int id, grafo *g){
    vertice *auxV = g->inicio;
    while (auxV != NULL){   // Percorre a lista de vertices
        if (id == auxV->id) // Verifica se o ID do vertice corresponde
            return 1;       // Retorna 1 se o vertice existe
        auxV = auxV->next;
    }
    return 0;
}

int busca_aresta(int v1, int v2, grafo *g){
    if (busca_vertice(v1, g)){
        vertice *auxV = g->inicio;

        while (auxV->id != v1) // Procura o vertice de origem v1
            auxV = auxV->next;

        aresta *auxA = auxV->a;
        while (auxA != NULL){           // Percorre a lista de adjacencias
            if (auxA->id_destino == v2) // Verifica se ha uma aresta para v2
                return 1;                // A aresta existe
            auxA = auxA->next;
        }
    }

    return 0; // Aresta não foi encontrada
}

void remover_vertice(int id, grafo *g) {
    if (busca_vertice(id, g)) { // Verifica se o vértice existe
        vertice *vAtual = g->inicio;
        vertice *vAnterior = NULL;

        // Localizar o vértice a ser removido
        while (vAtual != NULL && vAtual->id != id) {
            vAnterior = vAtual;
            vAtual = vAtual->next;
        }

        if (vAtual == NULL) {
            printf("Vertice nao encontrado.\n");
            return;
        }

        // 1. Remove todas as arestas associadas ao vértice
        aresta *aAtual = vAtual->a;
        while (aAtual != NULL) {
            aresta *aRemover = aAtual;
            aAtual = aAtual->next;

            // Remover a aresta correspondente no outro vértice
            remover_aresta(id, aRemover->id_destino, g);
        }

        // 2. Remove as arestas que apontam para o vértice removido
        vertice *v = g->inicio;
        while (v != NULL) {
            aresta *aAnterior = NULL;
            aresta *a = v->a;

            while (a != NULL) {
                if (a->id_destino == id) { // Encontra a aresta que aponta para o vértice removido
                    if (aAnterior == NULL) { // Se for a primeira aresta
                        v->a = a->next;
                    } else {
                        aAnterior->next = a->next;
                    }
                    free(a); // Libera a memória da aresta removida
                    break; // Sai do loop após remover a aresta
                }
                aAnterior = a;
                a = a->next;
            }
            v = v->next;
        }

        // 3. Atualiza os ponteiros para remover o vértice da lista de vértices
        if (vAnterior == NULL) { // Caso seja o primeiro vértice
            g->inicio = vAtual->next;
        } else {
            vAnterior->next = vAtual->next;
        }

        free(vAtual); // Liberar a memória do vértice
        printf("Vertice %d removido.\n", id);
    } else {
        printf("Vertice %d nao existe no grafo.\n", id);
    }
}

void remover_aresta(int v1, int v2, grafo *g){
    // Verifica se a aresta entre v1 e v2 existe no grafo
    if (busca_aresta(v1, v2, g)){
        vertice *vertice1 = g->inicio; // Ponteiro para localizar o vertice de origem (v1)
        vertice *vertice2 = g->inicio; // Ponteiro para localizar o vertice de destino (v2)

        // Percorre a lista de vertices ate encontrar v1
        while (vertice1 != NULL && vertice1->id != v1)
            vertice1 = vertice1->next;

        // Verifica se ambos os vertices foram encontrados
        if (vertice1 != NULL && vertice2 != NULL){
            // Remove v2 da lista de adjacencias do vertice v1
            if (remover_na_lista(v2, &(vertice1->a)))
                printf("Aresta (%d, %d) removida com sucesso.\n", v1, v2);
        }
        else
            printf("Um ou ambos os vertices nao existem no grafo.\n");
    }
    else
        printf("Aresta (%d -> %d) nao existe.\n", v1, v2);
}

// Função auxiliar para remover uma aresta da lista de adjacências
int remover_na_lista(int key, aresta **lista){
    // Verifica se a lista de adjacencias esta vazia
    if (*lista == NULL)
        return 0; // Nao ha nada a remover

    aresta *aux = *lista, *anterior = NULL; // Ponteiros auxiliares para percorrer a lista

    // Caso o elemento a ser removido esteja no inicio da lista
    if (aux->id_destino == key){
        *lista = aux->next; // Atualiza o inicio da lista para o proximo elemento
        free(aux);          // Libera a memoria
        return 1;
    }

    // Percorre a lista ate encontrar o elemento ou o final da lista
    while (aux != NULL && aux->id_destino != key){
        anterior = aux;  // Atualiza o ponteiro para o elemento anterior
        aux = aux->next; // Avanca para o proximo elemento
    }

    // Se encontrou o elemento na lista
    if (aux != NULL){
        anterior->next = aux->next; // Remove o elemento ajustando os ponteiros
        free(aux);                  // Libera a memoria do elemento removido
        return 1;
    }

    // Se o elemento nao foi encontrado, retorna 0
    return 0;
}

/*void liberar_grafo(grafo *g)
{
    vertice *vAux = g->inicio; // Ponteiro auxiliar para percorrer os vertices
    while (vAux != NULL)
    {
        aresta *aAux = vAux->a; // Ponteiro auxiliar para percorrer as arestas do vertice atual
        while (aAux != NULL)
        {
            aresta *aRemover = aAux; // Armazena o ponteiro para a aresta atual
            aAux = aAux->next;       // Avança para a proxima aresta
            free(aRemover);          // Libera a memoria da aresta atual
        }
        vertice *vRemover = vAux; // Armazena o ponteiro para o vertice atual
        vAux = vAux->next;        // Avanca para o proximo vertice
        free(vRemover);           // Libera a memoria do vertice atual
    }
    free(g); // Libera a memoria do grafo
}*/

// Começo do T3/T4 _____________________________________________________________-

// Função para calcular o grau de um vértice (número de arestas que saem dele)
int grau_saida(int id, grafo *g){
    vertice *vAtual = g->inicio; // Começa do início do grafo
    while (vAtual->id != id) // Procura o vértice com o ID especificado
        vAtual = vAtual->next;

    if(vAtual == NULL){ // Se não encontrou o vértice
        printf("O vertice %d nao existe.\n", id);
        return 0;
    }

    int grau = 0; // Contador de grau
    aresta *aAtual = vAtual->a; // Pega lista de adjacência
    while (aAtual != NULL){ // Conta todas as arestas (elementos na lista de adjacencias de vAtual)
        grau++;
        aAtual = aAtual->next;
    }

    return grau; // Retorna o grau calculado
}

// Função para calcular o grau de entrada (número de arestas que chegam no vértice)
int grau_entrada(int id, grafo *g) {
    int grau = 0;
    vertice *vAtual = g->inicio;

    // Percorre todos os vértices para verificar se possuem uma aresta para "id"
    while (vAtual != NULL) {
        aresta *aAtual = vAtual->a;
        while (aAtual != NULL) {
            if (aAtual->id_destino == id) // Se a aresta aponta para o vértice procurado
                grau++;
            aAtual = aAtual->next;
        }
        vAtual = vAtual->next;
    }

    return grau;
}
// Função que conta o número total de vértices no grafo
int conta_vertice(grafo *g){
    int cont = 0; // Contador de vértices
    vertice *vAtual = g->inicio;
    while (vAtual != NULL) {  // Percorre toda a lista de vertices
        cont++;
        vAtual = vAtual->next;
    }
    return cont;
}

// Função que converte o grafo para uma matriz de adjacência
int **converter_grafo(int cont, grafo *g) {
    cont = conta_vertice(g); // Atualiza cont com número real de vértices

    // Cria matriz de adjacência
    int **matriz = (int**)calloc(cont, sizeof(int*));

    for (int i = 0; i < cont; i++)
        matriz[i] = (int*)calloc(cont, sizeof(int));

    // Criar um vetor auxiliar para mapear IDs dos vértices para índices na matriz
    int *mapa = (int*)malloc(sizeof(int) * cont);
    vertice *vAtual = g->inicio;

    // Preenche o mapa de IDs
    for (int i = 0; i < cont; i++) {
        mapa[i] = vAtual->id; // Guarda o ID na posição i
        vAtual = vAtual->next;
    }

    // Preencher a matriz com os pesos das arestas
    vAtual = g->inicio;
    for (int i = 0; i < cont; i++) { // Itera em i, como se fosse cada vertice
        aresta *aAux = vAtual->a;
        while (aAux != NULL) { // Para cada aresta
            // Encontrar a posição do ID de destino na matriz
            for (int j = 0; j < cont; j++) {
                if (mapa[j] == aAux->id_destino) { // Encontra posição do destino no mapa
                    matriz[i][j] = aAux->peso; // Salva o peso na matriz
                    break;
                }
            }
            aAux = aAux->next;
        }
        vAtual = vAtual->next;
    }

    free(mapa);  // Libera memória do mapeamento
    return matriz;
}

// Função que imprime a matriz de adjacência
void imprimir_matriz(int nVertices, int **matriz){
    if (matriz != NULL) {
        for (int i = 0; i < nVertices; i++) {
            for (int j = 0; j < nVertices; j++)
                printf("%d ", matriz[i][j]);
            printf("\n");
        }

        // Liberar memória
        for (int i = 0; i < nVertices; i++)
            free(matriz[i]);
        free(matriz);
    }
}

// Constantes para o algoritmo BFS baseado no algoritmo do livro do Cormen
#define BRANCO 0  // Não visitado
#define CINZA 1   // visitado pela primeira vez
#define PRETO 2   // todos seus vizinhos foram visitados

// Função que verifica se o grafo é conexo usando BFS (Busca em Largura)
int grafo_conexo_bfs(grafo *g) {
    int cor[TAM_MAX];  // Armazena as cores dos vértices
    int d[TAM_MAX];    // Distância do vértice inicial até cada vértice
    int pi[TAM_MAX];   // Pai de cada vértice na árvore de busca

    int fila[TAM_MAX]; // Fila para a BFS
    int inicio = 0, fim = 0;

    // Inicializa todos os vértices
    for (int i = 0; i < TAM_MAX; i++) {
        cor[i] = BRANCO; // Todos vértices não visitados
        d[i] = INT_MAX; // Distância infinita
        pi[i] = -1; // Sem pai
    }

    // Começa a BFS pelo primeiro vértice do grafo
    vertice *vInicio = g->inicio;
    fila[fim++] = vInicio->id; // Adiciona à fila, depois fim = 1
    cor[vInicio->id] = CINZA; // Marca como descoberto
    d[vInicio->id] = 0;  // Distância inicial é 0
    pi[vInicio->id] = -1; // Raiz da árvore

    printf("Caminho percorrido pela BFS: ");

    int primeiro = 1;
    while (inicio < fim) {
        int id_atual = fila[inicio++]; // Remove da fila
        if (primeiro) {
            printf("%d", id_atual);
            primeiro = 0;
        } else
            printf(" -> %d", id_atual);

        // Encontrar o vértice correspondente no grafo
        vertice *v = g->inicio;
        while (v != NULL && v->id != id_atual)
            v = v->next;

        // Percorre todos os vértices vizinhos
        aresta *a = v->a;
        while (a != NULL) {
            if (cor[a->id_destino] == BRANCO) { // Se ainda não foi visitado
                fila[fim++] = a->id_destino; // Adiciona à fila
                cor[a->id_destino] = CINZA;  // Marca como descoberto
                d[a->id_destino] = d[id_atual] + 1; // Atualiza distância
                pi[a->id_destino] = id_atual; // Define o pai
            }
            a = a->next;
        }
        cor[id_atual] = PRETO; // Marca como visitado
    }
    printf("\n");

    // Verifica se todos os vértices existentes no grafo estão pretos
    vertice *v = g->inicio;
    while (v != NULL) {
        if (cor[v->id] != PRETO)
            return 0; // Grafo não é conexo
        v = v->next;
    }
    return 1; // Grafo é conexo
}