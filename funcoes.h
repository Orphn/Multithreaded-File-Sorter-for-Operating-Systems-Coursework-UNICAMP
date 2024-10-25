#include <pthread.h>
#include <time.h>

// Estrutura que contém os dados das Threads
typedef struct{
    int qnt_arq; // Quantidade de arquivos que vão ser processados pela Thread
    char **arq; // Nome dos arquivos
    int total_valores; // Quantidade total de valores que foram processados
    double tempo_total; // Tempo de operação da Thread
} info_thread;

void *thread_func(void *arg); // Função das Threads

// Funções do algoritmo de ordenação
// RETIRADO DE: https://www.geeksforgeeks.org/quick-sort-in-c/
void troca_pos(int* a, int* b);
void quickSort(int arr[], int low, int high);
int partition(int arr[], int low, int high);

int* unificar_valores(int** resultados, int* tam_resultados, int num_threads, int* tam_unificado); // Função para unificar os vetores obtidos das Threads