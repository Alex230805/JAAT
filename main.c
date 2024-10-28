#include <stdio.h>
#include <stdlib.h>

#define JAAT_IMPLEMENTATION

#include "./lib/JAAT.h"

void help_message(){

  Array* help;
  array_new(help);
  array_push(help,"PRT('============================================\n')");
  array_push(help, "PRT('MAIN USAGE: \n\nTo implement a JIT compiler using JAAT you need first a parser\nfor the language, that mean JAAT provide only the intermediate\nlanguage and the vm, but does not include a parser for the instruction.\n')");
  array_push(help, "PRT('JAAT will be called to load the programm and for initialize itsef, then it start the execution.\n')");
  array_push(help, "PRT('\n\nTo better understand how exactly all of that work I highly recommend to take \na look inside the single file library JAAT.h\n\n')");
  array_push(help, "PRT('\nBeside -h, you can use the -ex instruction followed by a number to see some example about how the vm work.\n\n')");
  array_push(help, "PRT('USING JAAT IN YOUR PROJECT:\n\n')");
  array_push(help, "PRT('To include JAAT you need to define first the macro JAAT_IMPLEMENTATION that will use the header like a c file, with all the implementation. This design choice is made with the goal of mantain a simple but yet powerfull architecture.\n\n')");
  array_push(help, "PRT('To load programm in JAAT you need a dynamic array that is needed to dinamically alloc the internal buffer of JAAT and not waste any memory.\n\n')");
  array_push(help, "PRT('to launch JAAT you can use the macro launch_vm() that use a reference to a dynamic array as first argument, or you can do manually.\n')");
  array_push(help, "PRT('If you want to use the JAAT executable for run programm you can provide with the argument -n a new file with your programm written in the JAAT assembly, and the executable will automatically create a dynamic array and launch the VM.\n\n')");
  array_push(help, "PRT('For more example you can check the main.c and see some programms or you can use the -l parameter to get an helper for all instruction.'\n)");
  array_push(help, "PRT('============================================\n\n - btw the helper is written and executed in the vm :D\n')");
  array_push(help, "HLT()");

  launch_vm(help);
}


void list_message(){
  Array*list;
  array_new(list);
  array_push(list,"PRT('============================================\n\n')");
  array_push(list, "PRT('SINTAX STRUCTURE:\n\n')");
  array_push(list, "PRT('NODE : You can add comment inside your code by start a line with //, this tell the parser to not analize this line and go for the next one\n')");

  array_push(list, "PRT('A standard instruction have a name of three chars that look like: PRT() \n')");
  array_push(list, "PRT('Any instruction have a name followed by two parantesis.\n\n')");
  array_push(list, "PRT('Inside those parentesis you can put the accepted argument for the instruction that can be integer, constant like INPUT or ACC (that stands for input and accumulator), or even entire phrase like the PRT('') function variation\n\n')");
  array_push(list, "PRT('Some instruction can be used in different ways, in some cases you need a specific mode to operate\n')");
  array_push(list, "PRT('Normally an instruction use form 0 to two arguments, by adding # just before the parentesis will tell the parser to use all argument like pointer for the stack\n\n')");
  array_push(list, "PRT('An example can be the CMP(arg_0,arg_1) function that use the first argument for get the number and the second for condition.\n')");
  array_push(list, "PRT('If CMP is written like CMP#(arg_0,arg_1) all two argument will be pointer to the stack\n\n')");
  array_push(list, "PRT('SUMMARY:\n\n')");
  array_push(list, "PRT('-> FUN(arg_0,arg_1)  : normal instruction with two argument\n')");
  array_push(list, "PRT('-> FUN#(arg_0,arg_1) : normal instruction but all argument are pointer\n')");
  array_push(list, "PRT('-> FUN(ACC)          : normal instruction but ACC is a costant that reference something with the processor\n')");
  array_push(list, "PRT('-> FUN('Hello World'): normal function that accept a string\n')");
  array_push(list, "PRT('\n'\n)");
  array_push(list, "PRT('LIST OF VALID INSTRUCTION:\n\n')");
  array_push(list, "PRT('HLT() : halt the virtual processor, you need this to stop your programm\n\n')");
  array_push(list, "PRT('PUT(arg_0) : put number passed by argument in stack , use variation with ACC to put accumulator content in stack\n\n')");
  array_push(list, "PRT('POP(arg_0) : pop content from stack and put it in accumulator\n\n')");
  array_push(list, "PRT('GET(arg_0) : get thing from stack and put it inside accumulator, it use the argument provided to address the stack\n\n')");
  array_push(list, "PRT('ADC(arg_0,arg_1) : add accumulator with content of location provided by the second argument, then put the result in the location provided by the first argument, you can use the variation address by add # before parentesis\n\n')");
  array_push(list, "PRT('SBC(arg_0,arg_1) : same as ADC but it perform subtraction\n\n')");
  array_push(list, "PRT('JMP(arg_0) : jump in a specific location of the programm provided by the first argument\n\n')");
  array_push(list, "PRT('JNZ(arg_0) : same as jump but only if the result of the previous operation IS NOT ZERO\n\n')");
  array_push(list, "PRT('JPO(arg_0) : same as jump but only if the result of the previous operation return an OVERFLOW\n\n')");
  array_push(list, "PRT('JEQ(arg_0) : same as jump but if the result of a comparison return an equal state, aka zero flag is setted\n\n')");
  array_push(list, "PRT('JSR(arg_0) : jump to subroutine and save return address in the last 256 location in stack\n\n')");  
  array_push(list, "PRT('RTS(): return from subroutine by set the programm counter to the saved address of jsr\n\n')");
  array_push(list, "PRT('CMP(arg_0,arg_1) : compare two element, first argument is used to get info from the stack, the second is used to set the compare number. You can use the # variation to use both element as address \n\n')");
  array_push(list, "PRT('NXT() and PRV: use to increment or decrement MANUALLY the stack pointer\n\n')");
  array_push(list, "PRT('PRT(arg_0) : print thing on stdout using the first argument that can be a location in the stack, a costant like INPUT to print out the keyboard bufer, or a direct string\n\n')");
  array_push(list, "PRT('INC(arg_0) : increment the number int the indicated location\n\n')");
  array_push(list, "PRT('DEC(arg_0): same as INC but it subtract 1 from the number in stack\n\n')");
  array_push(list, "PRT('SCN() : scan keyboard input and put it in the machine circular buffer\n\n')");
  array_push(list, "PRT('SWP(arg_0,arg_1) : swap data between two block of memory using arguments for addressing the stack\n\n')");
  array_push(list, "PRT('for reference, even this untility is written in the vm language and executed by the vm itself\n\n')");
  array_push(list, "PRT('NOTE: This is only a surface ride of the vm, if you want to know more I recommend to check out JAAT.h\n\n')");

  array_push(list,"PRT('============================================\n\n')");
  array_push(list, "HLT()");
  launch_vm(list);
}

void jaat_ex_1(){
  Array* prg;
  array_new(prg);
  array_push(prg,"PRT('put 69 into stack and add 1 until it reach 420\n')");
  array_push(prg,"PUT(69)");
  array_push(prg,"POP()");
  array_push(prg,"ADC(0,1)");
  array_push(prg,"PRT()");
  array_push(prg,"CMP(0, 420)");
  array_push(prg, "JEQ(9)");
  array_push(prg, "NXT()");
  array_push(prg, "JMP(2)");
  array_push(prg, "HLT()");

  launch_vm(prg);
}


void jaat_ex_2(){
// fibonacci sequence but with a for loop
  Array* prg2;
  array_new(prg2);
  array_push(prg2, "PRT('fibonacci sequence, but with for loop locked on 17\n')");
  array_push(prg2, "PUT(1)");
  array_push(prg2, "PUT(1)");
  array_push(prg2, "PUT(0)");
  array_push(prg2, "ADC#(0,1)");
  array_push(prg2, "SWP(0,1)");
  array_push(prg2, "INC(2)");
  array_push(prg2, "CMP(2, 17)");
  array_push(prg2, "JNZ(3)");
  array_push(prg2, "PRT(1)");
  array_push(prg2, "HLT()");

  launch_vm(prg2);

}

void jaat_ex_4(){
  Array*prg4;
  array_new(prg4);
  array_push(prg4, "PRT('Input test, try to write something: ')");
  array_push(prg4, "SCN()");
  array_push(prg4, "PRT('You write: ')");
  array_push(prg4, "PRT(INPUT)");
  array_push(prg4, "HLT()");

  launch_vm(prg4);
}

int main(int argc, char** argv){
  if(argc > 1){
    for(int i=0; i<argc;i++){
      if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)){
        help_message();
      }
      if((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--list") == 0)){
        list_message();
      }if((strcmp(argv[i], "-n") == 0) || (strcmp(argv[i], "--new") == 0)){
        if(argv[i+1] != NULL){

        /*

          =======================================

          JAAT BACKEND PREPROCESSOR

          ======================================

          */           
          
          FILE *programm;
          if((programm = fopen(argv[i+1], "rb")) == NULL){
            fprintf(stderr, "ERROR: unable to open file -> %d\n", errno);
            exit(errno);
          }
          Array* custom_programm;
          array_new(custom_programm);
          fseek(programm, 0,SEEK_SET);
          char* buffer_t;
          char * state = "a"; 
          while(state != NULL){
            buffer_t = (char*)malloc(sizeof(char)*255);
            state = fgets(buffer_t, 255, programm);
            // check for special chars in string like \n or \t
            if(buffer_t[0] != 10 && (buffer_t[0] != '/' && buffer_t[1] != '/' && state != NULL)){ 
              char new_buffer[255];
              for(int i=0;i<255 && buffer_t[i] != 0; i++){

                if(buffer_t[i] == 92 && buffer_t[i+1] == 110){  // \n string literal
                  buffer_t[i] = 10;
                  strcpy(new_buffer, &buffer_t[i+2]);
                  strcpy(&buffer_t[i+1], new_buffer);
                }
                if(buffer_t[i] == 92 && buffer_t[i+1] == 116){  // \t string literal
                  buffer_t[i] = 9;
                  strcpy(new_buffer, &buffer_t[i+2]);
                  strcpy(&buffer_t[i+1], new_buffer);
                }

              }
            
              char* len = (char*)memchr(buffer_t, ')', strlen(buffer_t));
              char* name_space = (char*)memchr(buffer_t, '!', strlen(buffer_t));
              if(len != NULL){
                buffer_t[len-buffer_t+1] = 0;
                array_push(custom_programm, buffer_t);
              }
              if(name_space != NULL){
                buffer_t[strlen(buffer_t)-1] = 0;
                array_push(custom_programm, buffer_t);
              }
            }
          }
          launch_vm(custom_programm);
          fclose(programm);

        }else{
          fprintf(stderr,"ERROR: missing argument");
        }
      }if(strcmp(argv[i], "-ex") == 0 && argc > i+1){
        if(strcmp(argv[i+1], "1") == 0) {
          jaat_ex_1();
        }else if(strcmp(argv[i+1], "2") == 0){
          jaat_ex_2();
        }else if(strcmp(argv[i+1], "3") == 0){
          jaat_ex_4();
        }else fprintf(stderr,"ERROR: missing argument");
      }
    }
  }else{
    printf("Possible parameters:\n");
    printf("-h : print help\n");
    printf("-l : print list of instruction\n");
    printf("-ex [n] : execute some example programm\n");
    printf("-n [file] : load external programm\n");
  }
  return 0;
}


