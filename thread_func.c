#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"

void *thread_func(void *arg){
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio); // Cronometra o tempo da Thread

    // Essa linha converte a entrada generica da Thread "*void" para um ponteiro de estrutura "info_thread"
    info_thread *info = (info_thread *)arg;

    int *valores = NULL;

    // Le o nome dos arquivos que vao ser processados pela Thread
    for (int j = 0; j < info->qnt_arq; j++){
        char *nome_arq = info->arq[j];
        int num;

        FILE *arq;
        arq = fopen(nome_arq, "r");
        if (arq == NULL){
            puts("Erro ao abrir o arquivo");
            continue;
        }

        /*
        A funcao seguinte faz a leitura dos dados do arquivo selecionado no momento
        Cada entrada de inteiro vai ser lida do arquivo
        Os valores vao ser armazenados em um vetor dinamico
        A contagem total dos valores nesse Thread vai ser incrementado a cada valor lido
        */
        while (fscanf(arq, "%d", &num) == 1){
            valores = realloc(valores, (info->total_valores + 1) * sizeof(int));
            if (valores == NULL){
                puts("Erro no realloc");
                fclose(arq);
                pthread_exit(NULL);
            }
            
            valores[info->total_valores] = num;
            info->total_valores++;
        }

        fclose(arq);

    }
    quickSort(valores, 0, info->total_valores - 1); // Algoritmo de Sort utilizado para cada Thread

    clock_gettime(CLOCK_MONOTONIC, &fim);
    info->tempo_total = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9; // Calcula o tempo total de operacao da Thread e salva o valor na estrutura

    pthread_exit(valores);
}