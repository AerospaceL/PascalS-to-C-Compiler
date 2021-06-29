#include "head.h"

//ȫ�ֱ���
ifstream in;  //Դ����������
ofstream out; //Ŀ����������
ofstream log_file; //��־�����


vector<word>word_stream;//�Ǻ���
vector<string>code_stream;//���ɴ�����
vector<error_item>error_stream;//������


//������

void process()
{
	lexical_analysis(); //�ʷ�����
	grammatical_analysis();//�﷨�Ƶ�����
}


int main(int argc, char* argv[])
{
	settings params = preprocess(argc, argv);  //�������������û�����Ĳ���ȷ������������ʽ

	init();
	cout << endl;
	if (input(params))  //Դ�������
	{
		process(); //������
		output(params);  //Ŀ��������־���

		test_code_generation();

		test_print_all();	//����������� krr
	}
}



