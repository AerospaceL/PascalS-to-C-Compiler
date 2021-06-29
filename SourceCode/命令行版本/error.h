//������ģ��
#ifndef _ERROR_
#define _ERROR_
#include "head.h"
using namespace std;

//�׶ζ���
#define LEXEME 1  //�ʷ�����
#define GRAMMAR 2  //�﷨����
#define SEMANTEME 3  //�������

typedef struct {
	int stage; //������ֵĽ׶�	
	int row; //������ֵ�����
	int col; //������ֵ���������һ��ʹ�ã�
	string content; //�ַ������ݣ����ʷ������׶εĴ����¼��
	string error_code; //�������
}error_item;  //������Ŀ

void append_error(int stage, string error_code, int row, int col = 0, string content = "");
#endif
