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

typedef struct ls_data{
   int directory_count;
   char** directories;
   int file_count;
   char** files;
   int dne_count;
   char** does_not_exist;
   int flagA;
   int flagT;
}lsd;

typedef struct directory_data{ //This will store the name of items inside a directory. 
    char** items;
}dd;

lsd create_struct(int size) //function to make the struct. Size should be the maximum number of arguments you expect to encounter
{
    lsd data;
    data.directory_count = 0;
    data.file_count = 0;
    data.dne_count = 0;
    data.flagA = 0;
    data.flagT = 0;
    data.directories = malloc(size * sizeof(char*));
    data.files = malloc(size * sizeof(char*));
    data.does_not_exist = malloc(size * sizeof(char*));
    return data;
}

int read_flag(char* param_1, lsd* data) //this function will check for specific flags
{
    int i = 1;
    while(param_1[i] != '\0')
    {
        if (param_1[i] != 'a' && param_1[i] != 't')
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
    return -1;
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

void log_item(char* string, char** string_array, int index) //this logs strings to a string array at the specified index
{
    int length = strlen(string);
    string_array[index] = malloc(length * sizeof(char));
    for (int i = 0; i < length; i++)
    {
        string_array[index][i] = string[i];
    }
}


/*
Next steps:

create function to organize string arrays according to a flag
create a function to read items inside a directory to a string array

*/