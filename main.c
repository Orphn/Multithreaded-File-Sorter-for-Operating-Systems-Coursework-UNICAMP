#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h" // Cabeçalho dos protótipos das funções utilizadas

int main(int argc, char *argv[]){
    // Contador de tempo de execução (do inicio até o fim da criação do arquivo de saída)
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    int num_threads = 0;

    // Captura dos dados passados pela linha de comando, em ordem:
    num_threads = atoi(argv[1]); // Quantidade de Threads a serem utilizadas
    if(num_threads <= 0){
        puts("Quantidade de Threads inválida");
        return 1;
    }

    int qnt_arq = argc - 4; // Quantidade de arquivos que vão ser ordenados
    char *arq_saida = argv[argc-1]; // Lê o nome do arquivo a ser criado
    int temp = num_threads; // Arquivo temporário para detecção de erros caso a qnt de Threads > arquivos
    
    if (qnt_arq < num_threads){
        num_threads = qnt_arq;
    }

    //Criação de vetores dinâmicos para:
    //IDs das Threads que vão ser criadas:
    pthread_t *TIDs = (pthread_t*)malloc(qnt_arq * sizeof(pthread_t));
    //Vetor de struct onde estará guardada as informações da Threads: (Quantidade de Arquivos para cada Thread e os nomes dos arquivos)
    info_thread *arg_threads = (info_thread*)malloc(num_threads * sizeof(info_thread));
    //Vetor que armazena os valores da Threads
    int **resultados = (int**)malloc(num_threads * sizeof(int*));
    //Vetor que armazena o tamanho de cada vetor resultante das Threads, ou seja, a quantidade de números que foram lidos pela Thread
    int *tam_resultados = (int*)malloc(num_threads * sizeof(int));

    //Laço de repetição para instanciar os vetores onde vão ser armazenados:
    for (int i = 0; i < num_threads; i++){
        arg_threads[i].qnt_arq = 0; // quantidade de arquivos
        arg_threads[i].arq = (char**)malloc(qnt_arq / num_threads * sizeof(char*)); // nomes dos arquivos
        arg_threads[i].total_valores = 0; // quantidade de valores na Thread
        arg_threads[i].tempo_total = 0; // tempo de execução
        arg_threads[i].indice_procura = 0; // indice de procura do mergesort
    }

    /*
    Loop com funções de alocação dos arquivos, no qual:
    Primeiramente, é utilizado a variável indice_thread para ciclicamente distribuir os arquivos para as Threads
    Em seguida, é realocado o tamanho do vetor dos arquivos, sempre dinamicamente expandindo o tamanho para a quantidade de arquivos e seus nomes que cada Thread vai processar
    Após isso, o nome do arquivo é atribuido a esse espaço que foi criado na posição correta do vetor
    Por fim, ao armazenar o nome, a variável qnt_arq é incrementada, indicando à Thread que agora tem um arquivo a mais atribuida à ela
    */
    for (int i = 0; i < qnt_arq; i++){
        int indice_thread = i % num_threads;
        arg_threads[indice_thread].arq = realloc(arg_threads[indice_thread].arq, (arg_threads[indice_thread].qnt_arq + 1) * sizeof(char*));
        arg_threads[indice_thread].arq[arg_threads[indice_thread].qnt_arq] = argv[i+2];
        arg_threads[indice_thread].qnt_arq++;
    }

    /* Teste de distribuição de Threads e Arquivos -- Ignorar
    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d vai processar %d arquivos.\n", i, arg_threads[i].qnt_arq);
        for (int j = 0; j < arg_threads[i].qnt_arq; j++) {
            printf("Arquivo %s\n", arg_threads[i].arq[j]);
        }
    }
    */

    /*
    A função seguinte cria as Threads com base no valor que foi passado na linha de comando
    Cada Thread vai possuir um dos IDs que foram criados, sua função para execução e os argumentos da função
    O argumento da função na Thread vai ser a estrutura criada anteriormente (convertida para um *void), que contém a quantidade de arquivos que vão ser lidos e seus nomes
    */
    for (int i = 0; i < num_threads; i++){
        pthread_create(&TIDs[i], NULL, thread_func, (void*)&arg_threads[i]);
    }

    /*
    A função seguinte espera todas as Threads terminarem e retornarem seus vetores ordenados resultantes
    Ao mesmo tempo, o laço atualiza o tamanho total de valores que foram processados pelas Threads para cada index diferente
    */
    for (int i = 0; i < num_threads; i++){
        pthread_join(TIDs[i], (void**)&resultados[i]);
        tam_resultados[i] = arg_threads[i].total_valores;
    }
    
    // Calcula o tamanho total dos resultados -- registrando esse valor totalizado em uma variável
    int tam_total = 0;
    for (int i = 0; i < num_threads; i++){
        tam_total += tam_resultados[i];
    }

    int *vetor_unificado = unificar_valores(resultados, tam_resultados, num_threads, arg_threads); // Função utilizada para unificar os valores e ordená-los

    // Criação do arquivo de saída com o nome referenciado na linha de comando
    FILE *saida = fopen(arq_saida, "w");
    if (saida == NULL) {
        puts("Erro ao abrir o arquivo de saída");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < tam_total; i++) {
        fprintf(saida, "%d\n", vetor_unificado[i]);  // Cada valor em uma nova linha do arquivo.dat
    }
    
    /*
    Impressão dos tempos de execução de cada Thread e do Tempo total
    A função verifica através da variável temp quais Threads foram necessárias para a execução
    Caso Threads > Arquivos, o sistema automaticamente descarta as Threads que não foram necessárias
    A partir disso, as Threads que foram executadas recebem seus tempos de execução
    Enquanto as desnecessárias ficam na condição de "não executou"
    */
    for (int i = 0; i < temp; i++){
        if(i < num_threads){
            printf("Tempo de execução do Thread %d: %.9lf segundos.\n", i, arg_threads[i].tempo_total);
        }
        else{
            printf("A Thread %d não executou.\n", i);
        }
    }

    // Liberação de memória para os vetores alocados dinâmicamente
    for (int i = 0; i < num_threads; i++){
        free(arg_threads[i].arq); // Nomes dos arquivos
        free(resultados[i]); // Vetores com os resultados de cada Thread
    }

    // Vetores
    free(arg_threads);
    free(TIDs);
    free(resultados);
    free(tam_resultados);
    free(vetor_unificado);

    // Fim da contagem de tempo
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_exec = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9; // Cálculo do tempo de execução total
    printf("Tempo total de execução: %.9lf segundos\n", tempo_exec);

    return 0;
}
