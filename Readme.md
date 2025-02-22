
# **Shell Command Handler**

## **Token Handler (`int tokenHandler(char *token[])`)**

### **`exit()`**
- The **`exit`** function is implemented separately instead of using **`execvp()`**, as we do not want to execute a new program.
- If executed in a bash shell, **`exit`** terminates the shell.
- If executed in a nested shell, only the inner shell terminates, returning control to the outer shell.

### **`cd()`**
- Implemented separately since it must change the current working directory of the shell.

---

## **Command Parser (`int parser(char *line[], int token)`)**
- Handles improperly split tokens that may not be separated by a space character.
- Checks for special characters such as:
  - **Pipes (`|`)**
  - **Input (`<`) and output (`>`/`>>`) redirection**
  - **Background process (`&`)**
- Calls the appropriate function when special characters are detected.
- Calls **`execvp()`** to execute correct commands.

---

## **Background Process Handler (`void bg(char args[][1024], int tokens) `)**
- Executes multiple commands in the background.
- Requires at least one **`&`** character to identify background execution.
- Determines whether each command should run in the background or not.
- If a process runs in the background, the parent process does not wait for the child process to finish.

---

## **Output Redirection (`void output_redirection(char *args[], char newArray[][1024], int token, int *i) `)**
- Handles two cases:
  1. **Simple output redirection (`>`):** Opens the file in non-append mode and redirects **`stdout`** to the file.
  2. **Double output redirection (`>>`):** Opens the file in append mode and redirects **`stdout`** to it.

---

## **Input Redirection (`void input_redirection(char *args[], char newArray[][1024], int token, int *i) `)**
- Opens the file specified after **`<`** and redirects **`stdin`** to the file.
- Removes the redirection character and file name before calling the **`parser`** function.

---

## **Pipe Handler (`void pipe_handler(char argv[][1024], int argc) `)**
- Implements the pipe **(`|`)** mechanism, allowing multiple commands to execute sequentially where the output of one serves as input for the next.
- Counts the number of pipes to determine the number of sub-commands.
- For each sub-command:
  - Sets up a pipe between it and the previous sub-command (if applicable).
  - Calls **`parser()`** to execute each sub-command.
- Handles odd and even numbers of commands differently:
  - The last sub-command connects its **`stdin`** to the read end of the previous command's pipe **(`filedes[0]`)**.
  - For an even number of commands, the last sub-command reads from the last pipe and outputs to the terminal.

---

### **Assumptions**
- The alias mechanism supports commands with a maximum of **two words** (such as: **`ls`**, **`ls -la`**).

