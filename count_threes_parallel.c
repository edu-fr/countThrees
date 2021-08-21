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

struct thread_info {
    pthread_t thread_id;
    int thread_num;
    int vector_size;
    int initial;
    int final;
    int** vector;
    int counter;
};



static void *threadStart(void *arg) {
    struct thread_info *t_info = arg;

    for (int i = t_info->initial; i < t_info->final; i++) {
        if(t_info->vector[i] == 3) t_info->counter++;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int opt, vector_size, *vector, counter = 0;
    int s, thread_num, num_threads;
    struct thread_info *thread_info;
    pthread_attr_t attr;


    while ((opt = getopt(argc, argv, "n:h:")) != -1) {
        switch(opt) {
            case 'n':
                vector_size = strtoul(optarg, NULL, 0);
                break;

            case 't':
                num_threads = strtoul(optarg, NULL, 0);
                break;

            case 'h':
                printf("Three Counter\nGet how many number 3's a n-sized vector contains.\n\n"
                       "Arguments:\n-n Vector size (ex: -n 1024)\n-h Help\n");
                exit(EXIT_SUCCESS);

            default:
                fprintf(stderr, "Arguments error. Execute -h to get help .\n",
                        argv[0]);
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

    thread_info = calloc(num_threads, sizeof(struct thread_info));
    if(thread_info == NULL) {
        handleError("thread_info calloc");
    }

    for (thread_num = 0; thread_num < num_threads; thread_num++) {
        thread_info[thread_num].thread_num = thread_num + 1;
        thread_info[thread_num].counter = 0;
        thread_info[thread_num].initial = ceil(vector_size / num_threads) * thread_num; // PAREI AQUI
        thread_info[thread_num].final = 0;
        thread_info[thread_num].vector = 0;
        thread_info[thread_num].vector_size = 0;

    }

    printf("Number of threes: %d", counter);
    return 0;
}
