COMP=gcc
FLAG=-Wall

all:	JAAT main


main: main.c 
	$(COMP) $(FLAG) -o JAAT -O3 -Llib/ -Ilib/ main.c -l:JAAT.a

JAAT: ./lib/JAAT.h
	$(COMP) $(FLAG) -c -x c -I./lib -o ./lib/JAAT.o -O3 ./lib/JAAT.h
	ar rc ./lib/JAAT.a ./lib/*.o 
	ranlib ./lib/JAAT.a 


devices: ./lib/devices.h
	$(COMP) $(FLAGS) -c x c -o ./lib/devices.o -I./lib/devs -O3 ./lib/devices.h 

