#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

int n;
int *fibonacci;
void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid;       /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }
    if (atoi(argv[1]) < 0)
    {
        fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
        return -1;
    }
    /* get the default attributes */
    pthread_attr_init(&attr);
    /* create the thread */
    // runner is the function that will be run, argv[1] is the parameter for the runner function
    pthread_create(&tid, &attr, runner, argv[1]);
    /* now wait for the thread to exit */
    pthread_join(tid, NULL);

    for (int i = 0; i < n; i++)
    {
        printf("%dth fibonacci number: %d\n", i + 1, fibonacci[i]);
    }

    free(fibonacci);
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param)
{
    n = std::max(0, atoi((char *)param));
    fibonacci = (int *)malloc(sizeof(int) * n);
    if (n > 0)
    {
        fibonacci[0] = 1;
    }
    if (n > 1)
    {
        fibonacci[1] = 1;
    }

    for (int i = 2; i < n; i++)
    {
        fibonacci[i] = fibonacci[i - 2] + fibonacci[i - 1];
    }

    pthread_exit(0);
}