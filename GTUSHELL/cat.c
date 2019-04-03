#include "gtushell.h"
//SOURCE USED : https://stackoverflow.com/questions/3866468/using-system-calls-to-implement-the-unix-cat-command


static int cat(const char *filename)
{
    int fd;
    char buffer[4096];
    ssize_t bytesRead;
    
    if ((fd = open(filename, O_RDONLY)) == -1)
        return -1;

    while ((bytesRead = read(fd, buffer, sizeof buffer)) > 0) {
        ssize_t bytesTotal = 0;
        while (bytesTotal < bytesRead) {
            ssize_t bytesWritten = write(STDOUT_FILENO, buffer + bytesTotal, bytesRead - bytesTotal);
            if (bytesWritten < 1)
                return -1;
            bytesTotal += bytesWritten;
        }
    }
    
    if (close(fd) != 0)
        return -1;
    
    return bytesRead == 0 ? 0 : -1;;
}

int main(int argc, char **argv)
{
    int i;
    
    if (argc == 2) {
        for (i = 1; i < argc; i++) {
            if (cat(argv[i]) != 0)
                return -1;
        }
    }

    return 0;
}
