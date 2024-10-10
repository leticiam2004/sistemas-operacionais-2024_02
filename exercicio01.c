#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUMCONS 1 // define o número de consumidores
#define NUMPROD 2 // define o número de produtores
#define BUFFERSIZE 1000 // define o tamanho do buffer

pthread_t cons[NUMCONS]; // threads para os consumidores
pthread_t prod[NUMPROD]; // threads para os produtores
pthread_mutex_t buffer_mutex; // mutex para sincronização do acesso ao buffer

int buffer[BUFFERSIZE]; // buffer compartilhado entre produtores e consumidores
int currentidx = 0; // índice atual do buffer
sem_t buffer_full, buffer_empty; // semáforos para controlar o estado do buffer (cheio e vazio)

void *produtor(void *arg)
{
    int n;
    while (1)
    {
        n = rand() % 1000; // gera um valor aleatório para produção
        sem_wait(&buffer_full); // espera até que haja espaço disponível no buffer
        pthread_mutex_lock(&buffer_mutex); // adquire o mutex para garantir acesso exclusivo ao buffer
        buffer[currentidx++] = n; // insere o valor produzido no buffer
        pthread_mutex_unlock(&buffer_mutex); // libera o mutex
        sem_post(&buffer_empty); // sinaliza que há itens disponíveis no buffer
        printf("Produzindo número %d\n", n);
        sleep((1 + rand() % 5)); // pausa a execução por um intervalo aleatório
    }
}

void *consumidor(void *arg)
{
    int n;
    while (1)
    {
        sem_wait(&buffer_empty);             // espera até que haja itens disponíveis no buffer
        pthread_mutex_lock(&buffer_mutex);   // adquire o mutex para garantir acesso exclusivo ao buffer
        n = buffer[--currentidx];            // remove um item do buffer
        pthread_mutex_unlock(&buffer_mutex); // libera o mutex
        sem_post(&buffer_full);              // sinaliza que há espaço disponível no buffer
        printf("Consumindo número %d\n", n);
        sleep(1 + (rand() % 5));             // pausa a execução por um intervalo aleatório
    }
}

int main(int argc, char **argv)
{
    int i;
    pthread_mutex_init(&buffer_mutex, NULL); // inicializa o mutex
    sem_init(&buffer_full, 0, BUFFERSIZE); // inicializa o semáforo que controla o buffer cheio com o valor inicial igual ao tamanho do buffer
    sem_init(&buffer_empty, 0, 0); // inicializa o semáforo que controla o buffer vazio com valor inicial zero

    // cria as threads de consumidores
    for (i = 0; i < NUMCONS; i++)
        pthread_create(&(cons[i]), NULL, consumidor, NULL);

    // cria as threads de produtores
    for (i = 0; i < NUMPROD; i++)
        pthread_create(&(prod[i]), NULL, produtor, NULL);

    // aguarda todas as threads de consumidores finalizarem
    for (i = 0; i < NUMCONS; i++)
        pthread_join(cons[i], NULL);

    // aguarda todas as threads de produtores finalizarem
    for (i = 0; i < NUMPROD; i++)
        pthread_join(prod[i], NULL);

    return 0;
}
