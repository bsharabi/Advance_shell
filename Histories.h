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

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct history_cmd
{
    struct history_cmd *next; /* next history command in shell */
    struct history_cmd *prev; /* prev history command in shell */
    char *command;            /* for exec */
    time_t dTime;             /* time create command */
    char status[32];          /* true if command has completed */
} History;



int addHistories(char *command);
void freeHistories();
void printAllHistory();
History *getHeadHistories();
History *getTailHistories();
History *getCurrentHistories();