#include "gtushell.h"

static unsigned int total = 0;
static unsigned int special = 0;
static unsigned int zflag = 0; //0 is off, 1 is on

int sum(const char *fpath, const struct stat *sb, int typeflag);
int sizepathfun (char *path);
int postOrderApply (char *path, int pathfun (char *path1));

int main(int argc, char **argv) {
    int result = -1;
    
    if(argc == 2){
        result = postOrderApply(argv[1], sizepathfun);
        sizepathfun(argv[1]);
        return result;
    }
    
    else if(argc == 3){
        zflag = 1;
        result = postOrderApply(argv[2], sizepathfun);
        sizepathfun(argv[2]);
        return result;
    }
    
    printf("Argument count: %d\n", argc);
    printf("Usage \n--------- \n");
    printf("%s path/to/folder\n", argv[0]);
    printf("%s -z path/to/folder\n", argv[0]);
    
    return result;
}

int sum(const char *fpath, const struct stat *sb, int typeflag) {
    total += sb->st_size;
    return 0;
}

int sizepathfun (char *path) {
    int temp;
    if (ftw(path, &sum, 1)) {
        perror("ftw");
        return -1;
    }
    temp = total;
    total = 0;
    printf("%-10u %s\n", temp, path);
    return temp;
}

int postOrderApply (char *path, int pathfun (char *path1)) {
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(path)))
        return -1;
    
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR || entry->d_type == DT_REG) {
            char filepath[1024];
            if (strncmp(entry->d_name, ".",1) == 0)
                continue;
            
            strcpy(filepath, path);
            strcat(filepath, "/");
            strcat(filepath, entry->d_name);
            
            pathfun(filepath);
            postOrderApply(filepath, pathfun);
        }
        else
            printf("Special file beware%d\n",special++);
    }
    closedir(dir);
    return -1;
}
