#include <stdio.h>
#include <stdlib.h>

#define JAAT_IMPLEMENTATION

#include "./lib/JAAT.h"

void help_message(){
  vm_programm help = {
    .programm_lenght = 9,
    .inst_array = {
      "PRT('============================================\n')",
      "PRT('\nWelcome to JAAT: a custom and simple vm for JIT compiler\n\n')",
      "PRT('MAIN USAGE: \n\nTo implement a JIT compiler using JAAT you need first a parser\nfor the language, that mean JAAT provide only the intermediate\nlanguage and the vm, but does not include a parser for the instruction.')",
      "PRT('\n\nNormaly a programm can use a parser to generate the vm code using\nthe 'vm_programm struct', that provide the programm length and the array of instruction.')",
      "PRT('JAAT will be called to load the programm and for initialize itsef, then it start the execution.')",
      "PRT('\n\nTo better understand how exactly all of that work I highly recommend to take \na look inside the single file library JAAT.h\n\n')",
      "PRT('\nBeside -h, you can use the -ex instruction followed by a number to see some example about how the vm work.\n\n')",
      "PRT('============================================\n\n - btw the helper is written and executed in the vm :D')",
      "HLT()"
    }
  };
  jaat_start();
  jaat_load_programm(&help);
  jaat_loop();
  jaat_free();
}

void jaat_ex_1(){
  vm_programm prg = {
    .programm_lenght = 9,
    .inst_array = {
      "PUT(69)",
      "POP()",
      "ADC(0,1)",
      "PRT()",
      "CMP(0,420)",
      "JEQ(8)",
      "NXT()",
      "JMP(1)",
      "HLT()"
    }
  };
  jaat_start();
  jaat_load_programm(&prg);
  jaat_loop();
  jaat_free();
}

void jaat_ex_2(){
  // fibonacci sequence
  vm_programm prg2 = {
    .programm_lenght = 9,
    .inst_array = {
      "PUT(1)",
      "PUT(1)",
      "ADC#(0,1)",
      "SWP(0,1)",
      "PRT(1)", 
      "CMP(1, 46368)",
      "JEQ(8)",
      "JMP(2)",
      "HLT()"
    }
  };
  jaat_start();
  jaat_load_programm(&prg2);
  jaat_loop();
  jaat_free();
}

void jaat_ex_3(){
// fibonacci sequence but with a for loop
  vm_programm prg3 = {
    .programm_lenght = 10,
    .inst_array = {
      "PUT(1)",
      "PUT(1)",
      "PUT(0)",
      "ADC#(0,1)",
      "SWP(0,1)",
      "INC(2)",
      "CMP(2, 17)",
      "JNZ(3)",
      "PRT(1)",
      "HLT()"      
    }
  };
  jaat_start();
  jaat_load_programm(&prg3);
  jaat_loop();
  jaat_free();
}

void jaat_ex_4(){
  // print a string with a variation of PRT
  vm_programm prg4 = {
    .programm_lenght = 7,
    .inst_array = {
      "PRT('Hello World')",
      "PRT('I'm Alive!!')",
      "PRT('This is JAAT vm, usefull for implement a JIT compiler by only implementing a parser!')",
      "PRT('There is a variation with the '' inside a function that can allow the user to insert a string')",
      "PRT('There is a variation of normal instruction that looks like 'NAME# ..', those are purely address instruction')",
      "PRT('So arg_0 and arg_1 are actually pointer for the memory, always')",
      "HLT()"
    }
  };
  jaat_start();
  jaat_load_programm(&prg4);
  jaat_loop();
  jaat_free();
}
int main(int argc, char** argv){
  if(argc > 1){
    for(int i=0; i<argc;i++){
      if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)){
        help_message();
      }
      if(strcmp(argv[i], "-ex") == 0 && argc > i+1){
        if(strcmp(argv[i+1], "1") == 0) {
          jaat_ex_1();
        }else if(strcmp(argv[i+1], "2") == 0){
          jaat_ex_2();
        }else if(strcmp(argv[i+1], "3") == 0){
          jaat_ex_3();
        }else if(strcmp(argv[i+1], "4") == 0){
          jaat_ex_4();
        }else fprintf(stderr,"ERROR: missing argument");
      }
    }
  }
  return 0;
}


