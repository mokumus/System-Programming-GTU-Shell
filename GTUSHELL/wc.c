#include "gtushell.h"

int main(int argc , char *argv[]) {
    int lineCount = 0;
    char ch;
    
    FILE *fp = fopen(argv[1],"r");
    
    if(fp == 0){
        printf("wc: %s: open: No such file or directory\n",argv[1]);
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
