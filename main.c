#include <stdio.h>
#include <stdlib.h>

#define JAAT_IMPLEMENTATION

#include "./lib/JAAT.h"

void help_message(){

  Array* help;
   launch_vm(help);
}


void list_message(){
  Array*list;
  array_new(list);
  
  launch_vm(list);
}

void jaat_ex_1(){
  Array* prg1;
  char program_1[][256] = {  
    {"PRT('put 69 into stack and add 1 until it reach 420\n')"},
    {"PUT(69)"},
    {"POP()"},
    {"ADC(0,1)"},
    {"PRT()"},
    {"CMP(0, 420)"},
    {"JEQ(9)"},
    {"NXT()"},
    {"JMP(2)"},
    {"PRT('\n')"},
    {"HLT()"},
  };
  array_new(prg1); 
  for(int i=0;i<11; i++){
    array_push(prg1, program_1[i];)
  }
  launch_vm(prg1);
}


void jaat_ex_2(){
// fibonacci sequence but with a for loop
  Array* prg2;
  char program_2[][256] = { 
    {"PRT('fibonacci sequence, but with for loop locked on 17\n')"},
    {"PUT(1)"},
    {"PUT(1)"},
    {"PUT(0)"},
    {"ADC#(0,1)"},
    {"SWP(0,1)"},
    {"INC(2)"},
    {"PRT(1)"},
    {"CMP(2, 17)"},
    {"JNZ(4)"},
    {"PRT(1)"},
    {"PRT('\n')"},
    {"HLT()"}
  };
  array_new(prg2); 
  for(int i=0;i<13; i++){
    array_push(prg2, program_2[i];)
  }
  launch_vm(prg2);
}

void jaat_ex_4(){
  Array*prg4;
  char program_4[][256] = {
    {"PRT('Input test, try to write something: ')"},
    {"SCN()"},
    {"PRT('You write: ')"},
    {"PRT(INPUT)"},
    {"PRT('\n')"},
    {"HLT()"}
  };
  array_new(prg4); 
  for(int i=0;i<6; i++){
    array_push(prg4, program_4[i];)
  }
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
          fprintf(stderr,"ERROR: missing argument\n");
        }
      }if(strcmp(argv[i], "-ex") == 0 && argc > i+1){
        if(strcmp(argv[i+1], "1") == 0) {
          jaat_ex_1();
        }else if(strcmp(argv[i+1], "2") == 0){
          jaat_ex_2();
        }else if(strcmp(argv[i+1], "3") == 0){
          jaat_ex_4();
        }else fprintf(stderr,"ERROR: missing argument\n");
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


