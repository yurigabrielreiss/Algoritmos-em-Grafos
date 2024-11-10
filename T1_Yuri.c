#include<stdio.h>
#include<stdlib.h>
#include"matriz_esparsa.h"

int main() {
    int linhaA = 5;
    int colunaA = 5;

    int linhaB = 5;
    int colunaB = 5;

    matriz_espa *A = criaMatriz(linhaA, colunaA);
    inserirElemento(A, 0, 0, 1);
    inserirElemento(A, 0, 1, 2);
    inserirElemento(A, 1, 2, 3);

    printf("Matriz A:\n");
    imprimirMatriz(A);

    matriz_espa *B = criaMatriz(linhaB, colunaB);
    inserirElemento(B, 0, 0, 4);
    inserirElemento(B, 1, 2, 5);
    inserirElemento(B, 2, 1, 6);

    printf("Matriz B:\n");
    imprimirMatriz(B);

    matriz_espa *transpostaA = criaMatrizTransposta(A);
    printf("Transposta de A\n");
    imprimirMatriz(transpostaA);

    matriz_espa *transpostaB = criaMatrizTransposta(B);
    printf("Transposta de B\n");
    imprimirMatriz(transpostaB);

    matriz_espa *soma = criaMatriz(linhaA, colunaA);
    if ((linhaA != linhaB) || (colunaA != colunaB))
        printf("\nSOMA: A ordem das matrizes A e B devem ser iguais!\n\n");
    else{
        somaMatrizes(A, B, soma);
        printf("Soma (A+B):\n");
        imprimirMatriz(soma);
    }
    
    matriz_espa *produto = criaMatriz(linhaA, colunaB);
    if (colunaA != linhaB)
        printf("\nPRODUTO: A quantidade de colunas de A deve ser igual a quantidade de linhas de B!\n\n");
    else{
        multiplicaMatrizes(A, B, produto);
        printf("Produto (A*B):\n");
        imprimirMatriz(produto);
    }

    apagaMatriz(A);
    apagaMatriz(B);
    apagaMatriz(produto);
    apagaMatriz(soma);

    return 0;
}
