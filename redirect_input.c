#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char buffer[100];

    fd = open("input.txt", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return 1;
    }

    close(fd);

    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        printf("Read from redirected input: %s", buffer);
    }

    return 0;
}
