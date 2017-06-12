all: test_mmap.c
	gcc -o test_mmap -ggdb3 -Wall -Wextra test_mmap.c
