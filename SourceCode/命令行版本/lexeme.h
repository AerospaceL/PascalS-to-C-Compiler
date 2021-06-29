#ifndef _LEXEME_
#define _LEXEME_
#include "head.h"
using namespace std;

#define ID	1
#define INT 2
#define REL 3
#define DIG 4	// 貌似没啥必要，优先考虑int
#define STR 5
#define CHR 6 //jnadd

#define KEY_program 101
#define KEY_const   102
#define KEY_var		103
#define KEY_procedure 104
#define KEY_funtion	105
#define KEY_begin	106
#define KEY_end		107
#define KEY_array	108
#define KEY_of		109
#define KEY_integer	110
#define KEY_real	111
#define KEY_boolean	112
#define KEY_char	113
#define KEY_if		114
#define KEY_then	115
#define KEY_else    116
#define KEY_for     117
#define KEY_to		118
#define KEY_do      119
#define KEY_read	120
#define KEY_write	121
#define KEY_or		122
#define KEY_div		123
#define KEY_mod		124
#define KEY_and		125
#define KEY_not		126
#define KEY_string	127
#define KEY_true    128
#define KEY_false   129
#define KEY_while   130
#define KEY_type    131
#define KEY_record  132

#define OPR_assign	201
#define OPR_equal	202
#define OPR_add		203
#define OPR_sub		204
#define OPR_great	205
#define OPR_less	206
#define OPR_gequal	207
#define OPR_lequal	208
#define OPR_nequal	209
#define OPR_mul		210
#define OPR_div		211

#define PUN_lbrkt	301
#define PUN_rbrkt	302
#define PUN_comma	303
#define PUN_semicolon	304
#define PUN_dot		305
#define PUN_colon	306
#define PUN_quotation	307
#define PUN_lsbrkt	308
#define PUN_rsbrkt	309
#define PUN_lcbrkt	310
#define PUN_rcbrkt	311
#define PUN_ddot	312

typedef struct
{
	int id;			// 类别编码：宏定义中的序号，
	string symbol;	// 类别助记符：类似 KEY_begin 之类的
	string value;	// 属性值, 类似 program，i，j 之类的
	int row;		// 行号
	int col;		// 列号

}word;


void lexical_analysis();
void analysis();            // 分析程序  

void get_char();            // 读取字符  
bool is_key(string word);    // 表示识别的单词是否是关键字  

void record_word(int record_id); 	// 将信息录入结构体 
void record_error();		//录入错误信息 
void lexical_output();

void init_lexeme();


#endif
