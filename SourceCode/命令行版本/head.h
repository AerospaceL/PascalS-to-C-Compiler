#ifndef _HEAD_
#define _HEAD_

#include<iostream>  
#include<fstream>  
#include<vector>  
#include<iomanip> 
#include <stack>
#include <map>
#include <string>
#include "lexeme.h"
#include "grammar.h"
#include "semanteme.h"
#include "code_generation.h"
#include "error.h"
#include "utils.h"

using namespace std;

//全局变量
extern ifstream in;  //源代码输入输入流
extern ofstream out; //目标代码输出流
extern ofstream log_file; //日志输出流

extern vector<word>word_stream;//记号流
extern vector<string>code_stream;//生成代码流
extern vector<error_item>error_stream;//错误流


#endif