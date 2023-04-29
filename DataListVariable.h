/**
 * @file DataListVariable.h
 * @author Barak Sharabi (https://github.com/bsharabi)
 * @brief
 * @version 0.1
 * @date 2023-04-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __Data_H__
#define __Data_H__
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

typedef struct data
{
    struct data *next;
    char *name;
    char *value;
} Data;


#endif
// Function to check if the list is empty
int Empty();

// Function to add a new node to the linked list with the given name and value
// Returns 1 if successful, 0 otherwise
int add(const char *name, const char *value);

// Function to search for a data with the given name in the linked list
// Returns a pointer to the data if found, or NULL if not found
Data *search(const char *name);

// Function to release the memory used by the linked list
void freeLinkedList();

// Function to print the linked list
void showList();