#include "gtushell.h"

int lsf(const char *path){
    DIR *d;
    struct dirent * dir;
    struct stat fileStat;
    char filepath[MAX_PATH];
    strcpy(filepath, path);

    d = opendir(path);
    
    while((dir = readdir(d)) != NULL) {
        stat(dir->d_name,&fileStat);
        if(dir->d_type != DT_DIR){
            //Access rights
            printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
            //Size in bytes
            printf("%10ld", fileStat.st_size);
            //File name
            printf("%18s", dir->d_name);
            if(dir->d_type == DT_REG)
                printf("%5s\n", "R");
            else
                printf("%5s\n", "S");
        }
        //Prepare path for next file
        strcpy(filepath, path);
        strcat(filepath, "/");
        strcat(filepath, dir->d_name);
    }
    
    closedir(d);
    return 0;
}

int main(int argc, const char * argv[]) {   
    //Simple run
    if(argv[0] != NULL && strcmp(argv[0], "lsf") == 0){
        char cwd[MAX_PATH];
        getcwd(cwd, sizeof(cwd));
        return lsf(cwd);
    }
    //Output redirect
    else{
        freopen(argv[0], "w", stdout);
        char cwd[MAX_PATH];
        getcwd(cwd, sizeof(cwd));
        return lsf(cwd);
    }
}
