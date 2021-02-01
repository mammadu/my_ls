# -*- MakeFile -*-

all: my_ls

test: my_ls_sort ls_test

my_ls: my_ls.o my_ls_functions.o
	gcc my_ls.o my_ls_functions.o -o my_ls -Wall -Wextra -Werror

my_ls.o: my_ls.c my_ls_functions.h
	gcc -c my_ls.c

my_ls_functions.o: my_ls_functions.c
	gcc -c my_ls_functions.c

unit_tests: unit_tests.o my_ls_functions.o
	gcc  unit_tests.o my_ls_functions.o -o unit_tests

unit_tests.o: unit_tests.c my_ls_functions.h
	gcc -c unit_tests.c

ls_test:
	@echo ""
	ls -1t
my_ls_sort: my_ls
	./my_ls unit_tests unit_tests.o unit_tests.c my_ls my_ls_functions.o my_ls.o bunga .git my_project2 my_project README.md my_ls.c printdir.c my_ls_functions.c my_ls_functions.h makefile unga ungabunga sametime2 sametime1 -t

unit_tests_time: unit_tests
	./unit_tests

clean:
	rm *.o