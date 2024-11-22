all: compile

compile:
	@gcc -o main.bin  *.c -I. -Wall 

zip:
	@zip -r run_lenght.zip *.c *.h Makefile

run:
	@./main.bin
