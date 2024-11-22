all: compile

compile:
	@gcc -o maze.run *.c -I. -Wall -lm

run:
	@./maze.run

zip:
	@zip -r maze.zip *.c *.h Makefile