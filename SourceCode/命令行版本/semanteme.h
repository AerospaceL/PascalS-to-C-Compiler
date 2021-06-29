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
typedef vector<bool> ref_flags_;	//true��ʾ������

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
	string record_type_name;	//���array_typee=RECORD 8�Ļ���ȥrecord�������ҽ�record_type_name��record
}sem_item_array_var;

typedef struct {
	int member_type;
	string member_name;
	sem_item_array_var array_info;	//���member_type=ARRAY 1�Ļ�����¼array��Ϣ
	string record_type_name;	//���member_type=RECORD 8�Ļ���ȥrecord�������ҽ�record_type_name��record
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
}sem_item;	//��������õı�Ԫ�� 

typedef struct {
	string name;	//����
	sem_item item_attri;	//����
	int next_chain_item_index;	//��ͻ����һ������
}symbol_list_item;	//���ű���� 

typedef struct {
	expression_type_list_ expression_type_list;
	int expression_type;
	bool valid_flag;
}sem_stack_item;	//��������õ�ջԪ�� 


void init_semanteme();	//��ʼ������

void push_symbol_table_item(string name, sem_item item, bool tab_flag, int cursor);	//���ű���ӱ�����������

int search_symbol_table(string name);	//������������

void locate_symbol_table();	//����λ��������ʶ���Ŀ�ʼ��

void relocate_symbol_table();	//���ض�λ��������ʶ�����Ľ�����

bool not_redu_name(string name, int cursor);	//�����������

int judge_lr_expression_type(int left, int right, string opr, int cursor);	//���ʽ�����������ͺϷ��ж�

int judge_arrary_range(string name, expression_type_list_ expression_type_list_val, int cursor);//����Ԫ�������Լ��±귶Χ�ж�

int judge_var(string name, int cursor);	//����/���������ж�

void judge_assign_type(string name, int variable_type, int  expression_type, int cursor);	//��ֵ��������ж�

int judge_func_and_proc(string name, expression_type_list_ expression_type_list_val, int cursor);	//����/���̲����б��ж�

void judge_if_expression(int expression_type, int cursor);	//if����������ͺϷ��ж�

void judge_while_expression(int expression_type, int cursor);	//while����������ͺϷ��ж�

void judge_for_expression(string name, int from, int to, int cursor);	//for����������ͺϷ��ж�

void judge_read_item(string name, int cursor);	//read��䱻��ֵԪ�غϷ����ж�

pair<bool, int> judge_record(string name, int cursor);	//record�����жϣ���˺Ϸ��ж�

unsigned int string_hash(string name);	//�����ֽ��й�ϣӳ��

vector<para_item_pair> split_para_line(string para_line);	//�зֲ��ַ���

string get_se_type(int type);	//type intתstring�Ĺ��ܺ���

int get_se_gr_type(string type);	//type stringתint�Ĺ��ܺ���

vector<string> split(string str, string pattern);	//�ַ����ָ�

void trim(string& s);	//ȥ���ַ��������пո�

string trim_head(string s);	//ȥ��ͷ���ո�

bool include_ch(string s, char ch);	//�ж�s���Ƿ����ch

bool is_basic_type(int type);	//�ж��ǲ��ǻ�������

bool replace_function_return(string name, int from_grammar_type, int cursor);	//�ж�Ҫ��Ҫ�滻Ϊresult

void register_record_type(string record_type_name, record_member_list_ record_info, int cursor);	//�൱��typedef ע��һ��record����
//��Ҫ������string�����֣�, sem_item_record����Ա�б�

void test_print_all();	//����������ű�

#endif
