#include "headers.h"


void bg(char args[][MAXLINE], int tokens) {

    int j = 0, stop = 0, k = 0, i = 0;
    char *command[256];
    int jobs[100];        
    int num_job = 0;

    while (args[j] != NULL && stop != 1) {          
        
        // for each group of commands between '&', create  
        // a new array and execute them seperately
		k = 0;
        int back = 0;
        
		while (strcmp(args[j],"&") != 0) {
			command[k] = args[j++];
			if (j == tokens) {        
				stop = 1;
				k++;
				break;
			}
			k++;
		}
        
        if(strcmp(args[j], "&") == 0)
            back = 1; // command should run on the background

		command[k] = NULL;
		j++;	
        
        // i now have an array of commands ee -> [sleep | 2]
        // and i execute them in the background if needed

        pid_t pid = fork();
        
        if (pid < 0) {          // Error forking
            perror("fork");
        } else if (pid == 0) {  // Child process
            parser(command, k);
        } 
        else {                // Parent process
            if(!back) {
                waitpid(pid, NULL, 0);
            }
        }
    }
    exit(0);
}