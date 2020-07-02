flag=g++ -Wall -g 

val=valgrind --tool=memcheck --leak-check=yes

all: clr_screen main val

clr_screen:
	@clear

main: prefix_trie.h main.cpp
	$(flag) $^ -o main

val: main
	$(val) ./main

clean:
	@rm main