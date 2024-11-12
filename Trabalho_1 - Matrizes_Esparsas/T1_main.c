#include<stdio.h>
#include<stdlib.h>
#include"matriz_esparsa.h"

int main() {
    int linhaA = 6;
    int colunaA = 6;

    int linhaB = 6;
    int colunaB = 6;

    matriz_espa *A = criaMatriz(linhaA, colunaA);
    inserirElemento(A, 0, 0, 2);
    inserirElemento(A, 1, 4, 45);
    inserirElemento(A, 1, 5, 13);
    inserirElemento(A, 3, 2, 12);
    inserirElemento(A, 2, 3, 89);
    inserirElemento(A, 5, 3, 3);

    printf("Matriz A:\n");
    imprimirMatriz(A);

    matriz_espa *B = criaMatriz(linhaB, colunaB);
    inserirElemento(B, 0, 2, 9);
    inserirElemento(B, 1, 4, 5);
    inserirElemento(B, 2, 5, 15);
    inserirElemento(B, 3, 1, 3);
    inserirElemento(B, 4, 3, 4);
    inserirElemento(B, 5, 0, 8);
    inserirElemento(B, 5, 2, 64);

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
    apagaMatriz(transpostaA);
    apagaMatriz(transpostaB);
    apagaMatriz(produto);
    apagaMatriz(soma);

    return 0;
}
