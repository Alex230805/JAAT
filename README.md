# JAAT VM

JAAT.h is a simple single header library that introduce a basic implementation of a modified stack based machine in C. It's simple to understand and simple to modify due to the minimalistic way that it was designed.
Here the JAAT.h is used with a main.c program to create a sort of backend that can use a simple preprocessor to analize text files with the assemly that JAAT can execute. If for example someone need to create a simple JIT compiler he can use the executable of JAAT as it is and feed to it the compiled program by his JIT implementation, so creating first a text file with the program translated to the assembly like of JAAT and then feed this file to the compiled JAAT backend by providing the "-n" flag ( JAAT -n ./your/program.txt ).


JAAT can also be used as a single header library by itself and integrate it inside your project. By first declaring the preprocessor constant "JAAT_IMPLEMENTATION" and then including the header file as usual.


The only difference with the backend at the preprocessing level is the lack of comments inside your code. This features is only provided if the program is loaded by the backend.

### Possible paramenter if you build the JAAT executable backend:

If you are using the JAAT executable those are the possible options:

-h : print help

-l : print list of instruction

-ex [n] : execute some example programm

-n [file] : load external programm


## VM instruction 


The instruction set is inspired by the simple ASM of the 6502 microprocessor. Therefore there is a total of 31 instruction that cover most of the possible usage. 


To create a more complex type of instruction that trigger a specific behaviour inside the Virtual Machine you can:



- directly modify the list of instruction and customise the ISA utilized by the VM ( hard but optimal ways )

- creating a routine inside your program to cover what you need ( classic approach for a RISC type of ISA )



List of instruction used by the VM:

- GET(arg_0) : get item from stack position 'arg_0' and store it in the accumulator. This modify the stack pointer

- GET(CONST) : get data from constant and store it in the accumulator

- PUT(arg_0) : put 'arg_0' into the stack

- PUT('string') : put string into the stack

- PUT(CONST) : put data from the selected const into the stack

- POP(arg_0) : pop item from the stack position 'arg_0' and store it in the accumulator. This modify the stack pointer

- ADC(arg_0,arg_1) : sum arg_1 with the accumulator and store the result inside the stack location arg_0

- ADC#(arg_0,arg_1) : sum stack location arg_1 with stack location arg_0 and store the result inside the stack location arg_0

- SBC(arg_0,arg_1) : subtract arg_1 with the accumulator and store the result inside the stack location arg_0

- SBC#(arg_0,arg_1) : subtract stack location arg_1 with stack location arg_0 and store the result inside the stack location arg_0

- CMP(arg_0,arg_1) : compare between stack location arg_0 and immediate arg_1

- CMP#(arg_0,arg_1) : compare between stack location arg_0 and stack location arg_1

- PRT(arg_0) : print content inside stack location arg_0

- PRT(CONST) : print data associated with the constant reference

- PRT('string') : print the immediate string provided as an argument

- SCN() : scan only for keyboard input

- SWP(arg_0,arg_1) : swap data between stack location arg_0 and stack location arg_1

- INC(arg_0) : increment stack location arg_0

- INC(CONST) : increment data associated with the constant reference

- DEC(arg_0) : decrement stack location arg_0

- DEC(CONST) : decrement data associated with the constant reference

- NXT() : increment the stack pointer

- PRV() : decrement the stack pointer

- PAR(arg_0, arg_1) : put immediate arg_1 inside the extension_bus location arg_0

- PAR#(arg_0, arg_1) : put data in stack location arg_1 inside the extension_bus location arg_0


JUMP related instruction:

note: those instruction can use a label namespace as an argument for the jump

- JMP(arg_0) : jump to the location arg_0

- JZN(arg_0) : jump if the result of a compare is not equal to 0

- JEQ(arg_0) : jump if the result of a compare is equal to 0

- JPO(arg_0) : jump if the result of a compare has generate an overflow

- JSR(arg_0) : jump to subroutine, the return address is pushed in a separate stack

- RTS() : return from subroutine


tips: take a look inside JAAT.h for more clarification
