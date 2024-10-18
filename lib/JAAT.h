#ifndef JAAT_H
#define JAAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>

#define TYPE char*

#include "./Array.h"


#define u8t uint8_t
#define u16t uint16_t
#define u32t uint32_t
#define u64t uint64_t

#define STACK_TYPES u16t
#define STACK_LENGHT 1024
#define BYTE_LENGHT u16t
#define WORD_LEN 512
#define DEBUG false
#define MACHINE_STATE false
#define NO_IMPLEMENTATION() fprintf(stderr,"Feature under development\n"); return;
#define STR_LEN 512

// instruction enum for parsing and instruction check

typedef enum{
  HLT = 0,
  PUT,
  POP,
  ADC,
  ADC_ADR,
  SBC,
  SBC_ADR,
  JMP,
  JNZ,
  JPO,
  CMP,
  JEQ,
  NXT,
  PRV,
  SWP,
  PRT,
  PRT_STRING,
  PRT_COSTANT,
  INC,
  DEC,
  SCN,
}vm_inst;

typedef enum{
  INPUT = 1 
}vm_constant_type;

// JAAT vm

typedef struct{
  int programm_counter;
  int current_pointer;
  STACK_TYPES stack[STACK_LENGHT];
  vm_inst current_instruction;
  STACK_TYPES accumulator;
  char*string_ptr;
  int buffer_tracker;
  int input_tracker_read;
int input_tracker_write;
  vm_constant_type constant_type;
  int arg_0;
  int arg_1;
  bool negative;
  bool overflow;
  bool zero;
  bool halt;
}vm;

typedef struct{
  int programm_lenght;
  char** inst_array;
}vm_programm;

// main functions

void jaat_loop(void);
void jaat_load(vm_inst instruction, int arg_0,int arg_1);
void jaat_exec(void);
void jaat_free(void);
void jaat_start(int size);
void jaat_load_programm(Array *new_prg);
void parse_instruction(void);

void jaat_hlt(void);
void jaat_put(int arg_0);
void jaat_pop(void);
void jaat_adc(int arg_0, int arg_1, bool address_op);
void jaat_sbc(int arg_0, int arg_1, bool address_op);
void jaat_jmp(int arg_0);
void jaat_jnz(int arg_0);
void jaat_jpo(int arg_0);
void jaat_cmp(int arg_0,int arg_1);
void jaat_jeq(int arg_0);
void jaat_nxt(void);
void jaat_prv(void);
void jaat_swp(int arg_0, int arg_1);
void jaat_prt(int arg_0);
void jaat_prt_string(char*arg_0);
void jaat_prt_costant();
void jaat_inc(int arg_0);
void jaat_dec(int arg_0);
void jaat_scn(void);

// tag definition

#define HLT()                  jaat_hlt();            // halt function: halt the execution
#define PUT(arg_0)             jaat_put(arg_0);        // put function:  put arg_0 data into stack
#define POP()                  jaat_pop();             // pop function:  pop the stack content from stack and load the accumulator
#define ADC(arg_0,arg_1,flag)  jaat_adc(arg_0,arg_1,flag);  // adc function:  sum accumulator content with arg_1 content and put into arg_0 address, if flag is enable then the sum is between arg_0 and arg_1 location content
#define SBC(arg_0,arg_1,flag)  jaat_sbc(arg_0,arg_1,flag);  // sbc function:  sub accumulator content with arg_1 content and put into arg_0 address, if flag is enable then it behave the same as ADC
#define JMP(arg_0)             jaat_jmp(arg_0);        // jmp function:  jump to a specific point in programm
#define JNZ(arg_0)             jaat_jnz(arg_0);        // jnz funciont:  jump if the alu result is not 0
#define JEQ(arg_0)             jaat_jeq(arg_0);        // jeq funciont:  jump if zero is set
#define CMP(arg_0, arg_1)      jaat_cmp(arg_0, arg_1); // cmp function:  compare arg_0 stack location with arg_1
#define JPO(arg_0)             jaat_jpo(arg_0);        // jpo function:  jump if the alu overflow frlag is set
#define PRT(arg_0)             jaat_prt(arg_0);        // prt print function: print content inside the provided location
#define NXT()                  jaat_nxt();             // nxt function: increment the stack pointer by one
#define PRV()                  jaat_prv();             // prv function: decrement the stack pointer by one 
#define SWP(arg_0,arg_1)       jaat_swp(arg_0,arg_1);  // swp function: swap the content of two element 
#define INC(arg_0)             jaat_inc(arg_0);        // inc function: increment data in position arg_0
#define DEC(arg_0)             jaat_dec(arg_0);        // dec function: decrement data in position arg_0
#define PRT_STRING(arg_0)      jaat_prt_string(arg_0); // prt_string:   string variation of prt
#define SCN()                  jaat_scn();             // scn function: scan for keyboard input
#define PRT_COSTANT()          jaat_prt_costant();

int input_buffer_size = 255;
int pool_size = 1024;
vm JAAT = {0};


// programm file 

BYTE_LENGHT* instruction_pool;
char** string_buffer;
vm_constant_type* constant_type_buffer;
char* input_buffer;

vm_programm* prg;

#ifdef JAAT_IMPLEMENTATION

#include "JAAT.h"


void parse_instruction(){
  if(DEBUG) printf("parse_instruction()\n");
  vm_inst type;
  int pool_index = 0;
  int arg_0, arg_1;
  char inst[3]; 
  bool string;
  bool is_constant = false;
  int buffer_index = 0;
  bool halt_exist = false;
  for(int i=0;i< prg->programm_lenght;i++){
    string = false;
    is_constant = false;
    arg_0 = 0;
    arg_1 = 0;
    memcpy(inst,prg->inst_array[i], sizeof(char)*3);

    // check for instruction alignment
    if(strcmp(inst,"HLT") == 0){
      type = HLT;
      halt_exist = true;
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
    }else if(strcmp(inst, "JNZ") == 0){
      type = JNZ;
    }else if(strcmp(inst, "CMP") == 0){
      type = CMP;
    }else if(strcmp(inst, "JEQ") == 0){
      type = JEQ;
    }else if(strcmp(inst, "NXT") == 0){
      type = NXT;
    }else if(strcmp(inst, "PRV") == 0){
      type = PRV;
    }else if(strcmp(inst, "SWP") == 0){
      type = SWP;
    }else if(strcmp(inst, "INC") == 0){
      type = INC;
    }else if(strcmp(inst, "DEC") == 0){
      type = DEC;
    }else if(strcmp(inst, "SCN") == 0){
      type = SCN;
    }else{
      fprintf(stderr, "ERROR: no such instruction to parse\n");
      exit(3);
    }
    
    int start_point;
    bool end = false;
    
    // find the starting point for parsing the arguments
    for(start_point = 0; start_point < WORD_LEN && !end ;start_point++){
      if(prg->inst_array[i][start_point] == 40){
        end = true;
      }
    }

    // check for full address variation
    if(prg->inst_array[i][start_point-2] == 35){
      switch(type){
        case ADC:
          type = ADC_ADR;
          break;
        case SBC:
          type = SBC_ADR;
          break;
        default:
          fprintf(stderr, "ERROR: Illegal instruction\n");
          exit(7);
          break;
      } 
    }

    // check for string variation
    if(prg->inst_array[i][start_point] == 34 || prg->inst_array[i][start_point] == 39){
      switch(type) {
        case PRT:
          type = PRT_STRING;
          break;
        default:
          fprintf(stderr, "ERROR: Illegal instruction\n");
          exit(7);
          break;
      }
      string = true;
    }

    // check for constant variation
    if(isupper(prg->inst_array[i][start_point]) > 0){
      switch(type){
        case PRT:
          type = PRT_COSTANT;
          break;
        default:
          fprintf(stderr, "ERROR: Illegal instruction\n");
          exit(7);
          break;
      }
      is_constant = true;
    }
    end = false;
    int ptr=0;
    bool first = false;
    int current_str_len = 0;
    char* str = 0;

    // check for default parameter arg_0,arg_1
    while(!end && ptr < WORD_LEN-start_point && string == false && is_constant == false){
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

    // check for lenght of string
    while(!end && current_str_len < STR_LEN && string == true && is_constant == false){
      if(prg->inst_array[i][start_point+ptr] == 41){
        end = true;
      }else {
        current_str_len += 1;
      }
      ptr += 1;
    }

    // check for lenght of constant
    while(!end && current_str_len < STR_LEN && is_constant == true && string == false){
     if(prg->inst_array[i][start_point+ptr] == 41){
        end = true;
      }else {
        current_str_len += 1;
      }
      ptr += 1; 
    }
    
    // check for string alignment 
    if(string){
      str = (char*)malloc(sizeof(char)*current_str_len-2);
      memcpy(str, &prg->inst_array[i][start_point+1],sizeof(char)*current_str_len-2);
      str[current_str_len-2] = 0;
      string_buffer[buffer_index] = str;
      buffer_index+=1;
    }

    // check for constant alignment 
    if(is_constant){
      char* word = (char*)malloc(sizeof(char)*current_str_len);
      memcpy(word, &prg->inst_array[i][start_point], sizeof(char)*current_str_len);
      word[current_str_len] = 0;
      if(strcmp(word, "INPUT") == 0){
        constant_type_buffer[buffer_index] = INPUT;
        buffer_index += 1;
      }else{
        fprintf(stderr, "No such constant to parse\n");
        exit(8);
      }
      free(word);
      word = 0;
    }
    // put instruction and default parameter into instruction_pool
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

    if(pool_index >= pool_size*3){
      fprintf(stderr, "Too much instruction, pool overflow"); 
      exit(4);
    }
  } 
  if(!halt_exist){
    fprintf(stderr,"ERROR: no halt function found, the programm may fail or may act in a non predetermined way\n");
    exit(6);
  }
}

void jaat_loop(){
  if(DEBUG) printf("jaat_loop()\n");
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
      JAAT.buffer_tracker += 1;
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
  if(DEBUG) printf("jaat_load()\n");
  JAAT.current_instruction = instruction;
  JAAT.arg_0 = arg_0;
  JAAT.arg_1 = arg_1;
  if(string_buffer[JAAT.buffer_tracker] != NULL){
    JAAT.string_ptr = string_buffer[JAAT.buffer_tracker];
  }
  if(constant_type_buffer[JAAT.buffer_tracker] != 0){
    JAAT.constant_type = constant_type_buffer[JAAT.buffer_tracker];  
  }
}

void jaat_start(int size){
  if(DEBUG) printf("jaat_start()\n");
  pool_size = size;
  instruction_pool = (BYTE_LENGHT*)malloc(sizeof(BYTE_LENGHT)*3*pool_size);
  string_buffer = malloc(sizeof(char*)*pool_size);
  constant_type_buffer = malloc(sizeof(vm_constant_type)*pool_size);
  input_buffer = malloc(sizeof(char)*input_buffer_size);
}

void jaat_free(){
  if(DEBUG) printf("jaat_free()\n");
  free(instruction_pool);
  instruction_pool = 0;
  free(string_buffer);
  string_buffer = 0;
  free(input_buffer);
  input_buffer = 0;
  free(constant_type_buffer);
  constant_type_buffer = 0;
  free(prg);
  prg = 0;
}

void jaat_exec(){
  if(DEBUG) printf("jaat_exec()\n");
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
        ADC(JAAT.arg_0,JAAT.arg_1,false);
        break;
    case SBC:
        SBC(JAAT.arg_0,JAAT.arg_1, false);
        break;
    case ADC_ADR:
        ADC(JAAT.arg_0,JAAT.arg_1, true);
        break;
    case SBC_ADR:
        SBC(JAAT.arg_0,JAAT.arg_1, true);
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
        PRT(JAAT.arg_0);
        break;
    case PRT_STRING:
        PRT_STRING(JAAT.string_ptr);
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
    case SWP:
        SWP(JAAT.arg_0, JAAT.arg_1);
        break;
    case PRV:
        PRV();
        break;
    case INC:
        INC(JAAT.arg_0);
        break;
    case DEC:
        DEC(JAAT.arg_0);
        break;
    case SCN:
        SCN();
        break;
    case PRT_COSTANT:
        PRT_COSTANT();
        break;
    default:
      NO_IMPLEMENTATION();
      fwrite("No such instruction please check your code!", 43,1, stderr);
      exit(1);
  }
  return;
}
void jaat_load_programm(Array *new_prg){
  if(DEBUG) printf("jaat_load_programm()\n");
  // create local array
  prg = (vm_programm*)malloc(sizeof(vm_programm));
  prg->inst_array = (char**)malloc(sizeof(char*)* new_prg->nelem+1);
  for(int i=0;i<new_prg->nelem;i++){
    char *st = NULL;
    array_get(new_prg, i, st);
    prg->inst_array[i] = st;
  }
  prg->programm_lenght = new_prg->nelem;

  JAAT.programm_counter = 0;
  JAAT.negative = false;
  JAAT.overflow = false;
  JAAT.zero = false;
  JAAT.halt = false;
  JAAT.accumulator = 0;
  JAAT.current_pointer = 0;
  JAAT.buffer_tracker = 0;
  JAAT.input_tracker_read = 0;
  JAAT.input_tracker_write = 0;
  parse_instruction();
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

void jaat_adc(int arg_0, int arg_1, bool address_op){
  if(DEBUG) printf("ADC\n");
  if(arg_0 < 0 && arg_0 > STACK_LENGHT){
    fprintf(stdout, "WARNING: stack out of bounds for arg_0");
    return;
  }
  if(arg_1 < 0 && arg_1 > STACK_LENGHT && address_op){
    fprintf(stdout, "WARNING: stack out of bounds for arg_1");
    return;
  }

  STACK_TYPES res = 0;
  int cache;
  if(address_op){
    cache = JAAT.stack[arg_0] + JAAT.stack[arg_1];
  }else{
    cache = JAAT.accumulator + arg_1;
  }

  if(cache > sizeof(STACK_TYPES)*255 ) {
    JAAT.overflow = true;
  }
  if(res == 0) JAAT.zero = true;
  res = (STACK_TYPES)cache;
  JAAT.stack[arg_0] = res;
}

void jaat_sbc(int arg_0, int arg_1, bool address_op){
  if(DEBUG) printf("SBC\n");
  if(arg_0 < 0 && arg_0 > STACK_LENGHT){
    fprintf(stdout, "WARNING: stack out of bounds for arg_0");
    return;
  }
  if(arg_1 < 0 && arg_1 > STACK_LENGHT && address_op){
    fprintf(stdout, "WARNING: stack out of bounds for arg_1");
    return;
  }
  STACK_TYPES res = 0;
  int cache;
  if(address_op){
    cache = JAAT.stack[arg_0] - JAAT.stack[arg_1];
  }else{
    cache = JAAT.accumulator - JAAT.stack[arg_0];
  }

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


void jaat_prt(int arg_0){
  if(DEBUG) printf("PRT\n");
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT) fprintf(stdout, "%d\n", JAAT.stack[arg_0]);
  return;
}


void jaat_cmp(int arg_0,int arg_1){
  if(DEBUG) printf("CMP\n");
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT){
    int a = JAAT.stack[arg_0];
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

void jaat_nxt(void){
  if(DEBUG) printf("NXT\n");
  JAAT.current_pointer += 1;
  if(JAAT.current_pointer > STACK_LENGHT){
    printf("WARNING: stack overflow using manual increment\n");
    JAAT.current_pointer = 0;
  }
}

void jaat_prv(void){
  if(DEBUG) printf("PRV\n");
  JAAT.current_pointer -=1;
  if(JAAT.current_pointer < 0){
    printf("WARNING: stack underflow using manual decrement\n");
    JAAT.current_pointer  = STACK_LENGHT;
  }
}

void jaat_swp(int arg_0, int arg_1){
  if(DEBUG) printf("MOV\n");
  if((arg_0 >= 0 && arg_0 < STACK_LENGHT) && (arg_1 >= 0 && arg_1 < STACK_LENGHT)){
    STACK_TYPES data = JAAT.stack[arg_0];
    STACK_TYPES data_2 = JAAT.stack[arg_1];
    JAAT.stack[arg_0] = data_2;
    JAAT.stack[arg_1] = data;
  }
}


void jaat_inc(int arg_0){
  if(DEBUG) printf("INC\n");
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT){
    STACK_TYPES res = 0;
    int cache;
    cache = JAAT.stack[arg_0] + 1;
    if(cache > sizeof(STACK_TYPES)*255 ) {
      JAAT.overflow = true;
    }
    if(res == 0) JAAT.zero = true;
    res = (STACK_TYPES)cache;
    JAAT.stack[arg_0] = res;   
  }
}

void jaat_dec(int arg_0){
  if(DEBUG) printf("DEC\n");
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT){
    STACK_TYPES res = 0;
    int cache;
    cache = JAAT.stack[arg_0] - 1;
    if(cache > sizeof(STACK_TYPES)*255 ) {
      JAAT.overflow = true;
    }
    if(res == 0) JAAT.zero = true;
    res = (STACK_TYPES)cache;
    JAAT.stack[arg_0] = res;   
  }
}

void jaat_prt_string(char*str){
  if(DEBUG) printf("PRT_STRING\n");
  fprintf(stdout, "%s", str);
  free(JAAT.string_ptr);
  JAAT.string_ptr = NULL;
}

void jaat_scn(){
  if(DEBUG) printf("SCN\n");
  char chr;
  scanf("%c", &chr);
  while(chr != 10){
    input_buffer[JAAT.input_tracker_write] = chr;
    JAAT.input_tracker_write += 1;
    if(JAAT.input_tracker_write > input_buffer_size){
      JAAT.input_tracker_write = 0;
    }
    scanf("%c", &chr);
  }
  input_buffer[JAAT.input_tracker_write] = 0;
  return;
}

void jaat_prt_costant(){
  switch(JAAT.constant_type){
    case INPUT:
      while(input_buffer[JAAT.input_tracker_read] != 0 && JAAT.input_tracker_read != JAAT.input_tracker_write){
        printf("%c", input_buffer[JAAT.input_tracker_read]);
        JAAT.input_tracker_read += 1;
        if(JAAT.input_tracker_read > input_buffer_size){
          JAAT.input_tracker_read = 0;
        }
      }
      printf("\n");
      break;
    default:
      NO_IMPLEMENTATION();
      break;
  } 
}

#endif

#endif
