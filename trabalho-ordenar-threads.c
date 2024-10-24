#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

pthread_barrier_t barrierThreads;

// função bubble sort
void bubbleSort(int vetor[], int n)
{
	int k, j, aux;
	for (k = 1; k < n; k++)
	{
		for (j = 0; j < n - 1; j++)
		{
			if (vetor[j] > vetor[j + 1])
			{
				aux = vetor[j];
				vetor[j] = vetor[j + 1];
				vetor[j + 1] = aux;
			}
		}
	}
}

// função insertion sort
void insertionSort(int vetor[], int n)
{
	for (int i = 1; i < n; i++)
	{
		int key = vetor[i];
		int j = i - 1;
		while (j >= 0 && vetor[j] > key)
		{
			vetor[j + 1] = vetor[j];
			j = j - 1;
		}
		vetor[j + 1] = key;
	}
}

// função selection sort
void selectionSort(int vetor[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		int minIdx = i;
		for (int j = i + 1; j < n; j++)
		{
			if (vetor[j] < vetor[minIdx])
			{
				minIdx = j;
			}
		}
		int temp = vetor[minIdx];
		vetor[minIdx] = vetor[i];
		vetor[i] = temp;
	}
}

// função quick sort
void quickSort(int vetor[], int low, int high)
{
	if (low < high) {
		int pivot = vetor[high];
		int i = low - 1;

		for (int j = low; j < high; j++) {
			if (vetor[j] < pivot) {
				i++;
				int temp = vetor[i];
				vetor[i] = vetor[j];
				vetor[j] = temp;
			}
		}

		int temp = vetor[i + 1];
		vetor[i + 1] = vetor[high];
		vetor[high] = temp;

		quickSort(vetor, low, i);
		quickSort(vetor, i + 2, high);

	}
}

void *controlBubbleSort(void *tid)
{	
    clock_t t;
	long *array;
	array = (long *)tid;
    pthread_barrier_wait(&barrierThreads);
	t = clock();
	bubbleSort((int *)array, 1000);
	t = clock() - t;
	printf("tempo para executar bubble sort: %lfs\n", ((double)t) / ((CLOCKS_PER_SEC)));
}

void *controlInsertionSort(void *tid)
{
    clock_t t;
	long *array;
	array = (long *)tid;
    pthread_barrier_wait(&barrierThreads);
	t = clock();
	insertionSort((int *)array, 1000);
	t = clock() - t;
	printf("tempo para executar insertion sort: %lfs\n", ((double)t) / ((CLOCKS_PER_SEC)));
}

void *controlSelectionSort(void *tid)
{
    clock_t t;
	long *array;
	array = (long *)tid;
    pthread_barrier_wait(&barrierThreads);
	t = clock();
	selectionSort((int *)array, 1000);
	t = clock() - t;
	printf("tempo para executar selection sort: %lfs\n", ((double)t) / ((CLOCKS_PER_SEC)));
}

void *controlQuickSort(void *tid)
{
    clock_t t;
    long *array;
    array = (long *)tid;
    pthread_barrier_wait(&barrierThreads);
    t = clock();
    quickSort((int *)array, 0, 999);  // Pass the correct low and high indices
    t = clock() - t;
    printf("tempo para executar quick sort: %lfs\n", ((double)t) / ((CLOCKS_PER_SEC)));
}

int main()
{
    int i;
    srand(time(NULL));
    int a[1000];
    int b[1000];
    int c[1000];
    int d[1000];
    for (i = 0; i < 1000; i++)
    {
        a[i] = rand() % 100;
        d[i] = b[i] = c[i] = a[i];
    }
    pthread_t t1, t2, t3, t4;    
    pthread_barrier_init(&barrierThreads,NULL, 4);                                
    pthread_create(&t1, NULL, controlBubbleSort, (void *)a);
    pthread_create(&t2, NULL, controlInsertionSort, (void *)b);
    pthread_create(&t3, NULL, controlSelectionSort, (void *)c);
    pthread_create(&t4, NULL, controlQuickSort, (void *)d);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_barrier_destroy(&barrierThreads);

    printf("finalizando a main\n");

	return 0;
}
