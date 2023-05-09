/**
 * @file Job.h
 * @author Barak Sharabi (https://github.com/bsharabi)
 * @brief
 * @version 0.1
 * @date 2023-04-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __Job_HPP__
#define __Job_HPP__
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
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

typedef enum action
{
    Pipe,        /* piping  */
    Append,      /* append  */
    Write,       /* write  */
    WriteError,  /* write to err  */
    ReversePipe, /* reverse pipe  */
    Placement,   /* Placement of value  */
    Read,        /* read command  */
    Normal,      /* normal command  */
    cond,        /* condition command  */
    econd,       /* else if condition command  */
    elcond,      /* else  condition command  */
    then,        /* else condition command  */
    fi           /* else condition command  */
} Action;

typedef struct
{
    char **data;  /* dynamic array of string  */
    int size;     /* number of element in data array */
    int capacity; /* capacity of data array */
} List;

typedef struct process
{
    struct process *next; /* next process in pipeline */
    List argv;            /* for exec */
    pid_t pid;            /* process ID */
    int completed;        /* true if process has completed */
    int stopped;          /* true if process has stopped */
    int status;           /* reported status value */
    Action action;        /* action */
} Process;

/* A job is a pipeline of processes.  */
typedef struct job
{
    struct job *next;   /* next active job */
    Process *head;      /* head of list of processes in this job */
    Process *tail;      /* tail of list of processes in this job */
    pid_t pgid;         /* process group ID */
    int job_number;     /* for count of job */
    Action action;      /* action */
    int process_number; /* for count of process */
    char notified;      /* true if user told about stopped job */
    int completed;      /* true if all process has completed */
    
} Job;

extern Job *headJob;
extern Job *tailJob;
extern Job *headCondition;

int isEmpty();
int initListArgument(List *array);
int appendArgumentToList(List *array, const char *str);
void freeListArgument(List *array);
int addJob();
int addProcess();
int setProcessArgument(Process *process, char *argument);
void freeProccess(Process *process);
void freeJob();
void printJob();
void updateStatusHistories();
int updateStatusProcess(pid_t pid, int completed, int stop, int status);
#endif