//ͨ��ģ��
#ifndef _UTILS_
#define _UTILS_
#include "head.h"
using namespace std;

typedef struct {
	string scr_path;  //Դ����·��
	string dst_path;  //Ŀ�����·��
	string log_path;  //��־·��
	bool file_output;  //�Ƿ��ṩ�ļ����
}settings;

settings preprocess(int argc, char* argv[]);  //�������������û�����Ĳ���ȷ������������ʽ
bool input(settings params);  //Դ�������
bool output(settings params);  //Ŀ��������־���

void init();
#endif