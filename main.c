#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h" // Cabecalho dos prototipos das funcoes utilizadas

int main(int argc, char *argv[]){
    // Contador de tempo de execucao (do inicio ate o fim da criacao do arquivo de saida)
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Teste para prevenir erros caso o usuario coloque menos argumentos que o minimo na linha de comando
    if(argc < 5){
        printf("Quantidade de argumentos incorreta\n");
        return 1;
    }

    int num_threads = 0;

    // Captura dos dados passados pela linha de comando, em ordem:
    num_threads = atoi(argv[1]); // Quantidade de Threads a serem utilizadas
    if(num_threads <= 0){
        puts("Quantidade de Threads invalida");
        return 1;
    }

    int qnt_arq = argc - 4; // Quantidade de arquivos que vao ser ordenados
    char *arq_saida = argv[argc-1]; // Le o nome do arquivo a ser criado
    int temp = num_threads; // Arquivo temporario para deteccao de erros caso a qnt de Threads > arquivos
    
    if (qnt_arq < num_threads){
        num_threads = qnt_arq;
    }

    //Criacao de vetores dinamicos para:

    //IDs das Threads que vao ser criadas
    pthread_t *TIDs = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    //Vetor de struct onde estara guardada as informacoes da Threads: (Quantidade de Arquivos para cada Thread e os nomes dos arquivos)
    info_thread *arg_threads = (info_thread*)malloc(num_threads * sizeof(info_thread));
    //Vetor que armazena os vetores resultantes que foram tratados das Threads
    int **resultados = (int**)malloc(num_threads * sizeof(int*));

    //Laco de repeticao para instanciar os vetores onde vao ser armazenados:
    for (int i = 0; i < num_threads; i++){
        arg_threads[i].qnt_arq = 0; // quantidade de arquivos
        arg_threads[i].arq = (char**)malloc(qnt_arq / num_threads * sizeof(char*)); // nomes dos arquivos
        arg_threads[i].total_valores = 0; // quantidade de valores na Thread
        arg_threads[i].tempo_total = 0; // tempo de execucao
        arg_threads[i].indice_procura = 0; // indice de procura do mergesort
    }

    /*
    Loop com funcoes de alocacao dos arquivos, no qual:
    Primeiramente, eh utilizado a variavel indice_thread para ciclicamente distribuir os arquivos para as Threads
    Em seguida, eh realocado o tamanho do vetor dos arquivos, sempre dinamicamente expandindo o tamanho para a quantidade de arquivos e seus nomes que cada Thread vai processar
    Apos isso, o nome do arquivo eh atribuido a esse espaco que foi criado na posicao correta do vetor
    Por fim, ao armazenar o nome, a variavel qnt_arq eh incrementada, indicando a Thread que agora tem um arquivo a mais atribuida a ela
    */
    for (int i = 0; i < qnt_arq; i++){
        int indice_thread = i % num_threads;
        arg_threads[indice_thread].arq = realloc(arg_threads[indice_thread].arq, (arg_threads[indice_thread].qnt_arq + 1) * sizeof(char*));
        arg_threads[indice_thread].arq[arg_threads[indice_thread].qnt_arq] = argv[i+2];
        arg_threads[indice_thread].qnt_arq++;
    }

    /*
    A funcao seguinte cria as Threads com base no valor que foi passado na linha de comando
    Cada Thread vai possuir um dos IDs que foram criados, sua funcao para execucao e os argumentos da funcao
    O argumento da funcao na Thread vai ser a estrutura criada anteriormente (convertida para um *void), que contem a quantidade de arquivos que vao ser lidos e seus nomes
    */
    for (int i = 0; i < num_threads; i++){
        pthread_create(&TIDs[i], NULL, thread_func, (void*)&arg_threads[i]);
    }

    /*
    A funcao seguinte espera todas as Threads terminarem
    Retornando seus vetores ordenados resultantes para a matriz resultados, no qual vai armazenar cada vetor ordenado em uma linha
    */
    for (int i = 0; i < num_threads; i++){
        pthread_join(TIDs[i], (void**)&resultados[i]);
    }
    
    // Calcula o tamanho total dos resultados -- registrando esse valor totalizado em uma variavel
    int tam_total = 0;
    for (int i = 0; i < num_threads; i++){
        tam_total += arg_threads[i].total_valores;
    }

    int *vetor_unificado = unificar_valores(resultados, tam_total, num_threads, arg_threads); // Funcao utilizada para unificar os valores e ordena-los

    // Criacao do arquivo de saida com o nome referenciado na linha de comando
    FILE *saida = fopen(arq_saida, "w");
    if (saida == NULL) {
        puts("Erro ao abrir o arquivo de saida");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < tam_total; i++) {
        fprintf(saida, "%d\n", vetor_unificado[i]);  // Cada valor em uma nova linha do arquivo.dat
    }
    
    /*
    Impressao dos tempos de execucao de cada Thread e do Tempo total
    A funcao verifica atraves da variavel temp quais Threads foram necessarias para a execucao
    Caso Threads > Arquivos, o sistema automaticamente descarta as Threads que nao foram necessarias
    A partir disso, as Threads que foram executadas recebem seus tempos de execucao
    Enquanto as desnecessarias ficam na condicao de "nao executou"
    */
    for (int i = 0; i < temp; i++){
        if(i < num_threads){
            printf("Tempo de execucao do Thread %d: %.9lf segundos.\n", i, arg_threads[i].tempo_total);
        }
        else{
            printf("A Thread %d nao executou.\n", i);
        }
    }

    // Liberacao de memoria para os vetores alocados dinamicamente
    for (int i = 0; i < num_threads; i++){
        free(arg_threads[i].arq); // Nomes dos arquivos
        free(resultados[i]); // Vetores com os resultados de cada Thread
    }

    // Vetores
    free(arg_threads);
    free(TIDs);
    free(resultados);
    free(vetor_unificado);

    // Fim da contagem de tempo
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_exec = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9; // Calculo do tempo de execucao total
    printf("Tempo total de execucao: %.9lf segundos\n", tempo_exec);

    return 0;
}
