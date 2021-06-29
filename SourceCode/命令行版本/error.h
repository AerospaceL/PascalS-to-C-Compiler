//错误处理模块
#ifndef _ERROR_
#define _ERROR_
#include "head.h"
using namespace std;

//阶段定义
#define LEXEME 1  //词法分析
#define GRAMMAR 2  //语法分析
#define SEMANTEME 3  //语义分析

typedef struct {
	int stage; //错误出现的阶段	
	int row; //错误出现的行数
	int col; //错误出现的列数（不一定使用）
	string content; //字符串内容（供词法分析阶段的错误记录）
	string error_code; //错误代码
}error_item;  //错误条目

void append_error(int stage, string error_code, int row, int col = 0, string content = "");
#endif
