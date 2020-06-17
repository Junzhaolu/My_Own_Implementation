flag=g++ -Wall -g

valgrind=valgrind --tool=memcheck --leak-check=yes ./a

all: main

main: main.cpp rb.h print_bst.h
	$(flag) $< -o a &
	$(valgrind) 

clean:
	@rm -f a