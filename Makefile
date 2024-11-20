COMP=gcc
FLAG=-Wall

all:	JAAT main


main: main.c 
	$(COMP) $(FLAG) -o JAAT -O3 -Llib/ -Ilib/ main.c

JAAT: ./lib/JAAT.h
	$(COMP) $(FLAG) -c -x c -o ./lib/JAAT.o -O3 ./lib/JAAT.h
	ar rc ./lib/JAAT.a ./lib/JAAT.o 
	ranlib ./lib/JAAT.a 
