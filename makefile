all: compile

compile: my_ls.c printdir.c
	@gcc my_ls.c -o my_ls -Wall -Wextra -Werror
	# @gcc my_ls.c -o my_ls

