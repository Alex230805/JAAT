#ifndef JAAT_H
#define JAAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>


#define u8t uint8_t
#define u16t uint16_t
#define u32t uint32_t
#define u64t uint64_t

#define STACK_TYPES u16t
#define STACK_LENGHT 32
#define BYTE_LENGHT u16t
#define WORD_LEN 16
#define DEBUG false
#define MACHINE_STATE false
#define NO_IMPLEMENTATION() fprintf(stderr,"Feature under development\n"); return;

// instruction enum for parsing and instruction check

typedef enum{
  HLT = 0,
  PUT,
  POP,
  ADC,
  SBC,
  JMP,
  JNZ,
  JPO,
  CMP,
  JEQ,
  NXT,
  PRT
}vm_inst;

// JAAT vm

typedef struct{
  int programm_counter;
  int current_pointer;
  STACK_TYPES stack[STACK_LENGHT];
  vm_inst current_instruction;
  STACK_TYPES accumulator;
  int arg_0;
  int arg_1;
  bool negative;
  bool overflow;
  bool zero;
  bool halt;
}vm;

typedef struct{
  int programm_lenght;
  char inst_array[120][WORD_LEN];
}vm_programm;


// main functions

void jaat_loop();
void jaat_load(vm_inst instruction, int arg_0,int arg_1);
void jaat_exec();
void jaat_load_programm(vm_programm *new_prg);
void parse_instruction();

void jaat_hlt(void);
void jaat_put(int arg_0);
void jaat_pop(void);
void jaat_adc(int arg_0, int arg_1);
void jaat_sbc(int arg_0, int arg_1);
void jaat_jmp(int arg_0);
void jaat_jnz(int arg_0);
void jaat_jpo(int arg_0);
void jaat_cmp(int arg_0,int arg_1);
void jaat_jeq(int arg_0);
void jaat_nxt();
void jaat_prt();

// tag definition

#define HLT()             jaat_hlt();            // halt function: halt the execution
#define PUT(arg_0)        jaat_put(arg_0);        // put function:  put arg_0 data into stack
#define POP()             jaat_pop();             // pop function:  pop the stack content from stack and load the accumulator
#define ADC(arg_0,arg_1)  jaat_adc(arg_0,arg_1);  // adc function:  sum accumulator content with arg_1 content and put into arg_0 address
#define SBC(arg_0,arg_1)  jaat_sbc(arg_0,arg_1);  // sbc function:  sub accumulator content with arg_1 content and put into arg_0 address
#define JMP(arg_0)        jaat_jmp(arg_0);        // jmp function:  jump to a specific point in programm
#define JNZ(arg_0)        jaat_jnz(arg_0);        // jnz funciont:  jump if the alu result is not 0
#define JEQ(arg_0)        jaat_jeq(arg_0);        // jeq funciont:  jump if zero is set
#define CMP(arg_0, arg_1) jaat_cmp(arg_0, arg_1); // cmp function:  compare stack location with arg_1
#define JPO(arg_0)        jaat_jpo(arg_0);        // jpo function:  jump if the alu overflow frlag is set
#define PRT()             jaat_prt();             // prt print function: print content inside the current stack pointer
#define NXT()             jaat_nxt();             // nxt function: increment the stack pointer by one

int pool_size = 1024;
vm JAAT = {0};

// programm file 

BYTE_LENGHT* instruction_pool;

vm_programm* prg;

#ifdef JAAT_IMPLEMENTATION

#include "JAAT.h"


void parse_instruction(){
  vm_inst type;
  int pool_index = 0;
  int arg_0, arg_1;
  char inst[3]; 
  for(int i=0;i< prg->programm_lenght;i++){
    arg_0 = 0;
    arg_1 = 0;
    memcpy(inst,prg->inst_array[i], sizeof(char)*3);

    if(strcmp(inst,"HLT") == 0){
      type = HLT;
    }else if(strcmp(inst,"PUT") == 0){
      type = PUT;
    }else if(strcmp(inst,"POP") == 0){
      type = POP;
    }else if(strcmp(inst,"ADC") == 0){
      type = ADC;
    }else if(strcmp(inst, "SBC") == 0){
      type = SBC;
    }else if(strcmp(inst,"JMP") == 0){
      type = JMP;
    }else if(strcmp(inst, "PRT") == 0){
      type = PRT;
    }else if(strcmp(inst, "CMP") == 0){
      type = CMP;
    }else if(strcmp(inst, "JEQ") == 0){
      type = JEQ;
    }else if(strcmp(inst, "NXT") == 0){
      type = NXT;
    }else{
      fprintf(stderr, "ERROR: no such instruction to parse\n");
      exit(3);
    }
    
    int start_point;
    bool end = false;
    
    for(start_point = 0; start_point < WORD_LEN && !end ;start_point++){
      if(prg->inst_array[i][start_point] == 40){
        end = true;
      }
    }
    
    end = false;
    int ptr=0;
    bool first = false;

    while(!end && ptr < WORD_LEN-start_point){
      if(prg->inst_array[i][start_point+ptr] == 41){
        end = true;
      }else if(prg->inst_array[i][start_point+ptr] != 32 && prg->inst_array[i][start_point+ptr] != 44){
        if (!first) {
          arg_0 = atoi(&prg->inst_array[i][start_point+ptr]);
          first = true;
        }else{
          arg_1 = atoi(&prg->inst_array[i][start_point+ptr]);
          end = true;
        }
      }
      ptr+=1;
    }

    instruction_pool[pool_index] = type;
    instruction_pool[pool_index+1] = arg_0;
    instruction_pool[pool_index+2] = arg_1;

    // debug flag

    if(DEBUG){
      printf("instruction type: %d\n", instruction_pool[pool_index]);
      printf("arg_0: %d\n", instruction_pool[pool_index+1]);
      printf("arg_1: %d\n\n", instruction_pool[pool_index+2]);
    }
    
    pool_index += 3;

    if(pool_index >= pool_size){
      fprintf(stderr, "Too much instruction, pool overflow"); 
      exit(4);
    }
  } 
}

void jaat_loop(){
  if(JAAT.programm_counter > pool_size){
    fprintf(stderr, "Programm counter not initialized");
    return;
  }
  while(JAAT.halt == false){
    if(JAAT.current_pointer <= STACK_LENGHT){
      
      BYTE_LENGHT inst = instruction_pool[JAAT.programm_counter];
      int arg_0 = (int) instruction_pool[JAAT.programm_counter+1]; 
      int arg_1 = (int) instruction_pool[JAAT.programm_counter+2];
      jaat_load(inst,arg_0, arg_1);
      jaat_exec();
      JAAT.programm_counter += 3;
      if(JAAT.programm_counter > prg->programm_lenght*3){
        JAAT.programm_counter = 0;
      }

    } else {
      fprintf(stderr, "Stack overflow\n");
      exit(2);
    }
  }
  if(MACHINE_STATE){
    printf("Machine state:\n");
    printf("Zero flag: %d\n", JAAT.zero);
    printf("Zero overflow: %d\n", JAAT.overflow);
    printf("Zero negative: %d\n", JAAT.negative);
  }
}

void jaat_load(vm_inst instruction, int arg_0,int arg_1){
  JAAT.current_instruction = instruction;
  JAAT.arg_0 = arg_0;
  JAAT.arg_1 = arg_1;
}



void jaat_exec(){
  switch(JAAT.current_instruction){
    case HLT:
        HLT();   
        break;
    case PUT:
        PUT(JAAT.arg_0);
        break;
    case POP:
        POP();
        break;
    case ADC:
        ADC(JAAT.arg_0,JAAT.arg_1);
        break;
    case SBC:
        SBC(JAAT.arg_0,JAAT.arg_1);
        break;
    case JMP:
        JMP(JAAT.arg_0);
        break;
    case JNZ:
        JNZ(JAAT.arg_0);
        break;
    case JPO:
        JPO(JAAT.arg_0);
        break;
    case PRT:
        PRT();
        break;
    case JEQ:
        JEQ(JAAT.arg_0);
        break;
    case CMP:
        CMP(JAAT.arg_0,JAAT.arg_1);
        break;
    case NXT:
        NXT();
        break;
    default:
      NO_IMPLEMENTATION();
      fwrite("No such instruction please check your code!", 43,1, stderr);
      exit(1);
  }
  return;
}
void jaat_load_programm(vm_programm *new_prg){
  prg = new_prg;
}

void jaat_hlt(void){
  if(DEBUG) printf("HLT\n");
  JAAT.halt = true;
}

void jaat_put(int arg_0){
  if(DEBUG) printf("PUT\n");
  JAAT.stack[JAAT.current_pointer] = arg_0;
  JAAT.current_pointer += 1;

  if(JAAT.current_pointer > STACK_LENGHT){
    fprintf(stdout, "WARNING: stack overflow reached, but execution is not halted\n");
    JAAT.current_pointer += 1;
  }
}

void jaat_pop(void){ 
  if(DEBUG) printf("POP\n");
  JAAT.current_pointer -= 1;
  JAAT.accumulator = JAAT.stack[JAAT.current_pointer];
  if(JAAT.current_pointer < 0){
    fprintf(stdout, "WARNING: stack underflow reached, but execution is not halted\n");
    JAAT.current_pointer = 0;
  } 
}

void jaat_adc(int arg_0, int arg_1){
  if(DEBUG) printf("ADC\n");
  STACK_TYPES res = 0;
  int cache;
  cache = JAAT.accumulator + arg_1;
  if(cache > sizeof(STACK_TYPES)*255 ) {
    JAAT.overflow = true;
  }
  if(res == 0) JAAT.zero = true;
  res = (STACK_TYPES)cache;
  JAAT.stack[arg_0] = res;
}

void jaat_sbc(int arg_0, int arg_1){
  if(DEBUG) printf("SBC\n");
  STACK_TYPES res = 0;
  int cache;
  cache = JAAT.stack[arg_0] - JAAT.accumulator;
  if(sizeof(cache) > sizeof(STACK_TYPES)){
    JAAT.overflow = true;
    res = (STACK_TYPES)cache;
  }
  if(res == 0) JAAT.zero = true;
  JAAT.stack[arg_0] = res;
}

void jaat_jmp(int arg_0){
  if(DEBUG) printf("JMP\n");
  if(arg_0 > prg->programm_lenght || arg_0 < 0){
    JAAT.halt = true;
    fprintf(stderr, "Programm counter is try to read out of programm lenght");
    return;
  }
  JAAT.programm_counter = (arg_0*3) - 3;
}

void jaat_jnz(int arg_0){
  if(DEBUG) printf("JNZ\n");
  if(JAAT.zero == false){
    if(arg_0 > prg->programm_lenght || arg_0 < 0){
      JAAT.halt = true;
      fprintf(stderr, "Programm counter is try to read out of programm lenght");
    }
    JAAT.programm_counter = (arg_0*3) - 3;
  }
    return;
}


void jaat_jpo(int arg_0){
  if(DEBUG) printf("JPO\n");
  if(JAAT.overflow == true){
    if(arg_0 > prg->programm_lenght || arg_0 < 0){
      JAAT.halt = true;
      fprintf(stderr, "Programm counter is try to read out of programm lenght");
    }
    JAAT.programm_counter = (arg_0*3) - 3;
  }
  return;
}


void jaat_prt(){
  if(DEBUG) printf("PRT\n");
  fprintf(stdout, "%d\n", JAAT.stack[JAAT.current_pointer]);
  return;
}


void jaat_cmp(int arg_0,int arg_1){
  if(DEBUG) printf("CMP\n");
  int a = JAAT.stack[JAAT.current_pointer];
  a -= arg_1;
  if(a == 0){
    JAAT.zero = true;
  }else{
    JAAT.zero = false;
  }
  if(a < 0){
    JAAT.negative = true;
  }else{
    JAAT.negative = false;
  }
}

void jaat_jeq(int arg_0){
  if(DEBUG) printf("JEQ\n");
  if(JAAT.zero == true){
    if(arg_0 > prg->programm_lenght || arg_0 < 0){
      JAAT.halt = true;
      fprintf(stderr, "Programm counter is try to read out of programm lenght");
    }
    JAAT.programm_counter = (arg_0*3)-3;
  }
}

void jaat_nxt(){
  if(DEBUG) printf("NXT\n");
  JAAT.current_pointer += 1;
  if(JAAT.current_pointer > STACK_LENGHT){
    printf("WARNING: stack overflow using manual increment\n");
    JAAT.current_pointer = 0;
  }
}

#endif

#endif
