all: compile

compile:
	@gcc -o trab.bin *.c -I. -Wall -lm

run:
	@./trab.bin

zip:
	@zip -r trab.zip *.c *.h Makefile