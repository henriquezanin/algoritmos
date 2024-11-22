all: clean main.o compile clean

main.o:
	@gcc -c main.c bitwise.c -g

clean:
	@find -name '*.o' | xargs rm -rf {}

compile:
	@gcc main.o bitwise.o -o trab1 -g -lm

run:
	@./trab1
	
zip:
	@zip -r trab1.zip *.c *.h Makefile
