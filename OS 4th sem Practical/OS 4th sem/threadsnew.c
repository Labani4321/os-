#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 500
#define NUM_THREADS 5
#define SUB_ARRAY_SIZE (ARRAY_SIZE / (NUM_THREADS - 1))

int array[ARRAY_SIZE];
int sum[NUM_THREADS - 1];
pthread_t threads[NUM_THREADS];

void *thread_sum(void *arg) {
    int thread_id = *(int *)arg;
    int start = thread_id * SUB_ARRAY_SIZE;
    int end = start + SUB_ARRAY_SIZE;
    int *local_sum = (int *)malloc(sizeof(int));
    *local_sum = 0;
    
    for (int i = start; i < end; ++i) {
        *local_sum += array[i];
    }
    
    pthread_exit(local_sum);
}

void *thread_total_sum(void *arg) {
    int *total_sum = (int *)malloc(sizeof(int));
    *total_sum = 0;
    
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        *total_sum += array[i];
    }
    
    pthread_exit(total_sum);
}

int main() {
    int thread_args[NUM_THREADS];
    int main_thread_total_sum = 0;
    
    // Seed the random number generator
    srand((unsigned int)time(NULL));
    
    // Initialize the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = rand() % 100;
    }
    
    // Create threads to compute partial sums
    for (int i = 0; i < NUM_THREADS - 1; ++i) {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, thread_sum, (void *)&thread_args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    
    // Create one thread to compute the total sum
    thread_args[NUM_THREADS - 1] = NUM_THREADS - 1;
    if (pthread_create(&threads[NUM_THREADS - 1], NULL, thread_total_sum, (void *)&thread_args[NUM_THREADS - 1]) != 0) {
        fprintf(stderr, "Error creating total sum thread\n");
        return 1;
    }
    
    // Join the partial sum threads and accumulate the results in the main thread
    for (int i = 0; i < NUM_THREADS - 1; ++i) {
        int *thread_result;
        if (pthread_join(threads[i], (void **)&thread_result) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
        }
        sum[i] = *thread_result;
        main_thread_total_sum += sum[i];
        free(thread_result);
    }
    
    // Join the total sum thread and get the result
    int *total_thread_result;
    if (pthread_join(threads[NUM_THREADS - 1], (void **)&total_thread_result) != 0) {
        fprintf(stderr, "Error joining total sum thread\n");
        return 1;
    }
    int total_sum_from_thread = *total_thread_result;
    free(total_thread_result);
    
    // Print the results
    printf("Total sum calculated by main thread: %d\n", main_thread_total_sum);
    printf("Total sum calculated by thread: %d\n", total_sum_from_thread);
    
    // Compare the results
    if (main_thread_total_sum == total_sum_from_thread) {
        printf("The sums match.\n");
    } else {
        printf("The sums do not match.\n");
    }
    
    return 0;
}
