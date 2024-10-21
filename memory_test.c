#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <time.h>

#define BILLION 1000000000L

// Shared memory
volatile atomic_int shared_mem = 0;
//int shared_mem = 0;

// Thread argument structure
typedef struct {
    int id;
    long long read_count;
    int duration;
    int write_rate;
    int is_writer;
} thread_arg_t;

// Writer thread function
void* writer_thread(void* arg) {
    thread_arg_t* args = (thread_arg_t*)arg;
    int value = 0;
    while (1) {
        atomic_store(&shared_mem, value++); // Corrected atomic store
        //shared_mem = value++;
        usleep(1000000 / args->write_rate); // Sleep for the given write rate
    }
    return NULL;
}

// Reader thread function
void* reader_thread(void* arg) {
    thread_arg_t* args = (thread_arg_t*)arg;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (1) {
        // Exit after the duration
        clock_gettime(CLOCK_MONOTONIC, &end);
        if ((end.tv_sec - start.tv_sec) >= args->duration) break;

        // Read from shared memory
        int val = atomic_load(&shared_mem); // Corrected atomic load
        //int val = shared_mem;
        args->read_count++;
    }
    return NULL;
}

// Function to create threads and run the test
void run_test(int n_threads, int write_rate, int duration, int with_writer) {
    pthread_t* threads = malloc(n_threads * sizeof(pthread_t));
    thread_arg_t* thread_args = malloc(n_threads * sizeof(thread_arg_t));

    // Start the threads
    for (int i = 0; i < n_threads; i++) {
        thread_args[i].id = i;
        thread_args[i].read_count = 0;
        thread_args[i].duration = duration;
        thread_args[i].write_rate = write_rate;
        thread_args[i].is_writer = (i == 0 && with_writer);

        if (thread_args[i].is_writer) {
            pthread_create(&threads[i], NULL, writer_thread, &thread_args[i]);
        } else {
            pthread_create(&threads[i], NULL, reader_thread, &thread_args[i]);
        }
    }

    // Wait for the readers to finish
    sleep(duration);

    // Cancel the writer thread if it exists
    if (with_writer) {
        pthread_cancel(threads[0]);
    }

    // Join all threads
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Report results
    printf("Results (with_writer = %d):\n", with_writer);
    int total_reads = 0;
    for (int i = 1; i < n_threads; i++) {
        printf("Reader %d: %lu reads\n", i, thread_args[i].read_count);
        total_reads += thread_args[i].read_count;
    }
    printf("Average reads per second: %.2f\n", (double)total_reads / duration);

    // Cleanup
    free(threads);
    free(thread_args);
}

int main() {
    int n_threads, write_rate, duration;

    // Get user input
    printf("Enter the number of threads: ");
    scanf("%d", &n_threads);
    printf("Enter the write rate (writes per second) for the first thread: ");
    scanf("%d", &write_rate);
    printf("Enter the duration of the test (seconds): ");
    scanf("%d", &duration);

    // Run the test with writing contention
    run_test(n_threads, write_rate, duration, 1);

    // Run the test without writing contention
    run_test(n_threads, write_rate, duration, 0);

    return 0;
}
