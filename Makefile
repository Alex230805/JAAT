COMP=gcc
FLAG=-Wall

all:	JAAT main


main: main.c 
	$(COMP) $(FLAG) -o JAAT -Ilib/ main.c

JAAT: ./lib/JAAT.h
	$(COMP) $(FLAG) -c -x c -o ./lib/JAAT.o ./lib/JAAT.h
