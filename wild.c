#include "headers.h"

// this function is being called in my main function and takes as argumnts the array of tokens. It then checks if there is 
// any wild character '*'. If any spotted is counts the number of files matching any wildcard pattern in the array of tokens.   
// It then crates a copy of the original array of tokens and the expanded file names nad finaly executes them. 

int wild_characters(int argc, char *argv[]) {
    if (argc < 2)  return -1;
    // Count the number of files matching the wildcard pattern
    int file_count = 0;
    for (int i = 1; i < argc; i++) {
        if (strchr(argv[i], '*') != NULL || strchr(argv[i], '?') != NULL) {     
            glob_t files;
            int result = glob(argv[i], 0, NULL, &files);
            if (result == 0) {
                file_count += files.gl_pathc;
                globfree(&files);
            }
            else return 1;
        }
        if(file_count == 0) return -1;
    }

    // Allocate a new array of tokens with enough space for the expanded file names
    char **new_argv = (char **)malloc((argc + file_count) * sizeof(char *));
    if (new_argv == NULL) {
        perror("malloc failed");
        return -1;
    }

    // Copy the original tokens to the new array, expanding any wildcard patterns
    int j = 0;
    for (int i = 0; i < argc; i++) {
        if (strchr(argv[i], '*') != NULL) {
        glob_t files;
        int result = glob(argv[i], 0, NULL, &files);
        if (result == 0) {
            for (size_t k = 0; k < files.gl_pathc; k++) {
                new_argv[j++] = strdup(files.gl_pathv[k]);
            }
            globfree(&files);
        }
        } else {
            new_argv[j++] = argv[i];
        }
    }
    new_argv[j] = NULL;

    // exec the command 
    pid_t pid = fork();
    if (pid < 0) {          // error in fork 
        printf("Failed to fork\n");
        exit(1);
    }
    else if (pid == 0) {    // child process 
        parser(new_argv, j);
        // free the new array 
        for (int i = 0; i < j; i++) {
            free(new_argv[i]);
        }
        free(new_argv);
    }
    else {      // parent process 
        wait(NULL);
    } 

    
    return 1;
}