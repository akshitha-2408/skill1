#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE (100 * 1024 * 1024)

int main(void)
{
    char *memory;
    pid_t pid;

    memory = malloc(SIZE);

    if (memory == NULL)
    {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < SIZE; i += 4096)
    {
        memory[i] = 1;
    }

    printf("Parent PID: %d\n", getpid());
    printf("100 MB memory allocated and initialized.\n");

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(memory);
        return 1;
    }

    if (pid == 0)
    {
        printf("Child PID: %d\n", getpid());
        printf("Child modifying memory...\n");

        for (size_t i = 0; i < SIZE; i += 4096)
        {
            memory[i] = 2;
        }

        printf("Child finished modifying memory.\n");
        sleep(60);

        free(memory);
        return 0;
    }
    else
    {
        printf("Parent waiting for child...\n");
        sleep(60);

        wait(NULL);

        printf("Child finished. Parent exiting.\n");

        free(memory);
    }

    return 0;
}
