//
//  gtushell.h
//  myshell
//
//  Created by Muhammed Okumuş on 31.03.2019.
//  Copyright © 2019 Muhammed Okumuş. All rights reserved.
//

#ifndef gtushell_h
#define gtushell_h

#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <fcntl.h> //Used for cat.c open() and definitions.

#define MAX_PATH 512 //max number letters in a path name
#define MAX_CMD 256 // max number of letters for a command
#define MAX_CMD_LIST 100 // max number of commands to be supported

#define SIGHUP  1   /* Hangup the process */
#define SIGINT  2   /* Interrupt the process */
#define SIGQUIT 3   /* Quit the process */
#define SIGILL  4   /* Illegal instruction. */
#define SIGTRAP 5   /* Trace trap. */
#define SIGABRT 6   /* Abort. */

int findArg(int argc,const char **argv, char* key){
    int i;
    for (i = 0; i < argc; i++) {
        if(strcmp(argv[i], key) == 0)
            return 0;
    }
    return -1;
}



#endif /* gtushell_h */
