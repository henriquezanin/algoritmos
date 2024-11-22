all: clean compile clean

clean:
	@find -name '*.o' | xargs rm -rf

compile:
	@gcc -o captcha *.c -I. -Wall -lm

run:
	@./captcha

zip:
	@zip -r captcha.zip *.h *.c Makefile