#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
    char* string;
    long int time_sec;
    long int time_nano_sec;
    struct node* sub_items;
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

int string_length(char* string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        i++;
    }
    return i;
}

char* combine_strings(char* first_string, char* second_string)
{
    char* return_val = malloc((string_length(first_string) + string_length(second_string)) * sizeof(char));
    int i = 0;
    while (first_string[i] != '\0')
    {
        return_val[i] = first_string[i];
        i++;
    }
    int j = 0;
    while (second_string[j] != '\0')
    {
        return_val[i] = second_string[j];
        i++;
        j++;
    }
    return return_val;
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

int read_list(node* head)
{  
    int index = 0;
    while (head != NULL)
    {
        index++;
        printf("%s  ", head->string);
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

int compare_lexico(char* first_string, char* second_string)
{
    int diff = 0;
    int i = 0;
    while (first_string[i] != '\0' && second_string[i] != '\0' && first_string[i] == second_string[i])
    {
        i++;
    }
    diff = first_string[i] - second_string[i];
    return diff;
}

char* my_strdup(char* param_1)
{
    char* str_copy;
    int length = string_length(param_1);
    str_copy = (char*)malloc(length * sizeof(char));
    for(int i = 0; i < length; i++)
    {
        str_copy[i] = param_1[i];
    }
    return str_copy;
}

node* create_link(char* data)
{
    node* link = malloc(sizeof(node));
    link->string = my_strdup(data);
    link->time_sec = time_mod_sec(data);
    link->time_nano_sec = time_mod_nano(data);
    link->sub_items = NULL;
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
    if (compare_lexico(new_link->string, current->string) < 0) //if new_link is lexicographically smaller (comes before) head
    {
        return prepend_link(new_link, current);
    }
    else
    {
        while (current->next != NULL)// case where link gets inserted bewteen links
        {
            if (compare_lexico(new_link->string, current->next->string) < 0)
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
    if(compare_time(new_link, current) == 0)
    {
        if (compare_lexico(new_link->string, current->string) < 0) //if new_link goes before current->next lexicographically
        {
            return prepend_link(new_link, current);
        }
        else
        {
            insert_link(new_link, current);
            return head;
        }
    }
    if (compare_time(new_link, current) > 0) //if new_link is newer (made after) head
    {
        return prepend_link(new_link, current);
    }
    else
    {
        while (current->next != NULL)// case where link gets inserted bewteen links
        {
            if(compare_time(new_link, current->next) == 0)
            {
                if (compare_lexico(new_link->string, current->next->string) < 0) //if new_link goes before current->next lexicographically
                {
                    insert_link(new_link, current);
                    return head;
                }
                else
                {
                    insert_link(new_link, current->next);
                    return head;
                }                
            }
            else if (compare_time(new_link, current->next) > 0)
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

void fill_dir(node* link, int flagA, int flagT)//lists the items in a directory
{
    
    DIR *folder;
    struct dirent *entry;
    struct stat filestat;
    folder = opendir(link->string);
    // int i = 0;
    while( (entry=readdir(folder)) )
    {
        if (flagA == 0 && entry->d_name[0] == '.')
        {
            continue;
        }
        else
        {
            node* dir_item = create_link(entry->d_name);
            //The next 4 lines update the modification time of the item. I'll have to make a function for this.
            char* path = combine_strings(link->string, "/");
            path = combine_strings(path, entry->d_name);
            dir_item->time_sec = time_mod_sec(path);
            dir_item->time_nano_sec = time_mod_nano(path);
            if (link->sub_items == NULL)
            {
                link->sub_items = dir_item;
            }
            else
            {
                link->sub_items = sort_link(dir_item, link->sub_items, flagT);
            }
        }
    }
}

void fill_all_dir(lsd data)//lists the items in every directory in the linked list
{
    node* current = data.directories;
    while (current != NULL)
    {
        fill_dir(current, data.flagA, data.flagT);
        current = current->next;
    }
}

void read_dir(lsd data)
{
    if (data.directory_count < 2)
    {
        read_list(data.directories->sub_items);
    }
    else
    {
        node* current = data.directories;
        while(current != NULL)
        {
            printf("%s:\n", current->string);
            if(read_list(current->sub_items) > 0)
            {
                printf("\n");
            }
            if (current->next != NULL)
            {
                printf("\n");
            }
            current = current->next;
        }        
    }
}

void read_does_not_exist(node* does_not_exist)
{
    node* current = does_not_exist;
    while (current != NULL)
    {
        printf("my_ls: cannot access '%s': No such file or directory\n", current->string);
        current = current->next;
    }
}


/*
Next steps:

*/