#include "gtushell.h"

//Misc
void help();
void printInfo();

//Signal handling
char *whatSignal(int sig);
void handleSignal(int sig);
int isIncluded(char* str, char c);

//Command handling
int handleBuiltInCommand(char** command);
int handleSimpleCommand(char** command, char* uwd);
int handlePipedCommand(char** command1, char** command2, char* uwd);
int handleInputDirectCommand(char** command, char** filename, char* uwd);
int handleOutputDirectCommand(char** command, char** filename, char* uwd);
char* pathToCommand(char* cmd, char* uwd);
char* getNthPreviousCmd(char* str);
int isHistoryCmd(char* str);

//String process and parsing functions
int parseCompound(char* str, char** compoundCmd);
int parseFileDirect(char* str, char** strdirected);
void parseSpace(char* str, char** parsed);
int processString(char* str, char** cmd1, char** cmd2);
int takeInput(char* str);


static int historySize = 0;
static char history[MAX_CMD_LIST][MAX_CMD];

int main(int argc, const char* argv[]) {
    
    char uwd[MAX_PATH];
    getcwd(uwd, sizeof(uwd));
    
    char inputString[MAX_CMD];
    
    char* parsedArgs[MAX_CMD_LIST];
    char* parsedCompoundCmd[MAX_CMD_LIST];
    int cmdType = 0;
    
   
    
    signal(SIGINT, handleSignal);
    while (1) {
        printInfo();
       
        if (takeInput(inputString))
            continue;
        
        //Previous command !n
        if(isHistoryCmd(inputString)){
            printInfo();
            printf("%s\n", getNthPreviousCmd(inputString));
            strcpy(inputString, (getNthPreviousCmd(inputString)));
        }
        
        cmdType = processString(inputString, parsedArgs, parsedCompoundCmd);
        
        switch (cmdType) {
            case 0: //built in command
                break;
            case 1: //Simple command
                handleSimpleCommand(parsedArgs, uwd);
                break;
            case 2: //Piped command
                handlePipedCommand(parsedArgs, parsedCompoundCmd, uwd);
                break;
            case 3: //Output redirection
                handleOutputDirectCommand(parsedArgs, parsedCompoundCmd, uwd);
                break;
            case 4: //Input redirection
                handleInputDirectCommand(parsedArgs, parsedCompoundCmd, uwd);
                break;
            default:
                printf("Something went wrong. Command no: %d\n", cmdType);
        }
    }
    return 0;
}


void help() {
    puts(  "*********************************"
         "\n*       GTU SHELL CSE-344       *"
         "\n*        Muhammed Okumus        *"
         "\n*           151044017           *"
         "\n*       Supported Commands      *"
         "\n*         lfs   pwd   cd        *"
         "\n*         help  cat   wc        *"
         "\n*         exit                  *"
         "\n*********************************");
    
    return;
}
char* pathToCommand(char* cmd, char* uwd){
    static char uPath[MAX_PATH];
    strcpy(uPath, uwd);
    strcat(uPath, "/");
    strcat(uPath, cmd);
    return uPath;
}


int handleSimpleCommand(char** command, char* uwd){
    pid_t pid = fork();
    
    if (pid == -1) {
        printf("Fork failed for simple comand.\n");
        return -1;
    }
    else if (pid == 0) {
        if (execv(pathToCommand(command[0], uwd), command) < 0) {
            printf("Utility failed."
                   "\nCommand: %s\n", command[0]);
        }
        exit(1);
    }
    else {
        wait(NULL);
        return 0;
    }
}


int handlePipedCommand(char** command1, char** command2, char* uwd){
    int pipeIO[2];
    pid_t p1, p2;
    
    if (pipe(pipeIO) < 0) {
        printf("Pipe failed in piped command %s | %s\n", command1[0], command2[0]);
        return -1;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("Fork failed for first command in piped command %s | %s\n", command1[0], command2[0]);
        return -1;
    }
    
    //CHILD FOR FIRST COMMAND
    if (p1 == 0) {
        close(pipeIO[0]);               //close input
        dup2(pipeIO[1], STDOUT_FILENO); //redirect output to stdout
        close(pipeIO[1]);               // close output
        
        if (execv(pathToCommand(command1[0], uwd), command1) < 0) {
            printf("Error executing command: %s\n", command1[0]);
            exit(-1);
        }
    }
    else {
        // PARENT
        p2 = fork();
        
        if (p2 < 0) {
            printf("Fork failed for second command in piped command %s | %s\n", command1[0], command2[0]);
            return -1;
        }
        
        //CHILD FOR SECOND COMMAND
        if (p2 == 0) {
            close(pipeIO[1]);               //close output
            dup2(pipeIO[0], STDIN_FILENO);  //redirect input to stdin
            close(pipeIO[0]);               //close input
            if (execv(pathToCommand(command2[0], uwd), command2) < 0) {
                printf("Error executing command: %s\n", command2[0]);
                exit(1);
            }
        } else {
            //Wait for commands to complete
            wait(NULL);
            wait(NULL);
            return 0;
        }
    }
    return 0;
}

int handleOutputDirectCommand(char** command, char** filename, char* uwd){
    pid_t pid = fork();
    if (pid == -1) {
        printf("Fork failed for directed comand.\n");
        return -1;
    }
    /*
     *  Important note:
     *  There are 2 conditional execl calls below
     *  First one will be only for bunedu since it can take a flag argument
     *  Other one is for the commands without flags
     *  Commands with flags should orginize arguments with itself.
    */
    else if (pid == 0) {
        printf("command[0]: %s\n", command[0]);
        if(command[1] != NULL){
            if (execl(pathToCommand(command[0], uwd), command[1], filename[0],"OUTPUT_DIRECT", (char *)0) < 0) //(char *)0 is for terminating argument list with null
                printf("Utility failed.\nCommand: %s\n", command[0]);
        }
        else{
            if (execl(pathToCommand(command[0], uwd), filename[0],"OUTPUT_DIRECT", (char *)0) < 0)
                printf("Utility failed.\nCommand: %s\n", command[0]);
        }
        exit(1);
    }
    else {
        wait(NULL);
        return 0;
    }
}


int handleInputDirectCommand(char** command, char** filename, char* uwd){
    pid_t pid = fork();
    if (pid == -1) {
        printf("Fork failed for directed comand.\n");
        return -1;
    }
    /*
     *  Important note:
     *  There are 2 conditional execl calls below
     *  First one will be only for bunedu since it can take a flag argument
     *  Other one is for the commands without flags
     *  Commands with flags should orginize arguments with itself.
     */
    else if (pid == 0) {
        if(command[1] != NULL){
            if (execl(pathToCommand(command[0], uwd), command[1], filename[0], "INPUT_DIRECT",(char *)0) < 0) //(char *)0 is for terminating argument list with null
                printf("Utility failed.\nCommand: %s\n", command[0]);
        }
        else{
            if (execl(pathToCommand(command[0], uwd), filename[0], "INPUT_DIRECT",(char *)0) < 0)
                printf("Utility failed.\nCommand: %s\n", command[0]);
        }
        exit(1);
    }
    else {
        wait(NULL);
        return 0;
    }
}


// Function to execute built-in commands
int handleBuiltInCommand(char** command){
    int i, cmdNo = 0;
    char* buildInCommands[3];
    
    buildInCommands[0] = "exit";
    buildInCommands[1] = "cd";
    buildInCommands[2] = "help";
    
    for (i = 0; i < 3; i++) {
        if (strcmp(command[0], buildInCommands[i]) == 0) {
            cmdNo = i + 1;
            break;
        }
    }
    
    switch (cmdNo) {
        case 1:
            printf("Goodbye\n");
            exit(0);
        case 2:
            if(chdir(command[1]) == -1)
                printf("cd: %s: No such file or directory\n",command[1]);
            return 1;
        case 3:
            help();
            return 1;

        default:
            break;
    }
    return 0;
}

int isIncluded(char* str, char c){
    return strchr(str, c) == NULL ? 0 : 1 ;
}

//0, Simple command
//1, Piped commands
//2, Output directed command
//3, Input directed command
int parseCompound(char* str, char** compoundCmd) {
    int i, k;
    char* parseTypes[3];
    
    parseTypes[0] = "|";
    parseTypes[1] = ">";
    parseTypes[2] = "<";
    
    for(i = 0; i < 3; i++){
        if(isIncluded(str, *parseTypes[i])){
            for (k = 0; k < 2; k++) {
                compoundCmd[k] = strsep(&str, parseTypes[i]);
                if (compoundCmd[k] == NULL)
                    break;
            }
            return i + 1; //Compound command
        }
    }
    return 0; //Simple command
}

void parseSpace(char* str, char** parsed) {
    int i;
    
    for (i = 0; i < MAX_CMD_LIST; i++) {
        parsed[i] = strsep(&str, " ");
        
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

char* getNthPreviousCmd(char* str) {
    char exc;
    int n;
  
    sscanf(str,"%c%d",&exc,&n);
    if(exc != '!')
        return NULL;
    return history[historySize -  n];
}

//cmd1 is always command
//cmd2 is command or file name depending on compound type
int processString(char* str, char** cmd1, char** cmd2) {
    char* compoundCmd[2];
    int compoundType = 0;
    
    compoundType = parseCompound(str, compoundCmd);
    
    if (compoundType) { //Compound command
        parseSpace(compoundCmd[0], cmd1);
        parseSpace(compoundCmd[1], cmd2);
    }
    else//Simple command
        parseSpace(str, cmd1);
    
    if (handleBuiltInCommand(cmd1))
        return 0;
    else
        return 1 + compoundType;
}

// Function to print username
void printInfo() {
    char username[64];
    getlogin_r(username, 64);
    printf("%s$ ", username);
}

char* readline(char *str) {
    while(1) {
        assert(fgets(str, MAX_CMD, stdin) != NULL);
        if(str[strlen(str) - 1] == '\n'){ //Break loop on '\n'
            str[strlen(str) - 1] = '\0';  //Explicit string termination
            break;
        }
    }
    return str;
}

int isHistoryCmd(char* str){
    return strncmp(str,"!",1) == 0 ? 1 : 0;
}

// Function to take input
int takeInput(char* str) {
    str = readline(str);
    
    if(!isHistoryCmd(str)) //Add to history if it isn't !n command
        strcpy(history[historySize++],str);

    if (strlen(str) != 0)
        return 0;
    else
        return 1;
}

char *whatSignal(int sig) {
    switch (sig) {
        case SIGHUP: return "SIGHUP";
        case SIGINT: return "SIGINT";
        case SIGQUIT: return "SIGQUIT";
        case SIGKILL: return "SIGKILL";
        case SIGTRAP: return "SIGTRAP";
        case SIGABRT: return "SIGABRT";
        default: return "OTHER";
    }
}

void handleSignal(int sig) {
    printf("Caught signal %s\nGoodbye\n", whatSignal(sig));
    exit(sig);
}
