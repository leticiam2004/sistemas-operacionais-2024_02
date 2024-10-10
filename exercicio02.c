#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

long mat[3][7]; // matriz 3x7 de números aleatórios
int somaTotal = 0; // variável global que armazenará a soma total
pthread_mutex_t mutexsum; // mutex para proteger o acesso à somaTotal

// função executada pelas threads para somar os elementos de uma linha da matriz
void *countSum(void *tid)
{
    long *linha; // ponteiro para a linha da matriz que a thread vai somar
    linha = (long *)tid;
    int somaLinha = 0; // variável local para armazenar a soma da linha
    int i;

    // loop para somar todos os elementos da linha
    for (i = 0; i < 7; i++)
    {
        somaLinha += linha[i];
    }

    // protege o acesso à variável global somaTotal usando mutex
    pthread_mutex_lock(&mutexsum);
    somaTotal += somaLinha; // adiciona a soma da linha à soma total
    pthread_mutex_unlock(&mutexsum);

    pthread_exit(NULL); // finaliza a thread
}

int main()
{
    pthread_t t1, t2, t3; // criando três threads
    int i, j;
    
    srand(time(NULL)); // inicializa o gerador de números aleatórios com a hora atual

    // preenche a matriz 3x7 com valores aleatórios de 0 a 9
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 7; j++)
        {
            mat[i][j] = rand() % 10;
        }
    }

    // cria a primeira thread para somar a primeira linha da matriz
    printf("Main: criando a thread %d\n", 1);
    pthread_create(&t1, NULL, countSum, (void *)mat[0]);

    // cria a segunda thread para somar a segunda linha da matriz
    printf("Main: criando a thread %d\n", 2);
    pthread_create(&t2, NULL, countSum, (void *)mat[1]);

    // cria a terceira thread para somar a terceira linha da matriz
    printf("Main: criando a thread %d\n", 3);
    pthread_create(&t3, NULL, countSum, (void *)mat[2]);

    // espera todas as threads finalizarem a execução
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // imprime a soma total de todas as linhas da matriz
    printf("Main: finalizando\n");
    printf("Soma total: %d\n", somaTotal);

    // destrói o mutex após o uso
    pthread_mutex_destroy(&mutexsum);

    return 0;
}
