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

#define launch_vm(prg) \
  jaat_start(prg->nelem+1);\
  jaat_load_programm(prg);\
  jaat_loop();\
  jaat_free();\
  array_free(prg);

#define u8t uint8_t
#define u16t uint16_t
#define u32t uint32_t
#define u64t uint64_t

#define STACK_TYPES u16t
#define STACK_LENGHT 1024
#define BYTE_LENGHT u16t
#define WORD_LEN 512
#define DEBUG false 
#define NAME_SPACE_LENGHT 256
#define MACHINE_STATE false
#define NO_IMPLEMENTATION() fprintf(stderr,"Feature under development\n"); return;

#define ILLEGAL_INST(line) fprintf(stderr, "[PARSER]: ERROR: Illegal instruction on line %d", line); exit(7);
#define STR_LEN 512

#define STACK_OVER() fprintf(stdout, "[SYSTEM]: WARNING: stack overflow reached, but execution is not halted\n"); 

#define STACK_UNDER() fprintf(stdout, "[SYSTEM]: WARNING: stack underflow reached, but execution is not halted\n");

#define OUT_OF_PROGRAMM() fprintf(stderr, "[SYSTEM]: Programm counter is try to read out of programm lenght");  


/*
 *  STEP TO ADD AN INSTRUCTION
 *
 *  1 - declare it in the vm_instruction list
 *  2 - decide the type (a normal one, a string or a constant one)
 *  3 - update the parser in the instruction identification part
 *  4 - update the parser in the selected parser section ( for string or constant)
 *  5 - create the function related to the instruction
 *  6 - define the instruction macro
 *  7 - use the macro in the jaat_loo() in a case inside the switch case
 *  
 * */



// instruction enum for parsing and instruction check

typedef enum{
  HLT = 0,
  PUT,
  PUT_CONSTANT,
  POP,
  GET,
  ADC,
  ADC_ADR,
  SBC,
  SBC_ADR,
  JMP,
  JNZ,
  JPO,
  JSR,
  JEQ,
  RTS,
  CMP,
  CMP_ADR,
  NXT,
  PRV,
  SWP,
  PRT,
  PRT_STRING,
  PRT_CONSTANT,
  INC,
  DEC,
  SCN,
}vm_inst;

typedef enum{
  INPUT = 1,
  ACC
}vm_constant_type;


typedef struct{
  int index;
  char* name_space;
}Box;

// JAAT vm

typedef struct{
  int programm_counter;
  int current_pointer;

  STACK_TYPES stack[STACK_LENGHT];
  vm_inst current_instruction;
  STACK_TYPES accumulator;

  char*string_ptr;
  int const_tracker;
  int string_tracker;
  
  int input_tracker_read;
  int input_tracker_write;
  
  int stack_address_pointer;
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
int parser_check_for_namespace(vm_inst inst, char* line);
void parse_preprocessor();

void jaat_hlt(void);
void jaat_put(int arg_0);
void jaat_put_constant(void);
void jaat_pop(void);
void jaat_get(int arg_0);
void jaat_adc(int arg_0, int arg_1, bool address_op);
void jaat_sbc(int arg_0, int arg_1, bool address_op);
void jaat_jmp(int arg_0);
void jaat_jnz(int arg_0);
void jaat_jpo(int arg_0);
void jaat_jsr(int arg_0);
void jaat_rts();

void jaat_cmp(int arg_0,int arg_1, bool address_op);
void jaat_jeq(int arg_0);
void jaat_nxt(void);
void jaat_prv(void);
void jaat_swp(int arg_0, int arg_1);
void jaat_prt(int arg_0);
void jaat_prt_string(char*arg_0);
void jaat_prt_constant(void);
void jaat_inc(int arg_0);
void jaat_dec(int arg_0);
void jaat_scn(void);

// tag definition

#define HLT()                  jaat_hlt();             // halt function: halt the execution
#define PUT_CONSTANT()         jaat_put_constant();    // put function with constant: basically put but with special things 
#define PUT(arg_0)             jaat_put(arg_0);        // put function:  put arg_0 data into stack
#define POP()                  jaat_pop();             // pop function:  pop the stack content from stack and load the accumulator
#define GET(arg_0)             jaat_get(arg_0);        // get function:  get item from stack and put it into accumulator
#define ADC(arg_0,arg_1,flag)  jaat_adc(arg_0,arg_1,flag);  // adc function:  sum accumulator content with arg_1 content and put into arg_0 address, if flag is enable then the sum is between arg_0 and arg_1 location content
#define SBC(arg_0,arg_1,flag)  jaat_sbc(arg_0,arg_1,flag);  // sbc function:  sub accumulator content with arg_1 content and put into arg_0 address, if flag is enable then it behave the same as ADC
#define JMP(arg_0)             jaat_jmp(arg_0);        // jmp function:  jump to a specific point in programm
#define JNZ(arg_0)             jaat_jnz(arg_0);        // jnz funciont:  jump if the alu result is not 0
#define JEQ(arg_0)             jaat_jeq(arg_0);        // jeq funciont:  jump if zero is set
#define JSR(arg_0)             jaat_jsr(arg_0);        // jsr function: jump into subroutine
#define RTS()                  jaat_rts();             // rts function: return from subroutine

#define CMP(arg_0, arg_1, flag)      jaat_cmp(arg_0, arg_1, flag); // cmp function:  compare arg_0 stack location with arg_1
#define JPO(arg_0)             jaat_jpo(arg_0);        // jpo function:  jump if the alu overflow frlag is set
#define PRT(arg_0)             jaat_prt(arg_0);        // prt print function: print content inside the provided location
#define NXT()                  jaat_nxt();             // nxt function: increment the stack pointer by one
#define PRV()                  jaat_prv();             // prv function: decrement the stack pointer by one 
#define SWP(arg_0,arg_1)       jaat_swp(arg_0,arg_1);  // swp function: swap the content of two element 
#define INC(arg_0)             jaat_inc(arg_0);        // inc function: increment data in position arg_0
#define DEC(arg_0)             jaat_dec(arg_0);        // dec function: decrement data in position arg_0
#define PRT_STRING(arg_0)      jaat_prt_string(arg_0); // prt_string:   string variation of prt
#define SCN()                  jaat_scn();             // scn function: scan for keyboard input
#define PRT_CONSTANT()          jaat_prt_constant();

int input_buffer_size = 255;
int pool_size = 1024;
vm JAAT = {0};
int fn_name_space_ptr = 0;

// programm file 

BYTE_LENGHT* instruction_pool;
char** string_buffer;
vm_constant_type* constant_type_buffer;
char* input_buffer;
Box *fn_name_space;

vm_programm* prg;

#ifdef JAAT_IMPLEMENTATION

#include "JAAT.h"

void parse_preprocessor(){
  int fn_index = 0;
  if(DEBUG) printf("[PARSER PREPROCESSOR]: Starting preprocessing, checking for name space definition\n");
  for(int i=0;i<prg->programm_lenght;i++){
    if(prg->inst_array[i][0] =='!'){
      if(DEBUG) printf("[PARSER PREPROCESSOR]: name space parser triggered\n");
      // save name_space
      fn_index = i-fn_name_space_ptr;
      fn_name_space[fn_name_space_ptr] = (Box){fn_index, prg->inst_array[i]};
      if(DEBUG) printf("[PARSER PREPROCESSOR]: name space found at %d: %s\n", i,fn_name_space[fn_name_space_ptr].name_space);
      fn_name_space_ptr+=1;
      if(fn_name_space_ptr > NAME_SPACE_LENGHT){
        fn_name_space_ptr = 0;
      }
    }
  }
}

void parse_instruction(){
  if(DEBUG) printf("[PARSER]: start instruction parser\n");
  vm_inst type = -1;
  
  int pool_index = 0;
  int arg_0, arg_1;
  char inst[3]; 
  int string_index = 0;
  int const_index = 0;


  bool is_constant = false;
  bool string;
  bool halt_exist = false;
  bool name_space_found = false;
  bool skip = false;
  bool fun_reference = false;

  for(int i=0;i< prg->programm_lenght;i++){
    fun_reference = false;
    skip = false;
    name_space_found = false;
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
    }else if(strcmp(inst,"GET") == 0){
      type = GET;
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
    }else if(strcmp(inst, "JSR") == 0){
      type = JSR;
    }else if(strcmp(inst, "RTS") == 0){
      type = RTS;
    }else if(inst[0] == '!'){
      skip = true;
    }else{
      fprintf(stderr, "[PARSER]: ERROR: no such instruction to parse, failed on line %d\n", i+1);
      exit(3);
    }
    
    int start_point;
    bool end = false;
    
    // find the starting point for parsing the arguments
    for(start_point = 0; start_point < WORD_LEN && !end && !name_space_found && !skip && !fun_reference ;start_point++){
      if(prg->inst_array[i][start_point] == 40){
        end = true;
      }
    }

    // check for full address variation
    if(prg->inst_array[i][start_point-2] == 35  && !name_space_found && !skip){
      if(DEBUG) printf("[PARSER]: Parsing address function variation\n");
      switch(type){
        case ADC:
          type = ADC_ADR;
          break;
        case SBC:
          type = SBC_ADR;
          break;
        case CMP:
          type = CMP_ADR;
          break;
        default:
          ILLEGAL_INST(i+1);
          break;
      } 
    }

    // check for string variation
    if((prg->inst_array[i][start_point] == 34 || prg->inst_array[i][start_point] == 39) && !name_space_found && !skip){
      if(DEBUG) printf("[PARSER]: Parsing string literal variation\n");
      switch(type) {
        case PRT:
          type = PRT_STRING;
          break;
        default:
          ILLEGAL_INST(i+1);
          break;
      }
      string = true;
    }

    // check for constant variation and name space reference
    if((isupper(prg->inst_array[i][start_point]) > 0) &&  !name_space_found && !skip && !fun_reference){
      if(DEBUG) printf("[PARSER]: Parsing constant variation\n");
      switch(type){
        case PRT:
          is_constant = true;
          type = PRT_CONSTANT;
          break;
        case PUT:
          is_constant = true;
          type = PUT_CONSTANT;
          break;
        default:
          arg_0 = parser_check_for_namespace(type, prg->inst_array[i]);
          if(arg_0 == -1){
            ILLEGAL_INST(i+1);
          }
          fun_reference = true;
          break;
      }
    }

    end = false;
    int ptr=0;
    bool first = false;
    int current_str_len = 0;
    char* str = 0;

    // check for default parameter arg_0,arg_1
    while(!end && ptr < WORD_LEN-start_point && string == false && is_constant == false && !name_space_found && !skip && !fun_reference){
      if(prg->inst_array[i][start_point+ptr] == 41 && prg->inst_array[i][start_point+ptr+1] == 0){
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
    end = false;
    while(!end && current_str_len < STR_LEN && string == true && is_constant == false && !name_space_found && !skip && !fun_reference ){
      if(prg->inst_array[i][start_point+ptr] == 41 && prg->inst_array[i][start_point+ptr+1] == 0){
        end = true;
      }else {
        current_str_len += 1;
      }
      ptr += 1;
    }

    // check for lenght of constant
    end = false;
    while(!end && current_str_len < STR_LEN && is_constant == true && string == false && !name_space_found && !skip && !fun_reference ){
     if(prg->inst_array[i][start_point+ptr] == 41 && prg->inst_array[i][start_point+ptr+1] == 0){
        end = true;
      }else {
        current_str_len += 1;
      }
      ptr += 1; 
    }
    
    // check for string alignment 
    if(string && !name_space_found && !skip && !fun_reference ){
      str = (char*)malloc(sizeof(char)*current_str_len-2);
      memcpy(str, &prg->inst_array[i][start_point+1],sizeof(char)*current_str_len-2);
      str[current_str_len-2] = 0;
      string_buffer[string_index] = str;
      string_index+=1;
    }

    // check for constant alignment 
    if(is_constant && !name_space_found && !skip && !fun_reference){
      char* word = (char*)malloc(sizeof(char)*current_str_len);
      memcpy(word, &prg->inst_array[i][start_point], sizeof(char)*current_str_len);
      word[current_str_len] = 0;
      if(strcmp(word, "INPUT") == 0){
        constant_type_buffer[const_index] = INPUT;
        const_index += 1;
      }else{
        ILLEGAL_INST(i+1); 
        exit(8);
      }
      free(word);
      word = 0;
    }
    // put instruction and default parameter into instruction_pool
    if(!skip){
      instruction_pool[pool_index] = type;
      instruction_pool[pool_index+1] = arg_0;
      instruction_pool[pool_index+2] = arg_1;
      if(DEBUG) printf("[SYSTEM PROGRAMM]: instruction: %s : (%d,%d)\n", inst,arg_0,arg_1);
      pool_index += 3;
    }

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

int parser_check_for_namespace(vm_inst inst, char* line){
  // parse name spaces
  char local_name_space[NAME_SPACE_LENGHT];
  int length = 0;
  int arg_0 = -1;
  int i=0;
  if(DEBUG) printf("[PARSER]: checks for name space references for [%d] instruction\n", inst);
  switch (inst) {
    case JMP:
    case JNZ:
    case JPO:
    case JSR:
    case JEQ:
      length = strlen(line);
      for(i=0;i<length-3;i++){
        local_name_space[i] = line[i+4];
      }
      local_name_space[i-2] = 0;
      for(i=0;i<fn_name_space_ptr;i++){
        if(strcmp(local_name_space, &fn_name_space[i].name_space[1]) == 0){
          if(DEBUG) printf("[PARSER]: Namespace %s reference function in line %d\n", fn_name_space[i].name_space, fn_name_space[i].index);
          arg_0 = fn_name_space[i].index;
        }
      }
      break; 
    default:
      if(DEBUG) printf("[PARSER]: unable to fund name space reference \n");
      break;
  }
  return arg_0;
}


void jaat_loop(){
  char input;
  if(DEBUG) printf("[LOOP]: starting loop\n");
  if(JAAT.programm_counter > pool_size){
    fprintf(stderr, "Programm counter not initialized");
    return;
  }
  while(JAAT.halt == false){
    if(JAAT.current_pointer <= STACK_LENGHT){
      
      BYTE_LENGHT inst = instruction_pool[JAAT.programm_counter];
      if(DEBUG){
        printf("[LOOP]: current programm counter: %d\n",JAAT.programm_counter);        
        printf("[LOOP]: current instruction loaded: ");
        switch(instruction_pool[JAAT.programm_counter]){
          case HLT:
              printf("HLT\n");
              break;
          case PUT:
              printf("PUT\n");
              break; 
          case PUT_CONSTANT:
              printf("PUT_CONSTANT\n");
              break; 
          case POP:
              printf("POP\n");
              break; 
          case GET:
              printf("GET\n");
              break; 
          case ADC:
              printf("ADC\n");
              break; 
          case ADC_ADR:
              printf("ADC_ADR\n");
              break; 
          case SBC:
              printf("SBC\n");
              break; 
          case SBC_ADR:
              printf("SBC_ADR\n");
              break; 
          case JMP:
              printf("JMP\n");
              break; 
          case JNZ:
              printf("JNZ\n");
              break; 
          case JPO:
              printf("JPO\n");
              break; 
          case JSR:
              printf("JSR\n");
              break; 
          case JEQ:
              printf("JEQ\n");
              break; 
          case RTS:
              printf("RTS\n");
              break; 
          case CMP:
              printf("CMP\n");
              break; 
          case CMP_ADR:
              printf("CMP_ADR\n");
              break; 
          case NXT:
              printf("NXT\n");
              break; 
          case PRV:
              printf("PRV\n");
              break; 
          case SWP:
              printf("SWP\n");
              break; 
          case PRT:
              printf("PRT\n");
              break; 
          case PRT_STRING:
              printf("PRT_STRING\n");
              break; 
          case PRT_CONSTANT:
              printf("PRT_CONSTANT\n");
              break; 
          case INC:
              printf("INC\n");
              break; 
          case DEC:
              printf("DEC\n");
              break; 
          case SCN:
              printf("SCN\n");
              break;  
        }
      } 
      int arg_0 = (int) instruction_pool[JAAT.programm_counter+1]; 
      int arg_1 = (int) instruction_pool[JAAT.programm_counter+2];
      jaat_load(inst,arg_0, arg_1);
      jaat_exec();
      if(DEBUG){
        scanf("%c", &input);
      } 
      JAAT.programm_counter += 3;
      if(JAAT.programm_counter > prg->programm_lenght*3){
        JAAT.programm_counter = 0;
      }
      if(DEBUG) printf("\n\n");
    } else {
      fprintf(stderr, "Stack overflow\n");
      exit(2);
    }

  }
  if(MACHINE_STATE){
    printf("[MACHINE STATE]: ");
    printf("Zero flag: [%d], ", JAAT.zero);
    printf("Zero overflow: [%d], ", JAAT.overflow);
    printf("Zero negative: [%d]\n", JAAT.negative);
  } 
}

void jaat_load(vm_inst instruction, int arg_0,int arg_1){
  if(DEBUG) printf("[LOAD]: load single instruction\n");
  JAAT.current_instruction = instruction;
  JAAT.arg_0 = arg_0;
  JAAT.arg_1 = arg_1;
  switch(instruction){
    case PRT_STRING:
      if(string_buffer[JAAT.string_tracker] != NULL){
        JAAT.string_ptr = string_buffer[JAAT.string_tracker];
        JAAT.string_tracker += 1;
      }
      break;

    case PRT_CONSTANT:
    case PUT_CONSTANT:
      if(constant_type_buffer[JAAT.const_tracker] != 0){
        JAAT.constant_type = constant_type_buffer[JAAT.const_tracker];  
        JAAT.const_tracker += 1;
      }
      break;
    default:
      break;
  }
}

void jaat_start(int size){
  if(DEBUG) printf("[STARTER]: startint vm\n");
  pool_size = size;
  instruction_pool = (BYTE_LENGHT*)malloc(sizeof(BYTE_LENGHT)*3*pool_size);
  if(DEBUG) printf("[STARTER]: instruction_pool buffer  allocated\n");
  string_buffer = malloc(sizeof(char*)*pool_size);
  if(DEBUG) printf("[STARTER]: string_buffer allocated\n");
  constant_type_buffer = malloc(sizeof(vm_constant_type)*pool_size);
  if(DEBUG) printf("[STARTER]: costant type buffer allocated\n");
  input_buffer = malloc(sizeof(char)*input_buffer_size);
  if(DEBUG) printf("[STARTER]: input buffer allocated\n");
  fn_name_space = (Box*)malloc(sizeof(Box)*pool_size);
  if(DEBUG) printf("[STARTER]: fn_name_space buffer allocated\n");
}

void jaat_free(){
  if(DEBUG) printf("[CLOSING PROCESS]: free ram location\n");
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
  free(fn_name_space);
  fn_name_space = 0;
}

void jaat_exec(){
  if(DEBUG) printf("[EXECUTOR]: starting execution\n");
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
    case GET:
        GET(JAAT.arg_0);
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
    case JSR:
        JSR(JAAT.arg_0);
        break;
    case RTS:
        jaat_rts();
        break;
    case CMP:
        CMP(JAAT.arg_0,JAAT.arg_1,false);
        break;
    case CMP_ADR:
        CMP(JAAT.arg_0,JAAT.arg_1, true);
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
    case PRT_CONSTANT:
        PRT_CONSTANT();
        break;
    case PUT_CONSTANT:
        PUT_CONSTANT();
        break;
    default:
      NO_IMPLEMENTATION();
      fprintf(stderr, "ERROR: Try to load an illegal instruction\n");
      exit(5);
  }
  return;
}
void jaat_load_programm(Array *new_prg){
  if(DEBUG) printf("[PROGRAMM_LOADER]: start loading programm\n");
  // create local array
  fn_name_space_ptr = 0;
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
  JAAT.string_tracker = 0;
  JAAT.const_tracker = 0;
  JAAT.input_tracker_read = 0;
  JAAT.input_tracker_write = 0;
  parse_preprocessor();
  parse_instruction();
}

void jaat_hlt(void){
  JAAT.halt = true;
}

void jaat_put(int arg_0){
  JAAT.stack[JAAT.current_pointer] = arg_0;
  JAAT.current_pointer += 1;

  if(JAAT.current_pointer > STACK_LENGHT){
    STACK_OVER();
    JAAT.current_pointer += 1;
  }
}

void jaat_pop(void){ 
  JAAT.current_pointer -= 1;
  JAAT.accumulator = JAAT.stack[JAAT.current_pointer];
  if(JAAT.current_pointer < 0){
    STACK_UNDER();
    JAAT.current_pointer = 0;
  } 
}

void jaat_adc(int arg_0, int arg_1, bool address_op){
  if(arg_0 < 0 && arg_0 > STACK_LENGHT){
    STACK_OVER();
    return;
  }
  if(arg_1 < 0 && arg_1 > STACK_LENGHT && address_op){
    STACK_OVER();
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
  if(arg_0 < 0 && arg_0 > STACK_LENGHT){
    STACK_OVER();
    return;
  }
  if(arg_1 < 0 && arg_1 > STACK_LENGHT && address_op){
    STACK_OVER();
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
  if(arg_0 > prg->programm_lenght || arg_0 < 0){
    JAAT.halt = true;
    OUT_OF_PROGRAMM();
    return;
  }
  JAAT.programm_counter = (arg_0*3) - 3;
}

void jaat_jnz(int arg_0){
  if(JAAT.zero == false){
    if(arg_0 > prg->programm_lenght || arg_0 < 0){
      JAAT.halt = true;
      OUT_OF_PROGRAMM();
    }
    JAAT.programm_counter = (arg_0*3) - 3;
  }
    return;
}


void jaat_jpo(int arg_0){
  if(JAAT.overflow == true){
    if(arg_0 > prg->programm_lenght || arg_0 < 0){
      JAAT.halt = true;
      OUT_OF_PROGRAMM();
    }
    JAAT.programm_counter = (arg_0*3) - 3;
  }
  return;
}


void jaat_prt(int arg_0){
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT) {
    if(DEBUG) printf("\nstdout -> ");
    fprintf(stdout, "%d\n", JAAT.stack[arg_0]);
    if(DEBUG) printf("\n");
  }
  return;
}


void jaat_cmp(int arg_0,int arg_1, bool flag){
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT){
    int a = 0;
    int b = 0;
    if(flag){
      a = JAAT.stack[arg_0];
      b = JAAT.stack[arg_1];
      a -= b;
    }else{
      a = JAAT.stack[arg_0];
      a -= arg_1;
    }
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
  if(JAAT.zero == true){
    if(arg_0 > prg->programm_lenght || arg_0 < 0){
      JAAT.halt = true;
      OUT_OF_PROGRAMM();
    }
    JAAT.programm_counter = (arg_0*3)-3;
  }
}

void jaat_nxt(void){
  JAAT.current_pointer += 1;
  if(JAAT.current_pointer > STACK_LENGHT){
    STACK_OVER();
    JAAT.current_pointer = 0;
  }
}

void jaat_prv(void){
  JAAT.current_pointer -=1;
  if(JAAT.current_pointer < 0){
    STACK_UNDER();
    JAAT.current_pointer  = STACK_LENGHT;
  }
}

void jaat_swp(int arg_0, int arg_1){
  if((arg_0 >= 0 && arg_0 < STACK_LENGHT) && (arg_1 >= 0 && arg_1 < STACK_LENGHT)){
    STACK_TYPES data = JAAT.stack[arg_0];
    STACK_TYPES data_2 = JAAT.stack[arg_1];
    JAAT.stack[arg_0] = data_2;
    JAAT.stack[arg_1] = data;
  }
}


void jaat_inc(int arg_0){
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
  if(DEBUG) printf("\nstdout -> ");
  fprintf(stdout, "%s", str);
  if(DEBUG) printf("\n");
  free(JAAT.string_ptr);
  JAAT.string_ptr = NULL;
}

void jaat_scn(){
  char chr;
  if(DEBUG) printf("\n-> stdin: ");
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
  if(DEBUG) printf("\n");
  return;
}

void jaat_prt_constant(){
  if(DEBUG) printf("\nstdout -> ");
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
      if(DEBUG){
        printf("\n");
      }
      break;
    default:
      NO_IMPLEMENTATION();
      break;
  } 
}

void jaat_get(int arg_0){
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT){
    JAAT.accumulator = JAAT.stack[arg_0];
  }
}

void jaat_put_constant(){
  switch (JAAT.constant_type) {
    case ACC:
      jaat_put(JAAT.accumulator); 
      break;
    default:
      NO_IMPLEMENTATION();
      break;
  }
}



void jaat_jsr(int arg_0){
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT){
    JAAT.stack[JAAT.stack_address_pointer + STACK_LENGHT-256] = JAAT.programm_counter;
    JAAT.stack_address_pointer += 1;
    if(JAAT.stack_address_pointer >= 256){
      JAAT.stack_address_pointer = 0;
    }
    JAAT.programm_counter = (arg_0*3) - 3;
  }
}

void jaat_rts(){
  JAAT.stack_address_pointer -= 1;
  if(JAAT.stack_address_pointer  < 0){
    JAAT.stack_address_pointer = 256;
  }
  JAAT.programm_counter = JAAT.stack[JAAT.stack_address_pointer + STACK_LENGHT-256];
}

#endif

#endif
