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

//ȫ�ֱ���
extern ifstream in;  //Դ��������������
extern ofstream out; //Ŀ����������
extern ofstream log_file; //��־�����

extern vector<word>word_stream;//�Ǻ���
extern vector<string>code_stream;//���ɴ�����
extern vector<error_item>error_stream;//������


#endif