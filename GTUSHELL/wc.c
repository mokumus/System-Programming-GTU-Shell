#include "gtushell.h"
int wc(char* filename);


int main(int argc , char *argv[]) {
    
    if (argc == 2 && strcmp(argv[0],"wc") == 0)
        return wc(argv[1]);
    
    else{
        freopen(argv[1], "w", stdout);
        return wc(argv[0]);
    }
}

int wc(char* filename){
    int lineCount = 0;
    char ch;
    
    FILE *fp = fopen(filename,"r");
    
    if(fp == 0){
        printf("wc: %s: open: No such file or directory\n", filename);
        exit(1);
    }
    
    while((ch=fgetc(fp)) != EOF){
        if(ch == '\n')
            lineCount++;
    }
    
    fclose(fp);
    printf("%d\n",lineCount);
    
    return 0;
}
