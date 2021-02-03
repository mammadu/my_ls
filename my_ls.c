/*
My goal is to make my own version of ls

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

    for (int i = 1; i < argc; i++) //determine whether each argument is a directory or a file
    {
        if (argv[i][0] != '-')
        {
            if (item_type(argv[i]) == -1)
            {
                node* non_item = create_link(argv[i]);
                if (data.does_not_exist == NULL)
                {
                    data.does_not_exist = non_item;
                }
                else
                {
                    append_link(non_item, data.does_not_exist);
                }
                data.dne_count++;
            }
            else if (item_type(argv[i]) == 0) //logs directories into the directory array
            {
                node* dir = create_link(argv[i]);
                if(data.directories == NULL)
                {
                    data.directories = dir;
                }
                else
                {
                    data.directories = sort_link(dir, data.directories, data.flagT);
                }
                data.directory_count++;
            }
            else if (item_type(argv[i]) == 1) //logs files into the file array
            {
                node* file = create_link(argv[i]);
                if (data.files == NULL)
                {
                    data.files = file;
                }
                else
                {
                    data.files = sort_link(file, data.files, data.flagT);
                }
                data.file_count++;
            }
        }        
    }
    
    if (data.dne_count == 0 && data.directory_count == 0 && data.file_count == 0) //This is for the case that LS receives no arguments
    {
        data.directories = create_link(".");
        data.directory_count++;
    }

    fill_all_dir(data);
    if(data.dne_count > 0)
    {
        read_does_not_exist(data.does_not_exist);
    }
    if(data.file_count > 0)
    {
        read_list(data.files);
        printf("\n\n");
    }
    if (data.directory_count > 0)
    {
        read_dir(data);
        printf("\n");
    }    
}

/*
Next steps:

*/