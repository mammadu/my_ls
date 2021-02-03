#include <dirent.h>
#include <stdio.h>
#include "my_ls_functions.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



int link_sort_test() //should print abcde to command line
{
    node* head = create_link("b");
    node* first_link = create_link("a");
    node* second_link = create_link("e");
    node* third_link = create_link("c");
    node* fourth_link = create_link("d");
    head = sort_link(fourth_link, head, 0);
    head = sort_link(first_link, head, 0);
    head = sort_link(third_link, head, 0);
    head = sort_link(second_link, head, 0);
    read_list(head);
    printf("if 'abcde' -> success, otherwise fail");
    return 0;
}

int print_dir()
{
    DIR *folder;
    struct dirent *entry;
    struct stat filestat;

    folder = opendir(".");
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

    /* Read directory entries */
    
    while( (entry=readdir(folder)) )
    {
        stat(entry->d_name,&filestat);
        if( S_ISDIR(filestat.st_mode) )
        {
            printf("%5s: %-20s %ld:%ld\n", "DIR", entry->d_name, filestat.st_mtime, filestat.st_mtim.tv_nsec);
            // printf("timestamp: %ld\n", filestat.st_mtime);
            // printf("timestamp nano: %ld\n", filestat.st_mtim.tv_nsec);
        }         
        else
        {
            printf("%5s: %-20s %ld:%ld\n", "FILE", entry->d_name, filestat.st_mtim.tv_sec, filestat.st_mtim.tv_nsec);
            // printf("timestamp: %ld\n", filestat.st_mtim.tv_sec);
            // printf("timestamp nano: %ld\n", filestat.st_mtim.tv_nsec);
        }
    }
    closedir(folder);

    return(0);
}

int main()
{
    // printf("sametime1 vs sametime2 = %d", strcmp("sametime1", "sametime2"));
    return print_dir();
}
