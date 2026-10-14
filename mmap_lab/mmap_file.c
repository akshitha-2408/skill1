#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main(void)
{
    int fd;
    struct stat st;
    char *data;

    fd = open("data.txt", O_RDWR);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &st) == -1)
    {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (st.st_size == 0)
    {
        printf("File is empty.\n");
        close(fd);
        return 0;
    }

    data = mmap(NULL, st.st_size,
                PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);

    if (data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Original file contents:\n");
    fwrite(data, 1, st.st_size, stdout);
    printf("\n\nModifying file...\n");

    if (st.st_size >= 5)
    {
        memcpy(data, "HELLO", 5);
    }

    if (msync(data, st.st_size, MS_SYNC) == -1)
    {
        perror("msync");
    }

    printf("File modified using mmap().\n");

    if (munmap(data, st.st_size) == -1)
    {
        perror("munmap");
    }

    close(fd);
    return 0;
}
