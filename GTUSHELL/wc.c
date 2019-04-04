#include "gtushell.h"
int wc(char* filename);


int main(int argc , char *argv[]) {
    //Simple run
    if (strcmp(argv[0],"wc") == 0)
        wc(argv[1]);
    
    
    //Input redirect
    else if(strcmp(argv[1],"INPUT_DIRECT") == 0){
        printf("HERE\n");
        FILE *fp;
        char path[MAX_PATH];
        fp = freopen(argv[0], "r", stdin);
        scanf("%s",path);
        wc(path);
        fclose(fp);
    }
    
    //Output redirect
    else if(strcmp(argv[2],"OUTPUT_DIRECT") == 0){
        FILE *fp;
        fp = freopen(argv[1], "w", stdout);
        wc(argv[0]);
        fclose(fp);
    }
    else
        printf("wc: %s: No such file or directory\n",argv[1] != NULL ?  " " : argv[1]);
        
    return 0;
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
