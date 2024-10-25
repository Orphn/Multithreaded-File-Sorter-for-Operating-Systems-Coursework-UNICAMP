#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int* unificar_valores(int** resultados, int* tam_resultados, int num_threads, int* tam_unificado) {
    int tam_total = 0;

    // Calcula o tamanho total de todos os valores lidos pelas Threads
    for (int i = 0; i < num_threads; i++){
        tam_total += tam_resultados[i];
    }

    /*
    * Aloca espaço para o vetor unificado
    * Cada valor lido pelas Threads foram totalizados em uma única variável
    * O vetor é criado com o espaço do tamanho total
    */ 
    int *vetor_unificado = (int*)malloc(tam_total * sizeof(int));
    if (vetor_unificado == NULL){
        puts("Erro ao alocar memória para vetor unificado");
        return NULL;
    }

    /*
    * Copia os resultados para o vetor unificado
    * Os valores foram armazenados de cada Thread
    */ 
    int index = 0;
    for (int i = 0; i < num_threads; i++){
        for (int j = 0; j < tam_resultados[i]; j++){
            vetor_unificado[index++] = resultados[i][j];
        }
    }

    // Ordena o vetor unificado através da mesma função de Quick Sort
    quickSort(vetor_unificado, 0, tam_total - 1);
    *tam_unificado = tam_total;
    return vetor_unificado;
}