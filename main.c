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

#define STACK_TYPES u8t
#define STACK_LENGHT 32
#define BYTE_LENGHT u8t
#define WORD_LEN 16
#define DEBUG true
#define NO_IMPLEMENTATION() fprintf(stderr,"Feature under development\n"); return;

// instruction enum for parsing and instruction check

typedef enum{
  HLT = 0,
  PUT,
  POP,
  ADC,
  SBC,
  JMP,
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
  bool halt;
}vm;

// programm file 

BYTE_LENGHT* instruction_pool;

// programm buffer

int programm_lenght = 6;
char inst_array[][WORD_LEN] = {
  "JMP(0)",
  "PUT(0,0)",
  "ADC(0,34)",
  "POP(0)",
  "PUT(1)",
  "HLT()"
};

int pool_size = 20;
vm JAAT = {0};

void jaat_loop();
void jaat_load(vm_inst instruction, int arg_0,int arg_1);
void jaat_exec();
void parse_instruction();

void parse_instruction(){
  vm_inst type;
  int pool_index = 0;
  int arg_0, arg_1;
  char inst[3]; 
  for(int i=0;i<programm_lenght;i++){
    arg_0 = 0;
    arg_1 = 0;
    memcpy(inst,inst_array[i], sizeof(char)*3);

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
    } else{
      fprintf(stderr, "ERROR: no such instruction to parse\n");
      exit(3);
    }
    
    int start_point;
    bool end = false;
    
    for(start_point = 0; start_point < WORD_LEN && !end ;start_point++){
      if(inst_array[i][start_point] == 40){
        end = true;
      }
    }
    
    end = false;
    int ptr=0;
    bool first = false;

    while(!end && ptr < WORD_LEN-start_point){
      if(inst_array[i][start_point+ptr] == 41){
        end = true;
      }else if(inst_array[i][start_point+ptr] != 32 && inst_array[i][start_point+ptr] != 44){
        if (!first) {
          arg_0 = atoi(&inst_array[i][start_point+ptr]);
          first = true;
        }else{
          arg_1 = atoi(&inst_array[i][start_point+ptr]);
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

      if(JAAT.programm_counter += 3 > sizeof(BYTE_LENGHT)*3*pool_size){
        JAAT.programm_counter = 0;
      }else{
        JAAT.programm_counter += 3;
      }

    } else {
      fprintf(stderr, "Stack overflow\n");
      exit(2);
    }
    
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
    case PUT:
    case POP:
    case ADC:
    case SBC:
    case JMP:
          NO_IMPLEMENTATION();
          break;
    default:
      fwrite("No such instruction please check your code!", 43,1, stderr);
      exit(1);
  }
  return;
}

int main(void){


  instruction_pool = (BYTE_LENGHT*)malloc(sizeof(BYTE_LENGHT)*3*pool_size);
  parse_instruction();

  free(instruction_pool);
  return 0;
}
