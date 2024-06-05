#include "headers.h"
#include <ctype.h>

#include "globals.h"

void deleteString(char arr[][MAXLINE], int index, int size) {
    for (int i = index; i < size - 1; i++) {
        strcpy(arr[i], arr[i+1]);
    }
    arr[size-1][0] = '\0';
}


void output_redirection(char *args[], char newArray[][MAXLINE], int token, int *i) {

    if(strcmp(newArray[*i+1], ">") == 0) {    // case : double output redirection (>>)
        for(int j = 0; j < *i; j++) {
            args[j] = newArray[j];
        }
        (*i)++;
        int fd = open(newArray[++(*i)], O_RDWR | O_CREAT | O_APPEND, 0666);
        if (fd < 0) {
            perror("-error- opening output file");
            exit(1);
        }

        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("-error- redirecting stdout");
            exit(1);
        }
        close(fd);

        execvp(args[0], args);
        perror("Error executing command");
        exit(1);

    }
    else {
        (*i)++;
        int fd = open(newArray[*i], O_RDWR | O_CREAT, 0666);
        if (fd < 0) {
            perror("-error- opening output file");
            exit(1);
        }

        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("-error- redirecting stdout");
            exit(1);
        }
        close(fd);

        deleteString(newArray, *i+1, token);
        token--;
        
    }        
}


void input_redirection(char *args[], char newArray[][MAXLINE], int token, int *i) {

    int fd = open(newArray[*i+1], O_RDONLY, 0);
    if (fd == -1) {
        perror("-error- opening input file");
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) < 0) {
        perror("-error- redirecting input\n");
        exit(1);
    }
    close(fd);

    deleteString(newArray, *i+1, token);
    token--;
}
