Código do Trabalho final de Sistemas Operacionais -- Feito por Renan Felipe Rodrigues e Nicolas Lourenço Mafei

Trabalho: Criar um código modular e recursivo no qual pode dinâmicamente se adaptar à quantidades variáveis de Threads e Arquivos e tem por objetivo ordenar os arquivos de forma crescente e colocar seus respectivos valores ordenados em um arquivo de saída resultante.

Como rodar no computador de sistema Linux:
 -- Com todos arquivos na pasta, utilizar o Makefile fornecido para compilar;

 -- Utilizar na linha de comando a operação "make";

 -- Rodar o executável do programa por meio da linha de comando fornecida de exemplo: "./mergesort num_threads arq1.dat arq2.dat arqN.dat -o saida.dat".

O programa é composto de diversos arquivos contendo suas respectivas funções nas quais:

    main.c: Contém o corpo do código, é a parte principal que instancia as Threads e faz as operações de contagem de tempo de execução total e cria o arquivo final;

    funcoes.h: Cabeçalho no qual possui a estrutura principal das Threads e o protótipo das funções utilizadas no código;

    sort_alg.c: Algoritmo de Quick Sort retirado e implementado a partir de (https://www.geeksforgeeks.org/quick-sort-in-c/);

    thread_func.c: Função que é utilizada pelas Threads, no qual tem a função de ordenar os arquivos que foram delegados e retornar um vetor com esses valores ordenados;
    
    unificar.c: Tem a função de unificar todos os vetores resultantes de cada Thread separada, de forma a criar um vetor único e retorná-lo para a main.