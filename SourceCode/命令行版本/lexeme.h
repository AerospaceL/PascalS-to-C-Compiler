#ifndef _LEXEME_
#define _LEXEME_
#include "head.h"
using namespace std;

#define ID	1
#define INT 2
#define REL 3
#define DIG 4	// ò��ûɶ��Ҫ�����ȿ���int
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
	int id;			// �����룺�궨���е���ţ�
	string symbol;	// ������Ƿ������� KEY_begin ֮���
	string value;	// ����ֵ, ���� program��i��j ֮���
	int row;		// �к�
	int col;		// �к�

}word;


void lexical_analysis();
void analysis();            // ��������  

void get_char();            // ��ȡ�ַ�  
bool is_key(string word);    // ��ʾʶ��ĵ����Ƿ��ǹؼ���  

void record_word(int record_id); 	// ����Ϣ¼��ṹ�� 
void record_error();		//¼�������Ϣ 
void lexical_output();

void init_lexeme();


#endif
