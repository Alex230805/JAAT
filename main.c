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
  array_push(help, "PRT('============================================\n\n - btw the helper is written and executed in the vm :D\n')");
  array_push(help, "HLT");

  jaat_start(help->size);
  jaat_load_programm(help);
  jaat_loop();
  jaat_free();
  array_free(help);
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

  jaat_start(prg->size);
  jaat_load_programm(prg);
  jaat_loop();
  jaat_free();
  array_free(prg);
}

void jaat_ex_2(){
  // fibonacci sequence
  Array* prg2;
  array_new(prg2);
  array_push(prg2, "PRT('fibonacci sequence\n')");
  array_push(prg2, "PUT(1)");
  array_push(prg2, "PUT(1)");
  array_push(prg2, "ADC#(0,1)");
  array_push(prg2, "SWP(0,1)");
  array_push(prg2, "PRT(1)");
  array_push(prg2, "CMP(1, 46368)");
  array_push(prg2, "JEQ(9)");
  array_push(prg2, "JMP(3)");
  array_push(prg2, "HLT()");

  jaat_start(prg2->size);
  jaat_load_programm(prg2);
  jaat_loop();
  jaat_free();
  array_free(prg2);
}

void jaat_ex_3(){
// fibonacci sequence but with a for loop
  Array* prg3;
  array_new(prg3);
  array_push(prg3, "PRT('fibonacci sequence, but with for loop locked on 17\n')");
  array_push(prg3, "PUT(1)");
  array_push(prg3, "PUT(1)");
  array_push(prg3, "PUT(0)");
  array_push(prg3, "ADC#(0,1)");
  array_push(prg3, "SWP(0,1)");
  array_push(prg3, "INC(2)");
  array_push(prg3, "CMP(2, 17)");
  array_push(prg3, "JNZ(3)");
  array_push(prg3, "PRT(1)");
  array_push(prg3, "HLT()");

  jaat_start(prg3->size);
  jaat_load_programm(prg3);
  jaat_loop();
  jaat_free();
  array_free(prg3);
}

void jaat_ex_4(){
  Array*prg4;
  array_new(prg4);
  array_push(prg4, "PRT('Input test, try to write something: ')");
  array_push(prg4, "SCN()");
  array_push(prg4, "PRT('You write: ')");
  array_push(prg4, "PRT(INPUT)");
  array_push(prg4, "HLT()");

  jaat_start(prg4->size);
  jaat_load_programm(prg4);
  jaat_loop();
  jaat_free();
  array_free(prg4);
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


