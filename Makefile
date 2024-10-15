COMP=gcc
FLAG=-Wall -lm

all:	main


main: main.c 
	$(COMP) $(FLAG) -o JAAT main.c
	
