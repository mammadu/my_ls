/*
My goal is to make my own version of my_ls

the cmd line call will be as follows:
./my_ls [-at] [file ...]

Where './my_ls' is the name of the program, '-at' are the possible flags allowed, 'files' is the name of one or more directories/files

*/

#include <dirent.h>
#include <stdio.h>
#include "printdir.c"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>




//This tests the create_struct and read_flag functions
int main(int argc, char *argv[])
{
    lsd data = create_struct(argc);
    int flag_index = 0;
    for (int i = 1; i < argc; i++) //check all arguments for flags
    {
        if (argv[i][0] == '-')
        {
            read_flag(argv[i], &data);
            flag_index = i;
        }
    }    
    for (int i = 1; i < argc; i++) //determine whether each argument is a directory or a file
    {
        if (i != flag_index)
        {
            if (item_type(argv[i]) == 0) //logs directories into the directory array
            {
                log_item(argv[i], data.directories, data.directory_count);
                data.directory_count++;
            }
            else if (item_type(argv[i]) == 1) //logs files into the file array
            {
                log_item(argv[i], data.files, data.file_count);
                data.file_count++;
            }
        }        
    }
    printf("flagA = %d\n", data.flagA);
    printf("flagT = %d\n", data.flagT);
    for (int i = 0; i < data.file_count; i++)
    {
        printf("file[%d] =  %s\n", i, data.files[i]);
    }
    for (int i = 0; i < data.directory_count; i++)
    {
        printf("directory[%d] = %s\n", i, data.directories[i]);
    }
}

/*
Next steps:

complete printdir.c file
determine how to handle no arguments

*/