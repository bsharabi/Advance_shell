#include "Job.h"
Job *headJob = NULL;
Job *tailJob = NULL;
int isEmpty()
{
    return (headJob == NULL) ? 1 : 0;
}

int initListArgument(List *array)
{
    array->data = (char **)calloc(INITIAL_CAPACITY, sizeof(char *));

    if (!array->data)
        return 0;
    array->size = 0;
    array->capacity = INITIAL_CAPACITY;
    return 1;
}

int appendArgumentToList(List *array, const char *str)
{
    if (array->size == array->capacity)
    {
        // Resize the array if it's full
        array->capacity *= GROWTH_FACTOR;
        array->data = (char **)realloc(array->data, array->capacity * sizeof(char *));
        if (!array->data)
            return 0;
    }
    // Allocate memory for the string and copy it to the array
    array->data[array->size] = (char *)malloc((strlen(str) + 1) * sizeof(char));
    if (!array->data[array->size])
        return 0;
    strcpy(array->data[array->size], str);
    array->size++;
    return 1;
}

void freeListArgument(List *array)
{
    for (int i = 0; i < array->size; i++)
    {
        free(array->data[i]);
    }
    if (array->data != NULL)
        free(array->data);
}

int addJob()
{
    Job *newJob = (Job *)malloc(sizeof(Job));
    if (newJob == NULL)
        return 0;

    if (headJob == NULL)
    {
        headJob = newJob;
        tailJob = newJob;
    }
    else
    {
        tailJob->next = newJob;
        tailJob = newJob;
    }
    tailJob->next = NULL;
    tailJob->head = NULL;
    tailJob->tail = NULL;
    tailJob->process_number = 0;
    return 1;
}

Job *getFirstJob()
{
    return headJob;
}

Job *getTailJob()
{
    return tailJob;
}

int addProcess()
{
    Process *newProcess = (Process *)malloc(sizeof(Process));
    if (newProcess == NULL)
        return 0;
    if (!initListArgument(&newProcess->argv))
    {
        free(newProcess);
        return 0;
    }
    if (tailJob->head == NULL)
    {
        tailJob->head = newProcess;
        tailJob->tail = newProcess;
    }
    else
    {
        tailJob->tail->next = newProcess;
        tailJob->tail = newProcess;
    }
    newProcess->next = NULL;
    newProcess->action = Normal;
    tailJob->process_number++;
    return 1;
}

int setProcessArgument(Process *process, char *argument)
{
    if (argument == NULL || !appendArgumentToList(&process->argv, argument))
    {
        perror("err");
        return 0;
    }
    return 1;
}

void freeProccess(Process *process)
{
    while (process != NULL)
    {
        Process *next = process->next;
        freeListArgument(&process->argv);
        free(process);
        process = next;
    }
}

void freeJob()
{
    Job *current = headJob;
    while (current != NULL)
    {
        Job *next = current->next;
        freeProccess(current->head);
        free(current);
        current = next;
    }
    headJob = tailJob = NULL;
}

void printJob()
{
    Job *current = headJob;
    while (current != NULL)
    {
        printf("   ----------_-JOB-_----------\n");
        printf("compleate -> %d notified -> %d \n", current->completed, current->notified);
        printf("Action -> %d  \n", current->action);
        Process *process = current->head;

        while (process != NULL)
        {
            printf("   ----------_-PROCESS-_ %d----------\n", process->action);
            printf("Argument: ");
            for (int i = 0; i < process->argv.size; i++)
            {
                printf("%s ", process->argv.data[i]);
            }
            printf("\nStatus -> %d compleate -> %d stop -> %d ", process->status, process->completed, process->stopped);
            process = process->next;
            puts("");
        }
        current = current->next;
        puts("");
    }
}

int updateStatusProcess(pid_t pid, int completed, int stop, int status)
{
    Job *head = headJob;
    while (head)
    {
        Process *process = head->head;
        while (process)
        {
            if (process->pid == pid)
            {
                process->status = status;
                process->completed = completed;
                process->stopped = stop;
                return 1;
            }
            process = process->next;
        }
        head = head->next;
    }

    return 0;
}
