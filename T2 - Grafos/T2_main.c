#include <stdio.h>
#include <stdlib.h>
#include "T2_Grafos.h"

int main(){
    int opcao = 0;
    grafo *g = criar_grafo(1);

    while(1){
        printf("_________________________________\n");
        printf("1. Inserir Vertice\n");
        printf("2. Inserir Aresta\n");
        printf("3. Visualizar grafo\n");
        printf("4. Remover Vertice\n");
        printf("5. Remover Aresta\n");
        printf("6. Liberar Grafo\n");
        printf("7. Sair\n");
        printf("_________________________________\n");
        printf("Informe qual a funcao desejada: ");
        scanf("%d", &opcao);

        int n;
        int id, origem, destino;

        switch(opcao){
            case 1:
                printf("\nQuantos Vertices deseja inserir? ");
                scanf("%d", &n);

                for(int i = 1; i <= n; i++){
                    printf("Id do vertice %d: ", i);
                    scanf("%d", &id);

                    inserir_vertice(id, g);
                }
                break;
            
            case 2: 
                printf("\nQuantas arestas deseja inserir? ");
                scanf("%d", &n);
                for(int i = 1; i <= n; i++){
                    printf("Vertice origem e destino 'x y': ");
                    scanf("%d %d", &origem, &destino);
                    inserir_aresta(origem, destino, g);
                }
                break;
            
            case 3:
                mostra_grafo(g);
                break;
            
            case 4: 
                printf("\nVertice a ser removido: ");
                scanf("%d", &id);
                remover_vertice(id, g);
                break;
            
            case 5: 
                printf("\nVertice origem e destino 'x y': ");
                scanf("%d %d", &origem, &destino);
                remover_aresta(origem, destino, g);
                break;
            
            case 6: 
                liberar_grafo(g);
                g = criar_grafo(1);
                break;

            case 7: 
                return 0;
                break;

            default:
                printf("Opcao Invalida!\n");
                break;
        }
    }
}