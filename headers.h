#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <glob.h>

#define MAXLINE 100
#define MAXTOKENS 100
#define MAX_HISTORY 20
#define MAX_ALIAS 20

#define COLOR_RED   "\x1b[95m"
#define COLOR_RESET "\x1b[0m"
#define BOLD "\x1b[1m"

typedef struct{
	pid_t pid;
	char name[100];
	int status;
} jobs;

typedef struct{
	jobs runningprocess[20];
	int activeprocesses[20];
	int count;
    int test;
}ProcessList;


typedef struct{
    char shortcut[MAXLINE];
    char command[MAXLINE];
} Alias;


int tokenHandler(char *token[]);
int parser(char *args[], int tokens);
void input_redirection(char *args[], char newArray[][MAXLINE], int token, int *i);
void output_redirection(char *args[], char newArray[][MAXLINE],int token, int *i);
void pipe_handler(char args[][MAXLINE], int tokens);
void bg(char args[][MAXLINE], int tokens);
void sigintHandler(int sig_num);
void sigtstpHandler(int sig_num);
int alias(char *args[], int tokens);
void replaceAlias(char *(*tokens_ref)[MAXTOKENS], int i);
int destroyAlias(char *args[], int tokens);
int wild_characters(int argc, char *argv[]);
void deleteString(char arr[][MAXLINE], int index, int size);