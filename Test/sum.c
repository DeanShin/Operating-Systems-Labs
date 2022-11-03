#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int a = 123;
    int b = 432;

    int pipeEnds[2];
    pipe(pipeEnds);

    int pid = fork();
    // Child process
    if (pid == 0)
    {
        // Close read side
        close(pipeEnds[0]);
        int sum = a + b;
        write(pipeEnds[1], &sum, sizeof(sum));
        exit(0);
    }
    else if (pid > 0)
    {
        // Close write side
        close(pipeEnds[1]);
        int sum;
        read(pipeEnds[0], &sum, sizeof(sum));
        printf("Sum: %d\n", sum);
        wait(NULL);
    }
    else
    {
        printf("Error: could not create child process.\n");
    }
}