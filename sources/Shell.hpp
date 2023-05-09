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
#ifndef MYSHELL_H
#define MYSHELL_H
#include "Job.hpp"
#include "DataListVariable.hpp"
#include "Histories.hpp"
#include <time.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
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
#include <readline/history.h>
#define blue() printf("\033[0;34m")
#define white() printf("\033[0;37m")
#define green() printf("\033[0;32m")
#define red() printf("\033[0;31m")
#define purple() printf("\033[0;35m")
#define resetColor() printf("\033[0m")
#define MAX_ACTION 32
#define MAX_ARGV 12
#define MAX_COMMANDS 1024
#define MAX_ACTION 32
#define MAX_ARGV 12
#define MAX_SYMBOL 10
#define MAX_PROMPT 256
extern Action action;
extern regex_t regex;
extern pid_t pid_m;
extern int quit;
extern int *fd;
extern int countOfJob;
extern char command[MAX_COMMANDS];
extern char prompt[MAX_PROMPT];
extern char symbol[MAX_SYMBOL][5];
void initFd(int *write, int *read, int *in, int numberJob, int flag);
void welcome();
void getInput(char *command);
void getLocation(char *prompt);
void changePrompt(char *prompt, char *newPrompt);
void startJob();
void varSubstitution(Process *p);
void printPrompt(char *prompt);
void freeAllAllocateMemory();
void replaceStrings(char *str1, const char *str2, const char *delim);
int initJob(char *command);
void sigHandler(int signum);
int replaceVar(char **data);
void initPipe(int write, int read, int in);
void execJob(int write, int read, int in, Process *p);
void appendToFile(Process *p);
void redirectOut(Process *p);
void reversePipe(int write, int read, int in, Process *p);
void readExec(Process *p);
int buildCondition();
void updateStatusHistories();
void initSignal();
int buildRegex();
void trimCommand();
void freeFD();
int initPipeArray();
int prerrToken(char *token);
int run();
void Substitution(char *command);
void printJobCondition();
void startProccess(Process *p, int write, int read, int in);
#endif
