all: compile

compile:
	@gcc -o sparse *.c -I. -Wall

run:
	@./sparse

zip:
	@zip -r sparse.zip *.c *.h Makefile