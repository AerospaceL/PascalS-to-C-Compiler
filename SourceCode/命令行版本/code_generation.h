#ifndef _CODE_GENERATION_
#define _CODE_GENERATION_
#include "head.h"
using namespace std;

void set_var_flag(int flag);
int get_var_flag();
void append_partial_code_line(string partial_code_line);
string get_temp_line();
void clean_temp_line();
void append_code_line(string code_line);
void tab_change(int tab_change);
void test_code_generation();
string get_c_type(int basic_type);
int get_tab_num();

void init_code_generation();

#endif 

