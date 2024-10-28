#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "funcoes.h"

int* unificar_valores(int** resultados, int* tam_resultados, int num_threads, info_thread* arg_threads){
    int tam_total = 0;

    // Calcula a quantidade total de todos os valores lidos pelas Threads
    for (int i = 0; i < num_threads; i++){
        tam_total += tam_resultados[i];
    }

    /*
    Aloca espaço para o vetor unificado
    Cada valor lido pelas Threads foram totalizados em uma única variável
    O vetor é criado com base no total de valores lidos
    */ 
    int *vetor_unificado = (int*)malloc(tam_total * sizeof(int));
    if (vetor_unificado == NULL){
        puts("Erro ao alocar memória para vetor unificado");
        return NULL;
    }

    /*
    Essa função aplica o conceito da fusão de dados do mergesort
    A função vai comparar em cada vetor resultante de cada Thread os valores lidos e ordenados
    A comparação é feita com base em um índice que mantém armazenado a posição do valor atual a ser comparado de cada Thread individualmente
    Como os valores já estão ordenados, a comparação feita sempre será entre os menores valores ATUAIS dos vetores resultantes
    O laço se repete com base na mesma quantidade de números que foram lidos no TOTAL
    */
    for (int k = 0; k < tam_total; k++){
        /*
        Utilizando a biblioteca limits.h
        Para sempre achar o menor valor é utilizado a função INT_MAX que define temporariamente o valor mínimo como o limite de um "inteiro"
        Dessa forma, qualquer valor pode ser o "menor"
        */ 
        int menor_valor = INT_MAX;
        int indice_thread = -1; // Essa variável armazena o indice da Thread que teve seu número colocado no vetor_unificado
    
        for (int i = 0; i < num_threads; i++){

            /* 
            Esse "if" verifica se o valor lido da Thread se qualifica como o "menor valor" em comparação aos anteriores já lidos
            Além disso, ele faz outra verificação, na qual observa se ainda restam valores para serem fundidos da Thread ou se já acabaram
            */
            if (arg_threads[i].indice_procura < arg_threads[i].total_valores &&
                resultados[i][arg_threads[i].indice_procura] < menor_valor){
                
                menor_valor = resultados[i][arg_threads[i].indice_procura]; // Caso a comparação seja verdadeira, o menor valor passa a ser o que foi lido da Thread
                indice_thread = i; // O indice passa a ser o mesmo da Thread que teve seu valor selecionado
                }
            }

        vetor_unificado[k] = menor_valor; // O vetor unificado recebe o menor valor encontrado pela verificação dessa vez

        /* 
        A Thread que teve seu número escolhido por ser o menor recebe um incremento em seu indice de procura
        Na próxima iteração do laço o número lido dessa Thread será o PRÓXIMO na ordem do vetor ordenado
        */
        arg_threads[indice_thread].indice_procura++;
    }

    return vetor_unificado;
}