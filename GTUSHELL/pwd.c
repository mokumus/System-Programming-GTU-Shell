#include "gtushell.h"

int getpwd();

int main(int argc, const char* argv[]) {
    
    if(strcmp(argv[0],"pwd") == 0)
        return getpwd();
    
    //File redirect
    else{
        freopen(argv[0], "w", stdout);
        return getpwd();
    }
}

int getpwd(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    return 0;
}
