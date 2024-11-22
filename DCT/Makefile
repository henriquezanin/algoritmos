all: clean object compile clean

object:
	@gcc -c dct.c main.c -g -I.

compile:
	@gcc dct.o main.o -o dct -g -lm -I.

run:
	@./dct

clean:
	@find -name '*.o' | xargs rm -rf 

zip:
	@zip -r dct.zip *.h *.c Makefile
