/*
My goal is to make my own version of my_ls

the cmd line call will be as follows:
./my_ls [-at] [file ...]

Where './my_ls' is the name of the program, '-at' are the possible flags allowed, 'files' is the name of one or more directories/files

*/

#include <dirent.h>
#include <stdio.h>
#include "my_ls_functions.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//This tests the create_struct and read_flag functions
int main(int argc, char *argv[])
{
    lsd data = create_struct(argc);
    int flag_value = 0; //Set flag value to 0 in case there are no flags
    int flag_index = 0;
    for (int i = 1; i < argc; i++) //check all arguments for flags
    {
        if (argv[i][0] == '-')
        {
            flag_value = read_flag(argv[i], &data); //return 0 on success
            flag_index = i;
        }
        if (flag_value != 0)
        {
            printf("invalid option  -- '%c'", argv[i][flag_value]);
            return -1;
        }
    }


    // for (int i = 1; i < argc; i++) //determine whether each argument is a directory or a file
    // {
    //     if (i != flag_index)
    //     {
    //         if (item_type(argv[i]) == -1)
    //         {
    //             log_item(argv[i], data.does_not_exist, data.dne_count);
    //             data.dne_count++;
    //         }
    //         else if (item_type(argv[i]) == 0) //logs directories into the directory array
    //         {
    //             log_item(argv[i], data.directories, data.directory_count);
    //             data.directory_count++;
    //         }
    //         else if (item_type(argv[i]) == 1) //logs files into the file array
    //         {
    //             log_item(argv[i], data.files, data.file_count);
    //             data.file_count++;
    //         }
    //     }        
    // }

    for (int i = 1; i < argc; i++) //determine whether each argument is a directory or a file
    {
        if (i != flag_index)
        {
            if (item_type(argv[i]) == -1)
            {
                node* non_item = create_link(argv[i]);
                printf("%p\n", data.does_not_exist);
                append_link(non_item, data.does_not_exist);
                printf("%p\n", data.does_not_exist);
                read_list(data.does_not_exist);
                // log_item(argv[i], data.does_not_exist, data.dne_count);
                // data.dne_count++;
            }
            else if (item_type(argv[i]) == 0) //logs directories into the directory array
            {
                node* dir = create_link(argv[i]);
                data.directories = sort_link(dir, data.directories, data.flagT);
                // log_item(argv[i], data.directories, data.directory_count);
                // data.directory_count++;
            }
            else if (item_type(argv[i]) == 1) //logs files into the file array
            {
                node* file = create_link(argv[i]);
                data.files = sort_link(file, data.files, data.flagT);
                // log_item(argv[i], data.files, data.file_count);
                // data.file_count++;
            }
        }        
    }



    // printf("flagA = %d\n", data.flagA);
    // printf("flagT = %d\n", data.flagT);
    // for (int i = 0; i < data.dne_count; i++)
    // {
    //     printf("does_not_exist[%d] =  %s\n", i, data.does_not_exist[i]);
    // }
    // for (int i = 0; i < data.file_count; i++)
    // {
    //     printf("file[%d] =  %s\n", i, data.files[i]);
    // }
    // for (int i = 0; i < data.directory_count; i++)
    // {
    //     printf("directory[%d] = %s\n", i, data.directories[i]);
    // }
}

/*
Next steps:

complete printdir.c file
determine how to handle no arguments

*/