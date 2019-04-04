#include "gtushell.h"

int getpwd();

int main(int argc, const char* argv[]) {
    
    if(strcmp(argv[0],"pwd") == 0)
        return getpwd();
    
    //Output redirect
    else if(strcmp(argv[1],"INPUT_DIRECT") == 0){
        FILE *fp;
        fp = freopen(argv[0], "r", stdin);
        getpwd();
        fclose(fp);
        return 0;
    }
    
    //Output redirect
    else if(strcmp(argv[1],"OUTPUT_DIRECT") == 0){
        FILE *fp;
        fp = freopen(argv[0], "w", stdout);
        getpwd();
        fclose(fp);
        return 0;
    }
    
    else
        return -1;
}

int getpwd(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    return 0;
}
