#include<stdio.h>
#include<stdlib.h>
#include"T3_Yuri_TAD.h"

int main(){
    int opcao = 0;
    grafo *g = criar_grafo(1);


    inserir_vertice(1, g);
    //inserir_vertice(5, g);
    inserir_vertice(2, g);
    inserir_vertice(4, g);
    inserir_vertice(3, g);

    inserir_aresta(1,2,10,g);
    //inserir_aresta(1,5,20,g);
    //inserir_aresta(1,4,30,g);
    //inserir_aresta(1,3,50,g);
    //inserir_aresta(2,1,80,g);
    inserir_aresta(3,2,90,g);
    inserir_aresta(4,3,40,g);
    //inserir_aresta(4,5,20,g);
    //inserir_aresta(5,1,10,g);


    while(1){
        printf("_________________________________\n");
        printf("1. Inserir Vertice\n");
        printf("2. Inserir Aresta\n");
        printf("3. Remover Vertice\n");
        printf("4. Remover Aresta\n");
        printf("5. Visualizar Grafo\n");
        printf("6. Informar grau de um vertice\n");
        printf("7. Informar se o grafo e conexo\n");
        printf("8. Converter grafo para Matriz de Adjacencia\n");
        printf("9. Sair\n");
        printf("_________________________________\n");
        printf("Informe qual a funcao desejada: ");
        scanf("%d", &opcao);

        int n, id, origem, destino, peso;

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
                    printf("Vertice origem, Vertice destino e peso da aresta 'x y z': ");
                    scanf("%d %d %d", &origem, &destino, &peso);
                    inserir_aresta(origem, destino, peso, g);
                }
                break;
            
            case 3:
                printf("\nVertice a ser removido: ");
                scanf("%d", &id);
                remover_vertice(id, g);
                break;
            
            case 4: 
                printf("\nVertice origem e destino 'x y': ");
                scanf("%d %d", &origem, &destino);
                remover_aresta(origem, destino, g);
                break;
            
            case 5: 
                mostra_grafo(g);
                break;
            
            case 6:
                printf("\nDeseja verificar o grau de qual vertice: ");
                scanf("%d", &id);
                printf("Grau de saida do vertice '%d': %d\n", id, grau_saida(id, g));
                printf("Grau entrada do vertice '%d': %d\n", id, grau_entrada(id, g));
                break;
                
            case 7:
                if (grafo_conexo_bfs(g))
                    printf("\nO grafo e conexo!\n");
                else
                    printf("\nO grafo NAO e conexo!\n");
                break;
            
            case 8:
                printf("\n");
                imprimir_matriz(conta_vertice(g), converter_grafo(conta_vertice(g), g));
                break;

            case 9:
                return 0;

            default:
                printf("Opcao Invalida!\n");
                break;
        }
    }
}