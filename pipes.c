#include "headers.h"

void pipe_handler(char argv[][MAXLINE], int argc) {

	// in those file descriptors we set as the input/output of the pipe as:
	// filedes[0]: output, filedes[1]: input 
	int filedes[2]; 
	int filedes2[2];
	int tokens = 0;
	char *command[256];
	pid_t pid;
	int stop = 0;
	int i = 0, j = 0, k = 0, l = 0;
	
	// find the amount of commands, separated by '|'
    for (int b = 0; b < argc; b++) {
        if (strcmp(argv[b], "|") == 0) {
            tokens++;
        }
    }
    tokens++;
	
	// for each command between a pipe we change the input/output
	while (argv[j] != NULL && stop != 1){            

		k = 0;
		// for each group of commands between '|', create  
        // a new array and execute them seperately
		while (strcmp(argv[j],"|") != 0) {
			command[k] = argv[j];
			j++;	
			if (j == argc){
				stop = 1;
				k++;
				break;
			}
			k++;
		}
		command[k] = NULL;
		j++;		

		
		if (i % 2 != 0) pipe(filedes); 
		else pipe(filedes2); 
			

		pid = fork();
		
		if(pid == -1) {   // smth bad happened
			if (i != tokens - 1) {
				if (i % 2 != 0) close(filedes[1]); 
				else close(filedes2[1]);  
			}			
			printf("Failed to fork\n");
			return;
		}
		if(pid == 0) {
			if (i == 0) {
				dup2(filedes2[1], STDOUT_FILENO);
			}
			else if (i == tokens - 1) {
				if (tokens % 2 != 0) dup2(filedes[0],STDIN_FILENO);
				else  dup2(filedes2[0],STDIN_FILENO);			
			}else {  
				if (i % 2 != 0) {
					dup2(filedes2[0],STDIN_FILENO); 
					dup2(filedes[1],STDOUT_FILENO);
				}else { 	 
					dup2(filedes[0],STDIN_FILENO); 
					dup2(filedes2[1],STDOUT_FILENO);					
				} 
			}
						
			parser(command, k); // call parser to execute the sub-command
		}
				
		if (i == 0){
			close(filedes2[1]);
		}
		else if (i == tokens - 1){
			if (tokens % 2 != 0) close(filedes[0]);
			else close(filedes2[0]);
		} else {
			if (i % 2 != 0){					
				close(filedes2[0]);
				close(filedes[1]);
			}else{					
				close(filedes[0]);
				close(filedes2[1]);
			}
		}
				
		waitpid(pid,NULL,0);
				
		i++;	
	}
	exit(0);

}

