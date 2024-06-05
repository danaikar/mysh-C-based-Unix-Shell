

-----------------------------------------------------------------------------------------------------------
int tokenHandler(char *token[]):
-----------------------------------------------------------------------------------------------------------

-exit(): 
We implement Exit Function separately and not using the execvp(), because we dont want to execute a new program.
Another detail is that if you are at a bash shell for example and type exit, it will terminate the shell, but if you are in a nested shell and type exit, only the inner shell will terminate and the control will be returned to the outer shell.
'
-cd()
We also implement this function separately bacuse it should change the current working director of the shell.


-----------------------------------------------------------------------------------------------------------
int parser(char *line[], int token):
-----------------------------------------------------------------------------------------------------------
In this function we handle tokens that maybe are not split correctly because they are not separated by space character. 
Then it checks for special characters such as pipes, input/output redirection, and background processes and if any of these special characters are present, it calls the appropriate function. 
Lastly, the function calls execvp() for the correct commands.




-----------------------------------------------------------------------------------------------------------
void bg(char args[][1024], int tokens):
-----------------------------------------------------------------------------------------------------------
This function is responsible for executing multiple commands in the background but in general it requires at leats one '&' character to be called and start handling each commad either it should run in the background or not. 
Note that if the process is running in the background the parent doesn't wait for the child to finish.


-----------------------------------------------------------------------------------------------------------
void output_redirection(char *args[], char newArray[][1024], int token, int *i):
-----------------------------------------------------------------------------------------------------------
Here we have two cases, simple output redirection and double redirection.
The two cases are similar with the differnce that the in the second one the function opens the file specified after the >> token in append mode, and redirects the stdout to this file, while in the first case the file is being opened in no-append mode.

Similarly, 
void input_redirection(char *args[], char newArray[][1024], int token, int *i):
-----------------------------------------------------------------------------------------------------------
Here we just open the file specified after the < token, and redirect the stdin to this file. 
Lastly, the redirection character and the file name are being deleted and the parse function is being called.





---PIPES---

---------------------------------------------------------
void pipe_handler(char argv[][1024], int argc):
---------------------------------------------------------
This function is responsible for implementing the pipe mechanism, allowing multiple commands to be executed together and the output of one command serving as the input of the next one.
It starts by counting the number of pipes "|" in the array of commands to determine how many sub-commands are there. For each sub-command, the function sets up a pipe between it and the previous sub-command, if there is one and calls parser() to execute each sub-command. 
Note that, odd and even number of commands are treated differently because in the first case it has no following command to connect its output to. So if we are in the last sub-command we connect its stdin to the read end of the pipe stored in filedes[0], which holds the command of the previous iteration. 
Otherwise, for an even number of commands, the last sub-command will use the input from the last pipe and output to the terminal.
  



Assumptions:
1. Alias works for commands with max 2 words, such as "ls", "ls -la"


