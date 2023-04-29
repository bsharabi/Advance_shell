/**
 * @file myShell.h
 * @author Barak Sharabi (https://github.com/bsharabi)
 * @brief
 * @version 1.0
 * @date 2023-04-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <curses.h>
#define blue() printf("\033[0;34m")
#define green() printf("\033[0;32m")
#define purple() printf("\033[0;35m")
#define reset() printf("\033[0m")
#define MAX_ACTION 32
#define MAX_ARGV 12
#define MAX_COMMANDS 1024
#define MAX_ACTION 32
#define MAX_ARGV 12
#define MAX_SYMBOL 10
#define MAX_PROMPT 256
regex_t regex;
int countOfJob;
int quit = 1;
int fd1[2];
int fd2[2];
char prompt[MAX_PROMPT] = "hello";
char command[MAX_COMMANDS] = {0};
char symbol[MAX_SYMBOL][5] = {
    "|",
    ">>",
    ">",
    "2>",
    "<",
    "=",
    "read",
};
pid_t pid_m;

void welcome();
void getInput(char *command);
void getLocation(char *prompt);
void changePrompt(char *prompt, char *newPrompt);
void startJob();
void printPrompt(char *prompt);
void freeAllAllocateMemory();
void replaceStrings(char *str1, char *str2, char *delim);
int initJob(char *command);
void sigHandler(int signum);
void initSignal();
int buildRegex();
void trimCommand();
int run();