# JAAT VM

> [!NOTE]  
> This is an implementation that is design to be educationally understandable with a low complexity, this does not necessary mean that is simple to use or easy modifiable as you may want, and therefore that could not be what you're searching. If you decide to use this library for some reason then I suggest to look at the source code of JAAT.h to better understand the machine architecture.



### What's JAAT

JAAT.h is a simple single header library that introduce a basic implementation of a stack based machine. It's simple to understand and simple to modify due to the minimalistic way that it was designed from the ground up.
Here the JAAT.h is used with a main.c program to create a sort of backend that can use a simple preprocessor to analize text files with the assemly that JAAT can execute. If for example someone need to create a simple JIT compiler he can use the executable of JAAT as it is and feed him the translated program to start the execution.
Yes if you check there is only the header library, that's because is a single header library, as you can see inside the main.c before including the header you must define the specific preprocessor flag ( "#define JAAT_IMPLEMENTATION" ) to include the implementation for each function.


The library was designed like that because I want this to be as simple as possible, but also portable and easy to use in every project, in fact the main.c as I stated before is the implementation of a simple "preprocessor" for the Assembly-like code that JAAT can use, and the only thing tha this preprocessor doeas is just copy the whole code without comments and feed it to JAAT for the execution, nothing else. 

This means that as it is the JAAT.h contain already what you want and since the comment capabilities is something that you use if you directly use the Assembly-like language to build something ( so if you use the default JAAT executable as a JIT compiler/machine ) this was only included in the sort of "interface" that main.c is, it's just a wrapper to use JAAT.h as an executable.
This capabilities will be lost if you use only the JAAT.h, but since in this case you probably use a code generator to create the program for JAAT, so there is no need to introduce comment's in code that is not supposed to be read. 



### Possible paramenter if you build the JAAT executable backend:

If you are using the JAAT executable backend those are the possible options:

-h : print help

-l : print list of instruction

-ex [n] : execute some example programm included inside the executable

-n [file] : load external programm


## VM instruction 


The instruction set is inspired by the simple ASM of the 6502 microprocessor. Therefore there is a total of 31 ready to use instructions that cover most of the possible requirement for a turing complete machine. 
To create a more complex type of instruction to trigger a specific behaviour inside the Virtual Machine you can:


- directly modify the list of instruction and customise the ISA utilized by the VM ( hard but optimal ways )

- creating a routine inside your program to cover what you need ( classic approach for a RISC type of ISA that is used by real life compiler )
