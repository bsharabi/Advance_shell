#include "Histories.hpp"

History *tailHistories = NULL;
History *headHistories = NULL;
History *currentHistories = NULL;


int addHistories(char *command)
{
    History *new_history = (History *)malloc(sizeof(History));
    if (new_history == NULL)
        return 0;
    new_history->next = NULL;
    new_history->prev = NULL;
    
    new_history->command = (char *)malloc(sizeof(char) * (strlen(command) + 1));
    if (new_history->command == NULL)
    {
        free(new_history);
        return 0;
    }
    strcpy(new_history->command, command);
    time(&new_history->dTime);

    if (headHistories == NULL)
        headHistories = tailHistories = currentHistories = new_history;
    else
    {
        tailHistories->next = new_history;
        new_history->prev = tailHistories;
        tailHistories = new_history;
        currentHistories = tailHistories;
    }

    return 1;
}
void freeHistories()
{
    History *h = headHistories;
    while (h != NULL)
    {
        if (h->command != NULL)
            free(h->command);
        History *hTemp = h;
        h = h->next;
        free(hTemp);
    }
    headHistories = tailHistories = currentHistories = NULL;
}
void printAllHistory()
{
    History *h = headHistories;
    printf("%s ", "Histories");

    while (h != NULL)
    {
        printf("%s -> ", h->command);
        printf("%s -> ", h->status);
        h = h->next;
    }
    puts("");
}