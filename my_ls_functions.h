#ifndef my_ls_functions_H
#define my_ls_functions_H

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

lsd create_struct();

int read_flag(char* param_1, lsd* data);

int item_type(char* param_1);

int read_list(node* head);

node* create_link(char* data);

void append_link(node* new_link, node* head);

void insert_link(node* new_link, node* previous_link);

node* prepend_link(node* new_link, node* head);

node* sort_lexico(node* new_link, node* head);

node* sort_mod_time(node* new_link, node* head);

node* sort_link(node* new_link, node* head, int flagT);

void fill_dir(node* link, int flagT);

void fill_all_dir(lsd data);

void read_dir(lsd data);

void read_does_not_exist(node* does_not_exist);

#endif