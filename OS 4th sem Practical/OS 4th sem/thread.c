#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 500
#define NUM_THREADS 5
#define SUB_ARRAY_SIZE (ARRAY_SIZE / NUM_THREADS)

int array[ARRAY_SIZE];
int sum[NUM_THREADS];
pthread_t threads[NUM_THREADS];

int calculate_partial_sum(int start, int end) {
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += array[i];
    }
    return sum;
}

void *thread_sum(void *arg) {
    int thread_id = *(int *)arg;
    int start = thread_id * SUB_ARRAY_SIZE;
    int end = start + SUB_ARRAY_SIZE;
    int *local_sum = (int *)malloc(sizeof(int));
    *local_sum = calculate_partial_sum(start,end);
    pthread_exit(local_sum);
}
void *thread_totalsum(void *arg) {
    int thread_id = *(int *)arg;
    int start = thread_id * SUB_ARRAY_SIZE;
    int end = start + SUB_ARRAY_SIZE;
    int (*sum_array)[2] = (int (*)[2])calloc(1,sizeof(int[2]));
    (*sum_array)[0] = calculate_partial_sum(start,end);
    (*sum_array)[1] = calculate_partial_sum(0,ARRAY_SIZE);
    pthread_exit(sum_array);
}

int main() {
    int thread_args[NUM_THREADS];
    int total_sum = 0;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = rand() % 100;
    }

    for (int i = 0; i < NUM_THREADS-1; ++i) {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, thread_sum, (void *)&thread_args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    thread_args[NUM_THREADS-1] = NUM_THREADS-1;
    int (*total_result)[2];
    pthread_create(&threads[NUM_THREADS-1], NULL, thread_totalsum, (void *)&thread_args[NUM_THREADS-1]);
    pthread_join(threads[NUM_THREADS-1], (void **)&total_result);
    printf("The total sum from thread %d is %d\n",NUM_THREADS-1, (*total_result)[1]);
  
    for (int i = 0; i < NUM_THREADS-1; ++i) {
        int *thread_result;
        if (pthread_join(threads[i], (void **)&thread_result) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
        }
        sum[i] = *thread_result;
        total_sum += sum[i];
        free(thread_result); 
    }
    total_sum = total_sum +(*total_result)[0];

    printf("Total sum calculated by main thread: %d\n", total_sum);
     if ((*total_result)[1] == total_sum) {
        printf("The sums match.\n");
    } else {
        printf("The sums do not match.\n");
    }
    
    return 0;
}

