# -*- MakeFile -*-

my_ls: my_ls.o my_ls_functions.o
	gcc my_ls.o my_ls_functions.o -o my_ls -Wall -Wextra -Werror

my_ls.o: my_ls.c
	gcc -c my_ls.c

my_ls_functions.o: my_ls_functions.c
	gcc -c my_ls_functions

unit_tests: unit_tests.o
	gcc unit_