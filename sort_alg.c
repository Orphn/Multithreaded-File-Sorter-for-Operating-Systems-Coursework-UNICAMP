#include <stdio.h>
#include "funcoes.h"

// IMPLEMENTADO DE: https://www.geeksforgeeks.org/quick-sort-in-c/

// Funcao troca a posicao de dois valores que estao desordenados
void troca_pos(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int vetor[], int men, int mai){

    // Inicializa o pivo como sendo o primeiro elemento
    int p = vetor[men];
    int i = men;
    int j = mai;

    while (i < j) {
        // Encontra o primeiro elemento maior que o pivo (a partir do inicio)
        while (vetor[i] <= p && i <= mai - 1){
            i++;
        }

        // Encontra o primeiro elemento menor que o pivo (a partir do final)
        while (vetor[j] > p && j >= men + 1){
            j--;
        }
        if (i < j) {
            troca_pos(&vetor[i], &vetor[j]);
        }
    }
    troca_pos(&vetor[men], &vetor[j]);
    return j;
}

void quickSort(int vetor[], int men, int mai){
    if (men < mai) {

        // Chama a funcao partition para encontrar o indice de particao
        int pi = partition(vetor, men, mai);

        // Chama recursivamente quickSort() para a metade esquerda e direita com base no indice de Particao
        quickSort(vetor, men, pi - 1);
        quickSort(vetor, pi + 1, mai);
    }
}