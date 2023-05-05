#include "headers.h"
#include "globals.h"

ProcessList processlist;
Alias aliasArray[MAX_ALIAS];

void printWelcome(){
        printf("\n\t------------------------------------------\n");
        printf("\t|              mySimple Shell            |\n");
        printf("\t------------------------------------------\n");
        printf("\n\n");
}


int main() {
    
    printWelcome();
    
    signal(SIGINT, SIG_IGN);   
    signal(SIGTSTP, SIG_IGN);
    
    char input[MAXLINE];       // buffer for input 
    char *tokens[MAXTOKENS];   // array of tokens 
    int tok = 0;

    char history[MAX_HISTORY][100]; // array to store history of commands
    int history_count = 0;          // counter for number of commands in history
    

    while(1) {
        

        printf(COLOR_RED BOLD "mysh> " COLOR_RESET);
        fflush(stdout);

		fgets(input, MAXLINE, stdin);      // read input 
        
        if(strcmp(input, "\n") == 0) continue;      // if nothing given as input, do nothing 

        input[strlen(input) - 1] = '\0';   // delete null terminator


        // copy the new command or shift the array if its full
        if (history_count < MAX_HISTORY) {
            strcpy(history[history_count], input);
            history_count++;
        } 
        else {
            for (int j = 0; j < MAX_HISTORY - 1; j++) {
                strcpy(history[j], history[j+1]);
            }
            strcpy(history[MAX_HISTORY-1], input);
        }

        // split the command into tokens
        char *token = strtok(input, " ");
        int i = 0;
        while(token != NULL) {
            tokens[i++] = token;
            token = strtok(NULL, " ");
        }
        tokens[i] = NULL;  

        
        /*--alias--*/
        if(strcmp(tokens[0],"createalias") == 0) {      // alias case
            if(i < 3) {
                printf("createalias: wrong number of arguments\n");
                continue;
            }
            else {
                alias(tokens, i); 
                continue;
            }
        }
        else if(strcmp(tokens[0],"destroyalias") == 0) {   // destroy alias case
            if(i != 2) {
                printf("destroyalias: wrong number of arguments\n");
                continue;
            }
            else 
                destroyAlias(tokens, i);
                continue;
        }

        replaceAlias(&tokens, i);
        /*--alias--*/

        if (wild_characters(i, tokens) != -1)
            continue;


        if(strcmp(tokens[0],"exit") == 0) {      // exit case
            break; 
        }

        if(strcmp(tokens[0],"myHistory") == 0) {    
            if(i == 2) {
                int t = atoi(tokens[1]);  // t specifies which command the user wants to run from the history
                if(t <= history_count && t != 0) {                    
                    char *token = strtok(history[t-1], " ");
                    int k = 0;
                    while(token != NULL) {
                        tokens[k++] = token;
                        token = strtok(NULL, " ");
                    }
                    tokens[k] = NULL;  

                    if(!tokenHandler(tokens)) {  // after creating an array that contains all the tokens, pass it to a function that handles them
                        pid_t pid = fork();
                        if (pid < 0) {          // error in fork 
                            printf("Failed to fork\n");
                            exit(1);
                        }
                        else if (pid == 0) {    // child process 
                            signal(SIGTSTP, SIG_DFL);
                            signal(SIGINT, SIG_DFL);
                            parser(tokens, k);
                        }
                        else {      // parent process 
                            waitpid(-1, 0, WUNTRACED);  
                        } 
                    }
                }
                else printf("History Error!\n");    
            }
            else {
                for(int i = 0; i < history_count; i++) 
                    printf("%d) %s\n", i+1, history[i]);
            }
        }
        else if(!tokenHandler(tokens)) {  // after creating an array that contains all the tokens, pass it to a function that handles them
            pid_t pid = fork();
            if (pid < 0) {          // error in fork 
                printf("Failed to fork\n");
                exit(1);
            }
            else if (pid == 0) {    // child process 
                signal(SIGTSTP, SIG_DFL);
                signal(SIGINT, SIG_DFL);
                parser(tokens, i);
            }
            else {      // parent process 
                waitpid(-1, 0, WUNTRACED);  
            } 
        }

	}
    
	return 0;
}