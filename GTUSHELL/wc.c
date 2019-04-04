#include "gtushell.h"

int wc(const char* filename);
int findArg(int argc,const char **argv, char* key);

int main(int argc , const char *argv[]) {
 /*
    for (int i = 0; i < argc; i++)
        printf("argv%d: %s\n",i,argv[i]);
    
    printf("farg: %d\n",findArg(argc,argv,"PIPE_IN"));
 */
    
    //Simple run
    if (strcmp(argv[0],"wc") == 0)
        wc(argv[1]);
    
    //Input redirect
    else if(strcmp(argv[1],"INPUT_DIRECT") == 0){
        
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
    
   
    else if(findArg(argc,argv,"PIPE_IN") == 0){
        printf("wc pipe in\n");
    }
    
    else
        printf("wc: %s: No such file or directory\n",argv[1] != NULL ?  " " : argv[1]);
        
    return 0;
}

int wc(const char* filename){
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
