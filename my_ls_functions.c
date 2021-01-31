/*
printdir.c will do the following:

*open directories
*read the names of the items in the directory
*depending on if this is a file or directory, sort it into the proper data structure
*Organize data structures lexicographically
*interpret flags
**-a: Include directory entries whose names begin with a dot (.).
**-t: Sort by time modified (most recently modified first) before sorting the operands by lexicographical order.
*print out final data structure
*/

#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// typedef struct ls_data{
//    int directory_count;
//    char** directories;
//    int file_count;
//    char** files;
//    int dne_count;
//    char** does_not_exist;
//    int flagA;
//    int flagT;
// }lsd;

typedef struct node
{
    char* string;
    long int time_sec;
    long int time_nano_sec;
    struct node* head;
    struct node* next;
}node;

typedef struct ls_data{
   int directory_count;
   node* directories;
   int file_count;
   node* files;
   int dne_count;
   node* does_not_exist;
   int flagA;
   int flagT;
}lsd;

// lsd create_struct(int size) //function to make the struct. Size should be the maximum number of arguments you expect to encounter
// {
//     lsd data;
//     data.directory_count = 0;
//     data.file_count = 0;
//     data.dne_count = 0;
//     data.flagA = 0;
//     data.flagT = 0;
//     data.directories = malloc(size * sizeof(ll*));
//     data.files = malloc(size * sizeof(ll*));
//     data.does_not_exist = malloc(size * sizeof(ll*));
//     return data;
// }

lsd create_struct() //function to make the struct. Size should be the maximum number of arguments you expect to encounter
{
    lsd data;
    data.directory_count = 0;
    data.file_count = 0;
    data.dne_count = 0;
    data.flagA = 0;
    data.flagT = 0;
    data.directories = NULL;
    data.files = NULL;
    data.does_not_exist = NULL;
    return data;
}


int read_flag(char* param_1, lsd* data) //this function will check for specific flags
{
    int i = 1;
    while(param_1[i] != '\0')
    {
        if (param_1[i] != 'a' && param_1[i] != 't') //returns the index of the character if it's not a recognized flag
        {
            return i;
        }
        else if (param_1[i] == 'a')
        {
            data->flagA = 1;
        }
        else if (param_1[i] == 't')
        {
            data->flagT = 1;
        }
        i++;
    }
    return 0; //return 0 on success
}

int item_type(char* param_1) //this determines if an entry is a not a real path (-1), a directory (0) or a file (1)
{
    struct stat filestat;
    int item_exist = lstat(param_1, &filestat);
    if (item_exist == -1)
    {
        return -1;
    }
    else if (S_ISDIR(filestat.st_mode)) //if this is true, the item is a directory
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// void log_item(char* string, char** string_array, int index) //this logs strings to a string array at the specified index
// {
//     int length = strlen(string);
//     string_array[index] = malloc(length * sizeof(char));
//     for (int i = 0; i < length; i++)
//     {
//         string_array[index][i] = string[i];
//     }
// }

int read_list(node* head)
{  
    int index = 0;
    while (head != NULL)
    {
        index++;
        printf("%s\n", head->string);
        head = head->next;
    }
    return index;
}

long int time_mod_sec(char* data) //what happens when this goes on an non-existing file?
{
    struct stat filestat;
    lstat(data, &filestat);
    return filestat.st_mtim.tv_sec;
}

long int time_mod_nano(char* data)
{
    struct stat filestat;
    lstat(data, &filestat);
    return filestat.st_mtim.tv_nsec;
}

int compare_time(node* first_link, node* second_link)
{
    int sec_diff = first_link->time_sec - second_link->time_sec;
    int nano_diff = first_link->time_nano_sec - second_link->time_nano_sec;
    if (sec_diff == 0)
    {
        return nano_diff;
    }
    else
    {
        return sec_diff;
    }
}


node* create_link(char* data)
{
    node* link = malloc(sizeof(node));
    link->string = strdup(data);
    link->time_sec = time_mod_sec(data);
    link->time_nano_sec = time_mod_nano(data);
    link->next = NULL;    
    return link;
}

void append_link(node* new_link, node* head)
{   
    node* current = head;
    while(current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_link;
}

void insert_link(node* new_link, node* previous_link)
{
    new_link->next = previous_link->next;
    previous_link->next = new_link;
}

node* prepend_link(node* new_link, node* head)
{
    new_link->next = head;
    head = new_link;
    return head;
}


node* sort_lexico(node* new_link, node* head) //for sorting lexicographically. make sure to always return the head
{
    node* current = head;

    //Case if new_link should go before head
    if (strcmp(new_link->string, current->string) < 0) //if new_link is lexicographically smaller (comes before) head
    {
        return prepend_link(new_link, current);
    }
    else
    {
        while (current->next != NULL)// case where link gets inserted bewteen links
        {
            if (strcmp(new_link->string, current->next->string) < 0)
            {
                insert_link(new_link,current);
                return head;
            }
            else
            {
                current = current->next;
            }
        }
        insert_link(new_link,current); //case where link gets added to the end.
        return head;
    }
}

node* sort_mod_time(node* new_link, node* head)
{
    node* current = head;
    
    //Case if new_link should go before head
    if (compare_time(new_link, head) > 0) //if new_link is newer (made after) head
    {
        return prepend_link(new_link, current);
    }
    else
    {
        while (current->next != NULL)// case where link gets inserted bewteen links
        {
            if (compare_time(new_link, current->next) > 0)
            {
                insert_link(new_link,current);
                return head;
            }
            else
            {
                current = current->next;
            }
        }
        insert_link(new_link,current); //case where link gets added to the end.
        return head;
    }
}

node* sort_link(node* new_link, node* head, int flagT) //reads the flag to determine to sort lexicographically or based on time modified. make sure to always return the head
{
    if (flagT == 0)
    {
        return sort_lexico(new_link, head);
    }
    else
    {
        return sort_mod_time(new_link, head);
    }
}


/*
Next steps:

create function to organize string arrays according to a flag
create a function to read items inside a directory to a string array

*/