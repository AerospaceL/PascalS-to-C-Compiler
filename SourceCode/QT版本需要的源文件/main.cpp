#include "compiler_design_qt.h"
#include <QtWidgets/QApplication>
#include "CodeEditor.h"
#include "head.h"

//ȫ�ֱ���
ifstream in;  //Դ����������
ofstream out; //Ŀ����������
ofstream log_file; //��־�����


vector<word>word_stream;//�Ǻ���
vector<string>code_stream;//���ɴ�����
vector<error_item>error_stream;//������


int a, b[10][10];

//������

//void process()
//{
//	lexical_analysis(); //�ʷ�����
//	grammatical_analysis();//�﷨�Ƶ�����
//
//}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    compiler_design_qt w;
    w.show();
    return app.exec();

	//settings params = preprocess(argc, argv);  //�������������û�����Ĳ���ȷ������������ʽ

	//if (input(params))  //Դ�������
	//{
	//	process(); //������
	//	output(params);  //Ŀ��������־���

	//	test_code_generation();
	//}
}
