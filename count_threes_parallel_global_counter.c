#include <stdio.h>
#include <time.h> // used to start random seed
#include <unistd.h> // getopt lib
#include <stdlib.h> // exit_failure lib
#include <pthread.h> // thread lib
#include <errno.h> // error number lib
#include <math.h> // ceil lib


#define handleErrorNumber(error_num, msg) \
do { errno = error_num; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handleError(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

int counter;
pthread_mutex_t lock;

struct thread_info {
    pthread_t thread_id;
    int thread_num;
    int vector_size;
    int initial;
    int final;
    int* vector;
};

static void * thread_start(void *arg) {
    struct thread_info *t_info = arg;
    int local_counter = 0;

    for (int i = t_info->initial; i < t_info->final; i++) {
        if(t_info->vector[i] == 3) local_counter++;
    }

    printf("Local counter: %d\n", local_counter);
    pthread_mutex_lock(&lock);
    counter += local_counter;
    printf("Global counter: %d\n", counter);
    pthread_mutex_unlock(&lock);
    return 0;
}

int main(int argc, char *argv[]) {
    int opt, vector_size, *vector;
    int s, thread_num, num_threads;
    void *res;
    struct thread_info *thread_info;
    pthread_attr_t attr;

    while ((opt = getopt(argc, argv, "n:t:h")) != -1) {
        switch(opt) {
            case 'n':
                vector_size = strtoul(optarg, NULL, 0);
                break;

            case 't':
                num_threads = strtoul(optarg, NULL, 0);
                break;

            case 'h':
                printf("== Three Counter INFO ==\nGet how many number 3's a n-sized vector contains.\n\n"
                       "Arguments:\n-n Vector size (ex: -n 1024)\n-t Number of threads (ex: -t 8)\n-h Help\n");
                exit(EXIT_SUCCESS);

            default:
                fprintf(stderr, "Usage: %s [-m vector size] arg...\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    vector = malloc(vector_size * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < vector_size; i++) {
        vector[i] = rand() % 100;
    }

    s = pthread_attr_init(&attr);
    if (s != 0) {
        handleErrorNumber(s, "pthread_attr_init");
    }

    s = pthread_mutex_init(&lock, NULL);
    if (s != 0) {
        handleErrorNumber(s, "pthread_mutex_init");
    }

    thread_info = calloc(num_threads, sizeof(struct thread_info));
    if(thread_info == NULL) {
        handleError("thread_info calloc");
    }



    for (thread_num = 0; thread_num < num_threads; thread_num++) {
        thread_info[thread_num].thread_num = thread_num + 1;
        thread_info[thread_num].initial = ceil(vector_size / num_threads) * thread_num;
        thread_info[thread_num].final = (thread_num == num_threads - 1) ? vector_size - 1 : (ceil(vector_size / num_threads) * (thread_num + 1) - 1);
        thread_info[thread_num].vector = vector;
        thread_info[thread_num].vector_size = vector_size;
        
        s = pthread_create(&thread_info[thread_num].thread_id, &attr, &thread_start, &thread_info[thread_num]);
        if(s != 0)
            handleErrorNumber(s, "pthread_create");
    }

    s = pthread_attr_destroy(&attr);
    if(s != 0)
        handleErrorNumber(s, "pthread_attr_destroy");

    for (thread_num = 0; thread_num < num_threads; thread_num++) {
        s = pthread_join(thread_info[thread_num].thread_id, &res);
        if(s != 0)
            handleErrorNumber(s, "pthread_join");
        free(res);
    }

    free(thread_info);
    free(vector);
    pthread_mutex_destroy(&lock);

    // printf("Number of threes: %d", counter);
    exit(EXIT_SUCCESS);
}
