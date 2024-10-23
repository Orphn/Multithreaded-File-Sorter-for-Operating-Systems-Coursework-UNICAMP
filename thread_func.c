#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"

void *thread_func(void *arg){
    struct timespec inicio, fim;

    // Inicializa a Thread com os valores de suas respectivas estruturas
    info_thread *info = (info_thread *)arg;

    int *valores = NULL;
    double tempo_leitura_total = 0.0;
    double tempo_ordenacao = 0.0;

    // Lê o nome dos arquivos que vão ser processados pela Thread
    for (int j = 0; j < info->qnt_arq; j++){
        char *nome_arq = info->arq[j];
        int num;

        FILE *arq;
        arq = fopen(nome_arq, "r");
        if (arq == NULL) {
            perror("Erro ao abrir o arquivo");
            continue;
        }

        clock_gettime(CLOCK_MONOTONIC, &inicio); // Cronometra o tempo de leitura do arquivo

        /*
        * Leitura dos dados do arquivo selecionado no momento
        * Cada entrada de inteiro vai ser lida por linha do arquivo
        * Os valores vão ser armazenados em um vetor dinâmico
        * A contagem total dos valores nesse Thread vai ser incrementado a cada valor lido
        */
        while (fscanf(arq, "%d", &num) == 1){
            valores = realloc(valores, (info->total_valores + 1) * sizeof(int));
            if (valores == NULL) {
                perror("Erro no realloc");
                fclose(arq);
                pthread_exit(NULL);
            }
            valores[info->total_valores] = num;
            info->total_valores++;
        }
        clock_gettime(CLOCK_MONOTONIC, &fim);

        fclose(arq);

        double tempo_leitura = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
        tempo_leitura_total += tempo_leitura;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio); // Cronometra o tempo de ordenação dos arquivos unificados
    quickSort(valores, 0, info->total_valores - 1);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_ordenacao = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    info->tempo_total = tempo_leitura_total + tempo_ordenacao; // Soma dos tempos de ordenação e leitura

    pthread_exit(valores);
}