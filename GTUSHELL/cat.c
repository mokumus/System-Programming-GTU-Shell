#include "gtushell.h"

static int cat(const char *filename);

int main(int argc, char **argv)
{
    int i;
    //Simple run
    if (strcmp(argv[0],"cat") == 0) {
        for (i = 1; i < argc; i++) {
            if (cat(argv[i]) != 0)
                return -1;
        }
    }
    
    //Input redirect
    else if(strcmp(argv[1],"INPUT_DIRECT") == 0){
        FILE *fp;
        char path[MAX_PATH];
        fp = freopen(argv[0], "r", stdin);
        scanf("%s",path);
        cat(path);
        fclose(fp);
    }
    
    //Output redirect
    else if(strcmp(argv[2],"OUTPUT_DIRECT") == 0){
        FILE *fp;
        fp = freopen(argv[1], "w", stdout);
        cat(argv[0]);
        fclose(fp);
    }
    
    else
        printf("cat: %s: No such file or directory\n",argv[1] != NULL ?  " " : argv[1]);
    
        
    return 0;
}


static int cat(const char *filename) {
    int fd;
    char buffer[4096];
    ssize_t bytesRead;
    
    if ((fd = open(filename, O_RDONLY)) == -1){
        printf("cat: %s: No such file or directory\n", filename);
        exit(1);
    }
    
    
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
