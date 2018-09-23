all:
	@gcc ./src/*.c -o main -O3 -Wall -lm -I./include

run:
	@./main

debug:
	@gcc ./src/*.c -o main -O3 -Wall -I./include -v -g -lm
	@valgrind --leak-check=full --show-leak-kinds=all -v ./main