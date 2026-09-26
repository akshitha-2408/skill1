#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig)
{
    if (sig == SIGINT)
        printf("\nSIGINT received. Cleaning up...\n");
    else if (sig == SIGTERM)
        printf("\nSIGTERM received. Terminating...\n");

    exit(0);
}

int main()
{
    struct sigaction sa;

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Process PID: %d\n", getpid());
    printf("Waiting for signals...\n");

    while (1)
    {
        pause();
    }

    return 0;
}
