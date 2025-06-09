# JAAT VM

> [!NOTE]  
> This is an implementation that is design to be educationally understandable with a low complexity, this does not necessary mean that is simple to use or easy modifiable as you may want, and therefore that could not be what you're searching. If you decide to use this library fore some reason then I suggest to look at the source code of JAAT.h to better understand the machine architecture.



### What's JAAT

JAAT.h is a simple single header library that introduce a basic implementation of a modified stack based machine in C. It's simple to understand and simple to modify due to the minimalistic way that it was designed.
Here the JAAT.h is used with a main.c program to create a sort of backend that can use a simple preprocessor to analize text files with the assemly that JAAT can execute. If for example someone need to create a simple JIT compiler he can use the executable of JAAT as it is and feed him the translated program to start the execution.


JAAT can also be used as a single header library by itself allowing a fast integration in project where is needed. By first declaring the preprocessor constant "JAAT_IMPLEMENTATION" and then including the header file as usual.


The only difference with the backend at the preprocessing level is the lack of comments inside your code. This features is only provided if the program is loaded by the backend. The reason is simple: if the code is generated and then sent to JAAT for the execution, there is no need to integrate a commets feature.

### Possible paramenter if you build the JAAT executable backend:

If you are using the JAAT executable backend those are the possible options:

-h : print help

-l : print list of instruction

-ex [n] : execute some example programm included inside the executable

-n [file] : load external programm


## VM instruction 


The instruction set is inspired by the simple ASM of the 6502 microprocessor. Therefore there is a total of 31 ready to use instructions that cover most of the possible requirement for a turing complete machine. 
To create a more complex type of instruction that trigger a specific behaviour inside the Virtual Machine you can:


- directly modify the list of instruction and customise the ISA utilized by the VM ( hard but optimal ways )

- creating a routine inside your program to cover what you need ( classic approach for a RISC type of ISA )
