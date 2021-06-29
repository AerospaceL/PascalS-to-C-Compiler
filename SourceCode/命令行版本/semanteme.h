#ifndef _SEMANTEME_
#define _SEMANTEME_
#include "head.h"

constexpr auto Xsplit = "---------------------\n";

using namespace std;

#define CONST 1
#define BASIC_VAR 2
#define ARRAY_VAR 3
#define FUNCTION 4
#define PROCEDURE 5
#define RECORDD 6

#define HASH_SIZE 100

#define PROCEDURE_CALL 1
#define OTHER_GRAMMERS 50

typedef vector<int> expression_type_list_;
typedef vector<bool> ref_flags_;	//true表示传引用

typedef struct {
	int para_type;
	string para_name;
	bool ref_flag;
}para_item_pair;

typedef struct {
	int typee;
}sem_item_const;

typedef struct {
	int typee;
}sem_item_basic_var;

typedef struct {
	vector<range>dimension;
	int array_typee;
	string record_type_name;	//如果array_typee=RECORD 8的话，去record类型里找叫record_type_name的record
}sem_item_array_var;

typedef struct {
	int member_type;
	string member_name;
	sem_item_array_var array_info;	//如果member_type=ARRAY 1的话，记录array信息
	string record_type_name;	//如果member_type=RECORD 8的话，去record类型里找叫record_type_name的record
}record_member_pair;

typedef vector<record_member_pair> record_member_list_;

typedef struct {
	expression_type_list_ para_type_list;
	ref_flags_ ref_flag_list;
	int return_type;
}sem_item_function;

typedef struct {
	expression_type_list_ para_type_list;
	ref_flags_ ref_flag_list;
}sem_item_procedure;

typedef struct {
	string record_type_name;
	//record_member_list_ member_list;
}sem_item_record;

typedef struct {
	int type;
	sem_item_const 	   attri_const;
	sem_item_basic_var attri_basic_var;
	sem_item_array_var attri_array_var;
	sem_item_function  attri_function;
	sem_item_procedure attri_procedure;
	sem_item_record	   attri_record;
}sem_item;	//语义分析用的表元素 

typedef struct {
	string name;	//名字
	sem_item item_attri;	//属性
	int next_chain_item_index;	//冲突链下一个表项
}symbol_list_item;	//符号表表项 

typedef struct {
	expression_type_list_ expression_type_list;
	int expression_type;
	bool valid_flag;
}sem_stack_item;	//语义分析用的栈元素 


void init_semanteme();	//初始化函数

void push_symbol_table_item(string name, sem_item item, bool tab_flag, int cursor);	//符号表添加表项【插入操作】

int search_symbol_table(string name);	//【检索操作】

void locate_symbol_table();	//【定位操作】（识别块的开始）

void relocate_symbol_table();	//【重定位操作】（识别出块的结束）

bool not_redu_name(string name, int cursor);	//块内重名检测

int judge_lr_expression_type(int left, int right, string opr, int cursor);	//表达式左右运算类型合法判断

int judge_arrary_range(string name, expression_type_list_ expression_type_list_val, int cursor);//数组元素类型以及下标范围判断

int judge_var(string name, int cursor);	//变量/常量类型判断

void judge_assign_type(string name, int variable_type, int  expression_type, int cursor);	//赋值语句类型判断

int judge_func_and_proc(string name, expression_type_list_ expression_type_list_val, int cursor);	//函数/过程参数列表判断

void judge_if_expression(int expression_type, int cursor);	//if语句条件类型合法判断

void judge_while_expression(int expression_type, int cursor);	//while语句条件类型合法判断

void judge_for_expression(string name, int from, int to, int cursor);	//for语句条件类型合法判断

void judge_read_item(string name, int cursor);	//read语句被赋值元素合法性判断

pair<bool, int> judge_record(string name, int cursor);	//record类型判断，兼顾合法判断

unsigned int string_hash(string name);	//对名字进行哈希映射

vector<para_item_pair> split_para_line(string para_line);	//切分参字符串

string get_se_type(int type);	//type int转string的功能函数

int get_se_gr_type(string type);	//type string转int的功能函数

vector<string> split(string str, string pattern);	//字符串分割

void trim(string& s);	//去掉字符串中所有空格

string trim_head(string s);	//去掉头部空格

bool include_ch(string s, char ch);	//判断s里是否包含ch

bool is_basic_type(int type);	//判断是不是基本类型

bool replace_function_return(string name, int from_grammar_type, int cursor);	//判断要不要替换为result

void register_record_type(string record_type_name, record_member_list_ record_info, int cursor);	//相当于typedef 注册一种record类型
//需要参数：string（名字）, sem_item_record（成员列表）

void test_print_all();	//测试输出符号表

#endif
