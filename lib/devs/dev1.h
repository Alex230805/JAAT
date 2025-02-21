#ifndef DEV1_H
#define DEV1_H 



// declare method here like this: 
//
//  void dev1_foo(int arg_0, int arg_1, int arg_2);
//  void dev1_bar(int arg_0, int arg_1, int arg_2);
//  void dev1_baz(int arg_0, int arg_1, int arg_2);


void dev1_log(int arg_0,int arg_1,int arg_2, static char* string);


#ifdef DEV1_IMP

void dev1_log(int arg_0,int arg_1,int arg_2, static char* string){
  fprintf(stdout, "%d %d %d %s", arg_0, arg_1,arg_2,string);
}


#endif

#endif // DEV1_H
