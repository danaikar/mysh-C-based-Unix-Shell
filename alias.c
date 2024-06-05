#include "headers.h"
#include "globals.h"


int alias(char *tokens[], int token) {

    // split the command line into tokens 
    char newArray[2*MAXTOKENS][1024];
    int k = 0;    
    int flag = 0;

    if(token == 3) {   // that means that the command have more that one words
        for (int i = 0; i < token; i++) {
            int length = strlen(tokens[i]);
            char buffer[1024];
            int buffer_i = 0;
            int in_quote = 0;

            for (int j = 0; j < length; j++) {
                char c = tokens[i][j];
                
                if (isspace(c) && !in_quote) {
                    if (buffer_i > 0) {
                        buffer[buffer_i] = '\0';
                        strcpy(newArray[k++], buffer);
                        buffer_i = 0;
                    }
                } else {
                    buffer[buffer_i++] = c;
                    if (c == '"') {
                        in_quote = !in_quote;
                    }
                }
            }

            if (buffer_i > 0) {
                buffer[buffer_i] = '\0';
                strcpy(newArray[k++], buffer);
            }
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            int length = strlen(tokens[i]);
            char buffer[1024];
            int buffer_i = 0;
            int in_quote = 0;

            for (int j = 0; j < length; j++) {
                char c = tokens[i][j];

                if (isspace(c) && !in_quote) {
                    if (buffer_i > 0) {
                        buffer[buffer_i] = '\0';
                        strcpy(newArray[k++], buffer);
                        buffer_i = 0;
                    }
                } else {
                    buffer[buffer_i++] = c;
                    if (c == '"') {
                        in_quote = !in_quote;
                    }
                    if(j == length-1 && i == 2) {
                        buffer[buffer_i++] = ' ';
                        j = -1;
                        length = length + strlen(tokens[++i]);
                        in_quote = 0;
                    }
                }
            }

            if (buffer_i > 0) {
                buffer[buffer_i] = '\0';
                strcpy(newArray[k++], buffer);
            }
        }  
    }


  

    // remove quotes
    for (int i = 0; i < k; i++) {
        int len = strlen(newArray[i]);
        if (newArray[i][0] == '"' && newArray[i][len - 1] == '"') {
            for (int j = 0; j < len - 2; j++) {
                newArray[i][j] = newArray[i][j + 1];
            }
            newArray[i][len - 2] = '\0';
        }
    }

    flag = 0;
    // check if alias already exists
    for (int i = 0; i < MAX_ALIAS; i++) {
        if (strcmp(aliasArray[i].shortcut, newArray[1]) == 0) {
            strcpy(aliasArray[i].command, newArray[2]);
            flag = 1;
        }
    }

    if(flag == 0) {  
        // add new alias to the array 
        int i;
        for (i = 0; i < MAX_ALIAS; i++) {
            if (aliasArray[i].shortcut[0] == '\0') {
                strcpy(aliasArray[i].shortcut, newArray[1]);
                strcpy(aliasArray[i].command, newArray[2]);
                break;
            }
        }
        if(i == MAX_ALIAS) {
            printf("Error! Remove some aliases\n");
            return -1;
        }
    }
    
    printf("List of aliases:\n");
    for (int i = 0; i < MAX_ALIAS; i++) { 
        if (aliasArray[i].shortcut[0] != '\0') {
            printf("%s = %s\n", aliasArray[i].shortcut, aliasArray[i].command);
        }
    }

    return 1;
}


void replaceAlias(char *(*tokens_ref)[MAXTOKENS], int token) {
    
    for(int i = 0; i < token; i++) {
        for (int j = 0; j < MAX_ALIAS; j++) {
            if (strcmp(aliasArray[j].shortcut, (*tokens_ref)[i]) == 0) {
                strcpy((*tokens_ref)[i], aliasArray[j].command);
            }
        }
    }

}


int destroyAlias(char *args[], int tokens) {
    for(int i = 0; i < tokens; i++) {
        for (int j = 0; j < MAX_ALIAS; j++) {
            if (strcmp(aliasArray[j].shortcut, args[i]) == 0) {

                printf("DESTROY ALIAS %s\n", args[i]);
                aliasArray[j].shortcut[0] = '\0';
                aliasArray[j].command[0] = '\0';

                printf("List of aliases:\n");
                for (int k = 0; k < MAX_ALIAS; k++) { 
                    if (aliasArray[k].shortcut[0] != '\0') {
                        printf("%s = %s\n", aliasArray[k].shortcut, aliasArray[k].command);
                    }
                }
                return 0;
            }
        }
    }
    printf("Error! Alias doesn't exist\n");
    return -1;
}
