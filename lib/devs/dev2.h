#ifndef DEV2_H
#define DEV2_H 



// declare method here like this: 
//
//  void dev2_foo(int arg_0, int arg_1, int arg_2);
//  void dev2_bar(int arg_0, int arg_1, int arg_2);
//  void dev2_baz(int arg_0, int arg_1, int arg_2);

void dev2_log(int arg_0,int arg_1,int arg_2, static char* string)

#ifdef DEV2_IMP


void dev2_log(int arg_0,int arg_1,int arg_2, static char* string){
  fprintf(stdout, "%d %d %d %s", arg_0, arg_1,arg_2,string);
}


#endif

#endif // dev2 h
