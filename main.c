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
  array_push(help, "PRT('\nBeside -h, you can use the -ex instruction followed by a number to see some example about how the vm work.\n')");
  array_push(help, "PRT('You can type -l to get a list of all possible instruction\n')");
  array_push(help, "PRT('============================================\n\n - btw the helper is written and executed in the vm :D\n')");
  array_push(help, "HLT");

  launch_vm(help);
}


void list_message(){
  Array*list;
  array_new(list);
  array_push(list,"PRT('============================================\n\n')");
  array_push(list, "PRT('LIST OF VALID INSTRUCTION:\n\n')");
  array_push(list, "PRT('HLT: halt the virtual processor, you need this to stop your programm\n')");
  array_push(list, "PRT('PUT: put number passed by argument in stack , use variation with ACC to put accumulator content in stack\n')");
  array_push(list, "PRT('POP: pop content from stack and put it in accumulator\n')");
  array_push(list, "PRT('GET: get thing from stack and put it inside accumulator, it use the argument provided to address the stack\n')");
  array_push(list, "PRT('ADC: add accumulator with content of location provided by the second argument, then put the result in the location provided by the first argument, you can use the variation address by add # before parentesis\n')");
  array_push(list, "PRT('SBC: same as ADC but it perform subtraction\n')");
  array_push(list, "PRT('JMP: jump in a specific location of the programm provided by the first argument\n')");
  array_push(list, "PRT('JNZ: same as jump but only if the result of the previous operation IS NOT ZERO\n')");
  array_push(list, "PRT('JPO: same as jump but only if the result of the previous operation return an OVERFLOW\n')");
  array_push(list, "PRT('JEQ: same as jump but if the result of a comparison return an equal state, aka zero flag is setted\n')");

  array_push(list, "PRT('CMP: compare two element, first argument is used to get info from the stack, the second is used to set the compare number. You can use the # variation to use both element as address \n')");
  array_push(list, "PRT('NXT and PRV: use to increment or decrement MANUALLY the stack pointer\n')");
  array_push(list, "PRT('PRT: print thing on stdout using the first argument that can be a location in the stack, a costant like INPUT to print out the keyboard bufer, or a direct string\n')");
  array_push(list, "PRT('INC: increment the number int the indicated location\n')");
  array_push(list, "PRT('DEC: same as INC but it subtract 1 from the number in stack\n')");
  array_push(list, "PRT('SCN: scan keyboard input and put it in the machine circular buffer\n\n')");
  array_push(list, "PRT('for reference, even this untility is written in the vm language and executed by the vm itself\n\n')");
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
  
  launch_vm(prg2);

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

  launch_vm(prg3);

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
      }if(strcmp(argv[i], "-ex") == 0 && argc > i+1){
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


