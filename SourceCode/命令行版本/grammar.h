//语义分析模块
#ifndef _GRAMMAR_
#define _GRAMMAR_
#include "head.h"
using namespace std;

// 这些define千万不要改 krr
#define ARRAY 1
#define INTEGER 2
#define REAL 3
#define CHAR 4
#define BOOLEAN 5
#define VOID 6

#define STRR 7	//krr
#define RECORD 8

#define MERVEILLEUS 99	//krr57

typedef struct{
	string type;
	string value;
}const_value_;

typedef vector<string> idlist_;

typedef vector<string> expression_list_;

typedef vector<string> variable_list_;

typedef struct{
	string upper; //数组上界
	string lower; //数组下界
}range;

typedef struct {
	int type;
	string id;
	vector<range>dimension;
	int array_type;
	string array_id;
}type_;

typedef struct {
	int type;  //子程序类型
	string id;  //子程序标识符号
}subprogram_head_;

typedef struct {
	type_ type_val; 
	idlist_ idlist_val; //相关id
}item;

typedef vector<item> record_list_;

typedef struct {
	char cval;
	int ival;
	double dval;
	string sval;
	const_value_ const_value_val;
	idlist_ idlist_val;
	type_ type_val;
	subprogram_head_ subprogram_head_val;
	expression_list_ expression_list_val;
	variable_list_ variable_list_val;
	record_list_ record_list_val;
}attribute_value;


void grammatical_analysis();
void analysis_preparation();
void programstruct();
void program_head();
void program_body();

void idlist();

void record_declarations();
void record_list();

void const_declarations();
void const_declaration();
void const_value();
void const_declarationT();

void var_declarations();
void var_declaration();
void var_declarationT();
void type();
void basic_type();
void period();
//void periodT();

void subprogram_declarations();
void subprogram();
void subprogram_head();
void subprogram_body();
void formal_parameter();
void parameter_list();
void parameter();
void var_parameter();
void value_parameter();
void compound_statement();
void statement_list();

void statement();
void assign_expression();
void procedure_call();
void if_expression();
void for_expression();
void while_expression();
void read_expression();
void write_expression();

void variable_list();
void variable();
//void record();

void expression_list();
void expression();
void simple_expression();
void term();
void factor();

void print_grammer_error();

void init_grammar();

#endif
