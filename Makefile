COMP=gcc
FLAG=-Wall

all: JAAT main


main: main.c 
	$(COMP) $(FLAG) -o JAAT -O3 -Llib/ -Ilib/ main.c -l:JAAT.a

JAAT: ./lib/JAAT.h
	$(COMP) $(FLAG) -c -x c -I./lib -I./lib/devs -o ./lib/JAAT.o -O3 ./lib/JAAT.h
	ar rc ./lib/JAAT.a ./lib/*.o 
	ranlib ./lib/JAAT.a 


