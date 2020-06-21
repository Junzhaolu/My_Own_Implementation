flag=g++ -Wall -g
val=valgrind --tool=memcheck --leak-check=yes

all: main valgrind

main: print_bst.h bst.h splay_tree.h main.cpp  
	$(flag) $^ -o main

valgrind: main
	$(val) ./$<

clean:
	@rm main