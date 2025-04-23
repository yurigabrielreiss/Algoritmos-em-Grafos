#include <stdio.h>
#include <stdlib.h>

typedef struct grafo grafo;
typedef struct vertice vertice;
typedef struct aresta aresta;

typedef struct grafo
{
    vertice *inicio;
} grafo;

typedef struct vertice
{
    aresta *a;     // Ponteiro para a lista de adjacencias (arestas) do vertice
    vertice *next; // Ponteiro para o proximo vertice na lista
    int id;        // Valor do vertice
} vertice;

typedef struct aresta
{
    int id_destino; // Identificador do vertice de destino
    aresta *next;   // Ponteiro para a proxima aresta na lista de adjacencias
} aresta;

grafo *criar_grafo(int x)
{
    grafo *g = (grafo *)calloc(1, sizeof(grafo));
    g->inicio = (vertice *)calloc(1, sizeof(vertice)); // Aloca o vertice inicial
    g->inicio->id = x; // Define o identificador do primeiro vertice
    g->inicio->a = NULL;
    g->inicio->next = NULL;
    return g;
}

void inserir_vertice(int x, grafo *g)
{
    if (!busca_vertice(x, g))
    {                             // Verifica se o vertice ja nao existe no grafo
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

void inserir_aresta(int v1, int v2, grafo *g)
{
    if (!busca_aresta(v1, v2, g))
    { // Verifica se a aresta ja nao existe
        vertice *vertice1 = g->inicio;
        vertice *vertice2 = g->inicio;

        // Localiza os vertices v1 e v2 na lista de vertices
        while (vertice1 != NULL && vertice1->id != v1)
            vertice1 = vertice1->next;

        while (vertice2 != NULL && vertice2->id != v2)
            vertice2 = vertice2->next;

        if (vertice1 != NULL && vertice2 != NULL)
        { // Se ambos os vertices existem
            // Insere v2 na lista de adjacencia de v1
            inserir_na_lista(v2, &(vertice1->a));
            // Insere v1 na lista de adjacencia de v2
            inserir_na_lista(v1, &(vertice2->a));
            printf("Aresta (%d, %d) inserida com sucesso.\n", v1, v2);
        }
        else
        {
            printf("Um ou ambos os vertices nao existem no grafo.\n");
        }
    }
    else
    {
        printf("Aresta (%d, %d) ja existe.\n", v1, v2);
    }
}

void inserir_na_lista(int key, aresta **lista)
{
    aresta *novaAresta = (aresta *)malloc(sizeof(aresta)); // Aloca memoria para a nova aresta
    novaAresta->id_destino = key;                          // Define o identificador do vertice de destino
    novaAresta->next = NULL;                               // Inicializa o proximo como NULL

    // Caso a lista esteja vazia ou o novo elemento seja o menor
    if (*lista == NULL)
    {
        novaAresta->next = *lista; // Insere no início da lista
        *lista = novaAresta;
    }
    else
    {
        aresta *aux = *lista; // Ponteiro auxiliar para percorrer a lista
        while (aux->next != NULL) // Encontra a posicao correta
            aux = aux->next;

        novaAresta->next = aux->next; // Insere o novo elemento na posicao correta
        aux->next = novaAresta;
    }
}

void mostra_grafo(grafo *g)
{
    vertice *auxV = g->inicio;
    while (auxV != NULL)
    {                              // Percorre a lista de vertices
        printf("%d ->", auxV->id); // Imprime o identificador do vertice
        aresta *auxA = auxV->a;

        while (auxA != NULL)
        {                                       // Percorre a lista de adjacencias do vertice
            printf(" %d ->", auxA->id_destino); // Imprime o vertice de destino
            auxA = auxA->next;
        }

        printf(" NULL\n"); // Fim da lista de adjacencias do vertice
        auxV = auxV->next;
    }
}

int busca_vertice(int id, grafo *g)
{
    vertice *auxV = g->inicio;
    while (auxV != NULL)
    {                       // Percorre a lista de vertices
        if (id == auxV->id) // Verifica se o ID do vertice corresponde
            return 1;       // Retorna 1 se o vertice existe
        auxV = auxV->next;
    }
    return 0;
}

int busca_aresta(int v1, int v2, grafo *g)
{
    if (busca_vertice(v1, g) && busca_vertice(v2, g))
    { // Verifica se ambos os vertices existem
        vertice *auxV = g->inicio;

        while (auxV->id != v1) // Encontra o vertice de origem
            auxV = auxV->next;

        aresta *auxA = auxV->a;
        while (auxA != NULL)
        {                               // Percorre a lista de adjacencias
            if (auxA->id_destino == v2) // Verifica se ha uma aresta para v2
                return 1;               // Retorna 1 se a aresta existe
            auxA = auxA->next;
        }
    }

    return 0; // Aresta não foi encontrada
}

void remover_vertice(int id, grafo *g)
{
    if (busca_vertice(id, g))
    {
        vertice *vAtual = g->inicio;
        vertice *vAnterior = NULL;

        // Localizar o vErtice a ser removido
        while (vAtual != NULL && vAtual->id != id)
        {
            vAnterior = vAtual;
            vAtual = vAtual->next;
        }

        if (vAtual == NULL)
        {
            printf("Vertice nao encontrado.\n");
            return;
        }

        // Remover todas as arestas associadas ao vErtice
        aresta *aAtual = vAtual->a;
        while (aAtual != NULL)
        {
            aresta *aRemover = aAtual;
            aAtual = aAtual->next;

            // Remover a aresta correspondente no outro vertice
            remover_aresta(id, aRemover->id_destino, g);
        }

        // Atualizar os ponteiros para remover o vertice
        if (vAnterior == NULL) // Caso seja o primeiro vertice
            g->inicio = vAtual->next;
        else
            vAnterior->next = vAtual->next;

        free(vAtual); // Liberar a memoria do vertice
        printf("Vertice %d removido.\n", id);
    }
    else
        printf("Vertice %d não existe no grafo.\n", id);
}

void remover_aresta(int v1, int v2, grafo *g)
{
    // Verifica se a aresta entre v1 e v2 existe no grafo
    if (busca_aresta(v1, v2, g))
    {
        vertice *vertice1 = g->inicio; // Ponteiro para localizar o vertice de origem (v1)
        vertice *vertice2 = g->inicio; // Ponteiro para localizar o vertice de destino (v2)

        // Percorre a lista de vertices ate encontrar v1
        while (vertice1 != NULL && vertice1->id != v1)
            vertice1 = vertice1->next;

        // Percorre a lista de vertices ate encontrar v2
        while (vertice2 != NULL && vertice2->id != v2)
            vertice2 = vertice2->next;

        // Verifica se ambos os vertices foram encontrados
        if (vertice1 != NULL && vertice2 != NULL)
        {
            // Remove v2 da lista de adjacencias do vertice v1
            if (remover_na_lista(v2, &(vertice1->a)))
            {
                // Remove v1 da lista de adjacencias do vertice v2
                if (remover_na_lista(v1, &(vertice2->a)))
                    printf("Aresta (%d, %d) removida com sucesso.\n", v1, v2);
            }
        }
        else
            printf("Um ou ambos os vertices nao existem no grafo.\n");
    }
    else
        printf("Aresta (%d, %d) nao existe.\n", v1, v2);
}

int remover_na_lista(int key, aresta **lista)
{
    // Verifica se a lista de adjacencias esta vazia
    if (*lista == NULL)
        return 0; // Nao ha nada a remover

    aresta *aux = *lista, *anterior = NULL; // Ponteiros auxiliares para percorrer a lista

    // Caso o elemento a ser removido esteja no inicio da lista
    if (aux->id_destino == key)
    {
        *lista = aux->next; // Atualiza o inicio da lista para o proximo elemento
        free(aux);          // Libera a memoria
        return 1;
    }

    // Percorre a lista ate encontrar o elemento ou o final da lista
    while (aux != NULL && aux->id_destino != key)
    {
        anterior = aux;  // Atualiza o ponteiro para o elemento anterior
        aux = aux->next; // Avanca para o proximo elemento
    }

    // Se encontrou o elemento na lista
    if (aux != NULL)
    {
        anterior->next = aux->next; // Remove o elemento ajustando os ponteiros
        free(aux);                  // Libera a memoria do elemento removido
        return 1;
    }

    // Se o elemento nao foi encontrado, retorna 0
    return 0;
}

void liberar_grafo(grafo *g)
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
}