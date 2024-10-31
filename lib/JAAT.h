/* 
 *
 *
 *
 *        _____                    _____                    _____                _____          
         /\    \                  /\    \                  /\    \              /\    \
        /::\    \                /::\    \                /::\    \            /::\    \
        \:::\    \              /::::\    \              /::::\    \           \:::\    \
         \:::\    \            /::::::\    \            /::::::\    \           \:::\    \
          \:::\    \          /:::/\:::\    \          /:::/\:::\    \           \:::\    \
           \:::\    \        /:::/__\:::\    \        /:::/__\:::\    \           \:::\    \
           /::::\    \      /::::\   \:::\    \      /::::\   \:::\    \          /::::\    \
  _____   /::::::\    \    /::::::\   \:::\    \    /::::::\   \:::\    \        /::::::\    \
 /\    \ /:::/\:::\    \  /:::/\:::\   \:::\    \  /:::/\:::\   \:::\    \      /:::/\:::\    \
/::\    /:::/  \:::\____\/:::/  \:::\   \:::\____\/:::/  \:::\   \:::\____\    /:::/  \:::\____\
\:::\  /:::/    \::/    /\::/    \:::\  /:::/    /\::/    \:::\  /:::/    /   /:::/    \::/    /
 \:::\/:::/    / \/____/  \/____/ \:::\/:::/    /  \/____/ \:::\/:::/    /   /:::/    / \/____/ 
  \::::::/    /                    \::::::/    /            \::::::/    /   /:::/    /          
   \::::/    /                      \::::/    /              \::::/    /   /:::/    /           
    \::/    /                       /:::/    /               /:::/    /    \::/    /            
     \/____/                       /:::/    /               /:::/    /      \/____/             
                                  /:::/    /               /:::/    /                           
                                 /:::/    /               /:::/    /                            
                                 \::/    /                \::/    /                             
                                  \/____/                  \/____/                              


  A SIMPLE AND FAST VIRTUAL MACHINE AND BACKEND IN C

*/ 



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
#define DEBUG true 
#define NAME_SPACE_LENGHT 256
#define MACHINE_STATE false


/*

=======================================

JAAT INSTRUCTION DECLARATION BY USING 
  THE X MACRO TECHNIQUE 

======================================

*/ 



/*
 *  STEP TO ADD AN INSTRUCTION
 *
 *  1 - First declare the instruction in the correct class (LIST_OF_INSTRUCTION, LIST_OF_INSTRUCTION_C_PRS, LIST_OF_INSTRUCTION_S_PRS, LIST_OF_INSTRUCTION_A_PRS, LIST_OF_INSTRUCTION_NSP) 
 *  2 - define and create the function associated with the instruction
 *  3 - create a tag to link the instruction
 *  4 - add the tag in the function jaat_exec() by creating a specific case in the switch case
 *  5 - IF you find some problem you can compile JAAT with the debug flag activated to check at witch level JAAT fails.
 *
 *
 *  If you want to add new constant to parse you can update the CONSTANT_TYPE_LIST up here 
 *
 * */



/* instruction that support constant as argument */

#define LIST_OF_INSTRUCTION_C_PRS(ext) \
  X(GET,ext)\
  X(PRT,ext)\
  X(PUT,ext)\
  X(INC,ext)\
  X(DEC,ext)

/* instruction that support string as argument */

#define LIST_OF_INSTRUCTION_S_PRS(ext) \
  X(PRT,ext)\
  X(PUT,ext)

/* instruction that support address operation */

#define LIST_OF_INSTRUCTION_A_PRS(ext) \
  X(ADC,ext)\
  X(SBC,ext)\
  X(CMP,ext)

/* instruction that support NAME_SPACE usage ( usually jump instruction and variation ) */

#define LIST_OF_INSTRUCTION_NSP() \
  X(JMP)\
  X(JNZ)\
  X(JPO)\
  X(JSR)\
  X(JEQ)


/* base instruction */ 

#define LIST_OF_INSTRUCTION() \
  X(HLT)\
  X(PUT)\
  X(POP)\
  X(GET)\
  X(ADC)\
  X(SBC)\
  X(RTS)\
  X(CMP)\
  X(NXT)\
  X(PRV)\
  X(SWP)\
  X(PRT)\
  X(INC)\
  X(DEC)\
  X(SCN)\
  LIST_OF_INSTRUCTION_NSP()\


/*

=======================================

JAAT CONSTANT DECLARATION USABLE
  BY EACH INSTRUCTION IF YOU WANT

======================================

*/ 




#define CONSTANT_TYPE_LIST\
  X(INPUT)\
  X(ACC)\
  X(STACK_PTR)

/*

=======================================

JAAT MAIN LAUNCH SEQUENCE

======================================

*/ 



#define launch_vm(prg) \
  jaat_start(prg->nelem+1);\
  jaat_load_programm(prg);\
  jaat_loop();\
  jaat_free();\
  array_free(prg);


/*
 * 
 *  Some preprocessor things to manage error message
 *
 * */

#define NO_IMPLEMENTATION() fprintf(stderr,"Feature under development\n"); return;
#define ILLEGAL_INST(line, message) fprintf(stderr, "[PARSER]: ERROR: Illegal instruction on line %d: %s", line, message); exit(7);
#define STR_LEN 512
#define STACK_OVER() fprintf(stdout, "[SYSTEM]: WARNING: stack overflow reached, but execution is not halted\n"); 
#define STACK_UNDER() fprintf(stdout, "[SYSTEM]: WARNING: stack underflow reached, but execution is not halted\n");
#define OUT_OF_PROGRAMM() fprintf(stderr, "[SYSTEM]: Programm counter is try to read out of programm lenght");  




/*

=======================================

JAAT INSTRUCTION LIST, CONSTANT 
        LIST AND MAIN DATA STRUCTURE

======================================

*/ 


/* NOTE: here the vm_inst enumerator is built with x macro technique,
 * remember that for each variation you need to declare the function
 * associated with the instruction AND the tag, then you can update the
 * jaat_exec function.
 *
 * I tell you this because it's not obvious that every variation have
 * an individual function associated, for example the address fariation
 * is declared as a separate instruction, BUT it use the same function 
 * with a flag to tell if is called to work with address or not.
 *
 *
 * */


typedef enum{

#define X(name) name,

  LIST_OF_INSTRUCTION()

#undef X

#define X(name, ext) name##ext,

  LIST_OF_INSTRUCTION_C_PRS(_CONSTANT)
  LIST_OF_INSTRUCTION_S_PRS(_STRING)
  LIST_OF_INSTRUCTION_A_PRS(_ADR)

#undef X

}vm_inst;


//
//  Constant type: where the constant are defined
//

#define X(name)name,

typedef enum{
  CONSTANT_TYPE_LIST
}vm_constant_type;

#undef X

//
//  JAAT virtual machine: it's a stack based machine, although it's slickly modified concept of a stack machine 
//

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

//
//  Programm buffer: After you launch the vm it will be allocated based on the dynamic array size. VM_PROGRAMM store all instruction
//

typedef struct{
  int programm_lenght;
  char** inst_array;
}vm_programm;


//
//  Box item: used for archive function references and the address related to them
//


typedef struct{
  int index;
  char* name_space;
}Box;


/*

=======================================

JAAT MAIN FUNCTION

======================================

*/ 



void jaat_loop(void);
void jaat_load(vm_inst instruction, int arg_0,int arg_1);
void jaat_exec(void);
void jaat_free(void);
void jaat_start(int size);
void jaat_load_programm(Array *new_prg);
void parse_instruction(void);
int parser_check_for_namespace(vm_inst inst, char* line);
void parse_preprocessor();
void jaat_debug(int inst_type, int pc);



/*

=======================================

JAAT FUNCTION USED FOR 
  IMPLEMENT ALL INSTRUCTIONS, YOU CAN 
  USE YOUR CUSTOM ONE, YOU NEED ONLY TO 
  LINK IT WITH THE TAG DEFINITION BELOW

======================================

*/ 



void jaat_hlt(void);
void jaat_put(int arg_0);
void jaat_put_constant(void);
void jaat_put_string(void);
void jaat_pop(void);
void jaat_get(int arg_0);
void jaat_get_constant(void);
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
void jaat_inc_constant(void);
void jaat_dec_constant(void);
void jaat_dec(int arg_0);
void jaat_scn(void);


/*

=======================================

JAAT TAGS DEFINITION 
    FOR EXECUTION REFERENCE

======================================

*/ 



#define HLT()                   jaat_hlt();             // halt function: halt the execution
#define PUT_CONSTANT()          jaat_put_constant();    // put function with constant: basically put but with special things 
#define PUT_STRING()            jaat_put_string();      // put string function: put a string into stack
#define PUT(arg_0)              jaat_put(arg_0);        // put function:  put arg_0 data into stack
#define POP()                   jaat_pop();             // pop function:  pop the stack content from stack and load the accumulator
#define GET(arg_0)              jaat_get(arg_0);        // get function:  get item from stack and put it into accumulator
#define GET_CONSTANT()          jaat_get_constant();
#define ADC(arg_0,arg_1,flag)   jaat_adc(arg_0,arg_1,flag);  // adc function:  sum accumulator content with arg_1 content and put into arg_0 address, if flag is enable then the sum is between arg_0 and arg_1 location content
#define SBC(arg_0,arg_1,flag)   jaat_sbc(arg_0,arg_1,flag);  // sbc function:  sub accumulator content with arg_1 content and put into arg_0 address, if flag is enable then it behave the same as ADC
#define JMP(arg_0)              jaat_jmp(arg_0);        // jmp function:  jump to a specific point in programm
#define JNZ(arg_0)              jaat_jnz(arg_0);        // jnz funciont:  jump if the alu result is not 0
#define JEQ(arg_0)              jaat_jeq(arg_0);        // jeq funciont:  jump if zero is set
#define JSR(arg_0)              jaat_jsr(arg_0);        // jsr function: jump into subroutine
#define RTS()                   jaat_rts();             // rts function: return from subroutine

#define CMP(arg_0, arg_1, flag) jaat_cmp(arg_0, arg_1, flag); // cmp function:  compare arg_0 stack location with arg_1
#define JPO(arg_0)              jaat_jpo(arg_0);        // jpo function:  jump if the alu overflow frlag is set
#define PRT(arg_0)              jaat_prt(arg_0);        // prt print function: print content inside the provided location
#define NXT()                   jaat_nxt();             // nxt function: increment the stack pointer by one
#define PRV()                   jaat_prv();             // prv function: decrement the stack pointer by one 
#define SWP(arg_0,arg_1)        jaat_swp(arg_0,arg_1);  // swp function: swap the content of two element 
#define INC(arg_0)              jaat_inc(arg_0);        // inc function: increment data in position arg_0
#define INC_CONSTANT()          jaat_inc_constant();    // inc constant function: increment by one a constant reference
#define DEC(arg_0)              jaat_dec(arg_0);        // dec function: decrement data in position arg_0
#define DEC_CONSTANT()          jaat_dec_constant();    // inc constant function: decrement by one a constant reference 

#define PRT_STRING(arg_0)       jaat_prt_string(arg_0); // prt_string:   string variation of prt
#define SCN()                   jaat_scn();             // scn function: scan for keyboard input
#define PRT_CONSTANT()          jaat_prt_constant();    // prt constant: print a constant reference



/*

=======================================

JAAT STATIC VARIABLE AND 
            BUFFER POINTERS

======================================

*/ 



int input_buffer_size = 255;      // keyboard input circular buffer size
int pool_size;                    // instruction pool size 
vm JAAT = {0};                    // JAAT vm
int fn_name_space_ptr = 0;        // function namespace pointer

BYTE_LENGHT* instruction_pool;    // instruction_pool: will be filled inside the parser
char** string_buffer;             // string buffer: will be filled with the string inside some instruction
vm_constant_type* constant_type_buffer; // constant type buffer: will be filled with the constant inside some instruction
char* input_buffer;               // keyboard input buffer 
Box *fn_name_space;               // function namespace buffer: where a box item will be created inside a parser if there is a reference to a function

vm_programm* prg;                 // loaded programm buffer: it's used inside the parser to parse each instructions

#ifdef JAAT_IMPLEMENTATION

#include "JAAT.h"


/*

=======================================

JAAT PROGRAMM LOADER

======================================

*/ 

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

  /* reset/initialize vm state: needed if you load and lauch multiple programms within the same process */

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

  /* call the parser for loading the instruction */
  parse_preprocessor();
  parse_instruction();
}





/*

=======================================

JAAT INSTRUCTION PREPROCESSOR

======================================

*/ 


void parse_preprocessor(){
  int fn_index = 0;
  if(DEBUG) printf("[PARSER PREPROCESSOR]: Starting preprocessing, checking for name space definition\n");
  for(int i=0;i<prg->programm_lenght;i++){
    // preprocessor: parsing namespace for function reference

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



/*

=======================================

JAAT INSTRUCTION PARSER

======================================

*/ 

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

#define X(name) \
    if(strcmp(inst, #name) == 0){\
      type = name;\
      printf("!! Found: "#name" \n");\
    }else 

    LIST_OF_INSTRUCTION()

#undef X

    if(inst[0] == '!'){
      skip = true;
    }else{
      fprintf(stderr, "[PARSER]: ERROR: no such instruction to parse, failed on line %d\n", i+1);
      exit(3);
    }
    
    int start_point;
    bool end = false;
    
    // find the starting point for parsing the arguments
    for(start_point = 0; start_point < WORD_LEN && !end && !name_space_found && !skip && !fun_reference ;start_point++){
      if(prg->inst_array[i][start_point] == '('){
        end = true;
      }
    }

    // check for full address variation
    if(prg->inst_array[i][start_point-2] == '#'  && !name_space_found && !skip){
      if(DEBUG) printf("[PARSER]: Parsing address function variation\n");
      switch(type){

#define X(name,ext)\
        case name:\
          type = name##ext;\
          break;
    
        LIST_OF_INSTRUCTION_A_PRS(_ADR)
#undef X

        default:
          ILLEGAL_INST(i+1, "failed while checking for address variation\n");
          break;
      } 
    }

    // check for string variation
    if((prg->inst_array[i][start_point] == '"' || prg->inst_array[i][start_point] == 39) && !name_space_found && !skip){
      if(DEBUG) printf("[PARSER]: Parsing string literal variation\n");
      switch(type) {
#define X(name, ext)\
        case name:\
          type = name##ext;\
          break;
        
        LIST_OF_INSTRUCTION_S_PRS(_STRING)

#undef X

        default:
            ILLEGAL_INST(i+1, "failed while checking for string\n");
          break;
      }
      string = true;
    }

    // check for constant variation and name space reference
    if((isupper(prg->inst_array[i][start_point]) > 0) &&  !name_space_found && !skip && !fun_reference){
      if(DEBUG) printf("[PARSER]: Parsing constant variation\n");
      switch(type){
#define X(name,ext)\
        case name:\
          is_constant = true;\
          type = name##ext;\
          break;

      LIST_OF_INSTRUCTION_C_PRS(_CONSTANT)

#undef X

       default:
          arg_0 = parser_check_for_namespace(type, prg->inst_array[i]);
          if(arg_0 == -1){
            ILLEGAL_INST(i+1, "failed while checking for constant");
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
      if(prg->inst_array[i][start_point+ptr] == ')' && prg->inst_array[i][start_point+ptr+1] == 0){
        end = true;
      }else if(prg->inst_array[i][start_point+ptr] != ' ' && prg->inst_array[i][start_point+ptr] != ','){
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

    // check for lenght of string and constant
    end = false;
    while(!end && current_str_len < STR_LEN && ( string == true || is_constant == true ) && !name_space_found && !skip && !fun_reference ){
      if(prg->inst_array[i][start_point+ptr] == ')' && prg->inst_array[i][start_point+ptr+1] == 0){
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
      bool is_constant = false;

#define X(name) \
      if(strcmp(word, #name) == 0){\
        constant_type_buffer[const_index] = name;\
        const_index += 1;\
        is_constant = true;\
      }

      CONSTANT_TYPE_LIST

#undef X

      if(!is_constant){
        ILLEGAL_INST(i+1, "unable to identify constant\n"); 
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
}



/*

=======================================

JAAT INSTRUCTION PREPROCESSOR: 
        name space reference part

======================================

*/ 

int parser_check_for_namespace(vm_inst inst, char* line){
  // parse name spaces
  char local_name_space[NAME_SPACE_LENGHT];
  int length = 0;
  int arg_0 = -1;
  int i=0;
  if(DEBUG) printf("[PARSER]: checks for name space references for [%d] instruction\n", inst);
  switch (inst) {

#define X(name)\
    case name:

    LIST_OF_INSTRUCTION_NSP()

#undef X
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
      if(DEBUG) printf("[PARSER]: unable to find name space reference \n");
      break;
  }
  return arg_0;
}



/*

=======================================

JAAT ENVIRONMENT STARTER

======================================

*/ 

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




/*

=======================================

JAAT INSTRUCTION LOADER

======================================

*/ 

void jaat_load(vm_inst instruction, int arg_0,int arg_1){
  if(DEBUG) printf("[LOAD]: load single instruction\n");
  JAAT.current_instruction = instruction;
  JAAT.arg_0 = arg_0;
  JAAT.arg_1 = arg_1;
  switch(instruction){

#define X(name,ext)\
    case name##ext:

    LIST_OF_INSTRUCTION_S_PRS(_STRING)
      if(string_buffer[JAAT.string_tracker] != NULL){
        JAAT.string_ptr = string_buffer[JAAT.string_tracker];
        JAAT.string_tracker += 1;
      }
      break;

    LIST_OF_INSTRUCTION_C_PRS(_CONSTANT)
      if(constant_type_buffer[JAAT.const_tracker] != 0){
        JAAT.constant_type = constant_type_buffer[JAAT.const_tracker];  
        JAAT.const_tracker += 1;
      }
      break;

#undef X

    default:
      break;
  }
}




/*

=======================================

JAAT RUNTIME LOOP

======================================

*/ 

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
        jaat_debug(inst, JAAT.programm_counter);
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
    printf("\n\n[MACHINE STATE]: flags\n");
    printf("Zero flag: [%d]\n", JAAT.zero);
    printf("Zero overflow: [%d]\n", JAAT.overflow);
    printf("Zero negative: [%d]\n", JAAT.negative);

    printf("\n[MACHINE STATE]: pointer and tracker\n");
    printf("String tracker: [%d]\n", JAAT.string_tracker);
    printf("Constant tracker: [%d]\n", JAAT.const_tracker);
    printf("Current stack pointer: [%d]\n", JAAT.current_pointer);
    printf("Input tracker read: [%d]\n", JAAT.input_tracker_read);
    printf("Input tracker write: [%d]\n", JAAT.input_tracker_write);
    printf("stack address pointer: [%d]\n", JAAT.stack_address_pointer);

  } 
}


#define X(name) \
    case name:\
        printf(#name "\n");\
        break;

void jaat_debug(int inst_type, int pc){
        printf("[LOOP]: current programm counter: %d\n",pc);
        printf("[LOOP]: current instruction loaded: ");
        switch(inst_type){
          LIST_OF_INSTRUCTION()
        }
}

#undef X



/*

=======================================

JAAT RUNTIME INSTRUCTION EXECUTOR

======================================

*/ 


void jaat_exec(){
  if(DEBUG) printf("[EXECUTOR]: starting execution\n");
  switch(JAAT.current_instruction){
    case HLT:
        HLT();   
        break;
    case PUT:
        PUT(JAAT.arg_0);
        break;
    case PUT_STRING:
        PUT_STRING();
        break;
    case POP:
        POP();
        break;
    case GET:
        GET(JAAT.arg_0);
        break;
    case GET_CONSTANT:
        GET_CONSTANT();
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
    case INC_CONSTANT:
        INC_CONSTANT();
        break;
    case DEC:
        DEC(JAAT.arg_0);
        break;
    case DEC_CONSTANT:
        DEC_CONSTANT();
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


/*

=======================================

JAAT ENVIRONMENT FREE

======================================

*/ 

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

// ############################################################################################################
//  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ############################################################################################################ 
//  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

/*
 *
 *  NOTE: the address variation is implemented inside the normal instruction implementation, the 
 *  only way used to switch between normal operation and address  operation is with a flag passed 
 *  by argument
 *
*/

/*

=======================================

JAAT INSTRUCTION: normal one

======================================

*/ 



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

void jaat_get(int arg_0){
  if(arg_0 >= 0 && arg_0 < STACK_LENGHT){
    JAAT.accumulator = JAAT.stack[arg_0];
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


/*

=======================================

JAAT INSTRUCTION: constant variation

======================================

*/ 



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


void jaat_put_constant(){
  switch (JAAT.constant_type) {
    case ACC:
      PUT(JAAT.accumulator); 
      break;
    default:
      NO_IMPLEMENTATION();
      break;
  }
}

void jaat_get_constant(){
  switch(JAAT.constant_type){
    case STACK_PTR:
      JAAT.accumulator = JAAT.current_pointer;
      if(DEBUG){
        printf("\n");
      }
      break;
    default:
      NO_IMPLEMENTATION();
      break;
  } 
}

void jaat_inc_constant(){
  switch(JAAT.constant_type){
    case ACC:
      JAAT.accumulator += 1;
      if(DEBUG){
        printf("\n");
      }
      break;
    default:
      NO_IMPLEMENTATION();
      break;
  }
}

void jaat_dec_constant(){
  switch(JAAT.constant_type){
    case ACC:
      JAAT.accumulator -= 1;
      if(DEBUG){
        printf("\n");
      }
      break;
    default:
      NO_IMPLEMENTATION();
      break;
  }
}



/*

=======================================

JAAT INSTRUCTION: string variation

======================================

*/ 


void jaat_put_string(){
  for(int i=0;i<strlen(JAAT.string_ptr); i++){
    PUT((int)JAAT.string_ptr[i]);
  }
  if(DEBUG) printf("\n");
  free(JAAT.string_ptr);
  JAAT.string_ptr = NULL;
}



void jaat_prt_string(char*str){
  if(DEBUG) printf("\nstdout -> ");
  fprintf(stdout, "%s", str);
  if(DEBUG) printf("\n");
  free(JAAT.string_ptr);
  JAAT.string_ptr = NULL;
}


#endif

#endif
