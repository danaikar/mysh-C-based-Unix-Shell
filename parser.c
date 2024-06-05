#include "headers.h"
#include "globals.h"


/* Some basic commands */
int tokenHandler(char *token[]) {

    int length = 0;
    while (token[length] != NULL) length++;

    if(strcmp(token[0],"exit") == 0) {          // exit case
        exit(0); 
    }
    else if (strcmp(token[0],"cd") == 0) {      // cd case
        if(token[1] == NULL) {     // if only 'cd' is given, return to home dir
            chdir(getenv("HOME")); 
        }
        else {
            if(chdir(token[1]) != 0)  perror("-error- chdir()");
        }    		    
        return 1;
    }

    return 0;
}

int parser(char *line[], int token) {

    // split the command line into tokens 
    char newArray[2*MAXTOKENS][MAXLINE];
    int k = 0;    
    
    char *args[MAXTOKENS];

    for (int i = 0; i < token; i++) {

        int length = strlen(line[i]);
        char buffer[MAXLINE];
        int buffer_i = 0;

        for (int j = 0; j < length; j++) {
            char c = line[i][j];

            if (c == '>' || c == '<' || c == '|' || c == '&') {
                if (j == 0) {
                    newArray[k][0] = c;
                    newArray[k++][1] = '\0';
                } else {
                    if (buffer_i > 0) {
                        buffer[buffer_i] = '\0';
                        strcpy(newArray[k++], buffer);
                        buffer_i = 0;
                    }
                    newArray[k][0] = c;
                    newArray[k++][1] = '\0';
                }
            } else if (isspace(c)) {
                if (buffer_i > 0) {
                    buffer[buffer_i] = '\0';
                    strcpy(newArray[k++], buffer);
                    buffer_i = 0;
                }
            } else {
                buffer[buffer_i++] = c;
            }
        }

        if (buffer_i > 0) {
            buffer[buffer_i] = '\0';
            strcpy(newArray[k++], buffer);
        }
    }


    int stop = k;  
    for (int i = 0; i < k; i++) {
        
        // check for special characters and call the appropriate function 

        if (strcmp(newArray[i], "|") == 0) {     // case : pipe (|) 
            stop = k;
            pipe_handler(newArray, k);
            return 0;
        }
        else if (strcmp(newArray[i], "<") == 0) {    // case : input (<)
            stop = i;
            input_redirection(args, newArray, token, &i);
        }
        else if (strcmp(newArray[i], ">") == 0) {    // case : output (>)
            stop = i;
            output_redirection(args, newArray, token, &i);
        }
        else if (strcmp(newArray[i], "&") == 0) {    // case : background process (&)
            bg(newArray, k);
            return 0;
        }
    }        
    
    // copy the strings from newArray into args
    for (int j = 0; j < stop; j++) {
        args[j] = newArray[j];
    }
    args[stop] = NULL;

    execvp(args[0], args);
    printf("Error executing command\n");
    exit(1);

    return 0;
}
