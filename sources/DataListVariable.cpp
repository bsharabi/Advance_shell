#include "DataListVariable.hpp"

Data *headData = NULL;
Data *tailData = NULL;

int Empty()
{
    // If head is NULL, the list is empty
    // Otherwise, the list is not empty
    return (headData == NULL) ? 1 : 0;
}

int add(const char *name, const char *value)
{
    Data *new_node;
    // Check if the name already exists in the list
    // If the name already exists, replace the value
    if ((new_node = search(name)))
    {
        // Allocate memory for the new value string
        if (strcmp(value, new_node->value) == 0)
            return 0;
        char *new_value = (char *)malloc(strlen(value) + 1);
        if (new_value == NULL)
            return 0;
        free(new_node->value);
        new_node->value = new_value;
        strcpy(new_node->value, value);
        return 1;
    }
    // Allocate memory for the new Data
    new_node = (Data *)malloc(sizeof(Data));
    if (new_node == NULL)
    {
        return 0;
    }
    // Copy the name and value strings into the new Data
    new_node->name = (char *)malloc(strlen(name) + 1);
    if (new_node->name == NULL)
    {
        free(new_node);
        return 0;
    }
    strcpy(new_node->name, name);
    new_node->value = (char *)malloc(strlen(value) + 1);
    if (new_node->value == NULL)
    {
        free(new_node->name);
        free(new_node);
        return 0;
    }
    strcpy(new_node->value, value);
    // Set the new Data's next pointer to NULL
    new_node->next = NULL;
    // If the list is empty, set the new Data as the head and tail
    if (headData == NULL)
    {
        headData = new_node;
        tailData = new_node;
    }
    // Otherwise, add the new Data to the end of the list
    else
    {
        tailData->next = new_node;
        tailData = new_node;
    }
    return 1;
}

Data *search(const char *name)
{
    if (Empty())
        return NULL;
    Data *current = headData;
    while (current)
    {

        if (strcmp(name, current->name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void freeLinkedList()
{
    Data *current = headData;
    while (current != NULL)
    {
        // Store a pointer to the next node before freeing the current node
        Data *next = current->next;
        // Free the memory used by the name and value strings
        free(current->name);
        free(current->value);
        // Free the memory used by the current node
        free(current);
        // Move on to the next node
        current = next;
    }
    // Set the head and tail pointers to NULL to indicate that the list is empty
    headData = tailData = NULL;
}

void showList()
{
    Data *current = headData;
    while (current)
    {
        printf("{Name %s, ", current->name);
        printf("Value %s} ", current->value);
        printf("->");
        current = current->next;
    }
    puts("");
}

// int main()
// {
//     add("hello", "dd");
//     showList();
//     add("hello", "dd");
//     add("hello", "dd");
//     add("hello", "dd");
//     showList();
//     add("hello", "dd3");
//     showList();
//     add("hello", "dd5");
//     showList();
//     add("helldo", "dd5");
//     add("helldso", "dd5");
//     add("helldod", "dd5");
//     showList();
//     freeLinkedList();
//     return 0;
// }
