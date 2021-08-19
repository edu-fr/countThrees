#include <stdio.h>
#include <unistd.h> // getopt lib
#include <stdlib.h> // exit_failure lib
#include <time.h>

int main(int argc, char *argv[]) {
    int opt, vector_size, *vector, counter = 0;
    while ((opt = getopt(argc, argv, "n:h:")) != -1) {
        switch(opt) {
            case 'n':
                vector_size = strtoul(optarg, NULL, 0);
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

    for (int i = 0; i < vector_size; i++) {
        if(vector[i] == 3) counter++;
    }

    printf("Number of threes: %d", counter);
    return 0;
}
