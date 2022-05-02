#include "head.h"

void init()
{
	//���������
	word_stream.clear();
	code_stream.clear();
	error_stream.clear();

	init_lexeme();
	init_grammar();
	init_semanteme();
	init_code_generation();
}


settings preprocess(int argc, char* argv[])
{
	//����Ϊ�˲��Է�����ʱ�����롢���д��
	//֮��Ӧ�ø����û�����Ĳ�������settings

	settings params;
	for (int i = 0; i < argc; i++) {
		//szl//cout << argv[i] << endl;
	}

	if (argc == 1) {
		params.scr_path = "test.txt";
		params.dst_path = "result.c";
		params.log_path = "log.txt";
	}
	else if (argc == 2){
		params.scr_path = argv[1];
		params.dst_path = "result.c";
		params.log_path = "log.txt";
	}
	else if (argc == 3) {
		params.scr_path = argv[1];
		params.dst_path = argv[2];
		params.log_path = "log.txt";
	}
	params.file_output = true;

	return params;
}

bool input(settings params)
{
	bool success = true;
	in.open(params.scr_path, ios::in);
	if (!in) {
		//szl//cout << "���ļ�ʧ�ܡ�" << endl;
		success = false;
	}
	else
	{
		//szl//cout << "���ļ��ɹ���" << endl;
	}
	return success;
}

bool output(settings params)
{
	/*
	out.open(params.dst_path, ios::out);
	//log.open(params.log_path, ios::out);

	int i = 0;
	ofstream out_error;
	out_error.open("error.txt", ios::in);
	out_error << "������" << "\t" << "���Ƿ�" << "\t" << "����" << "\t" << "�к�" << "\t" << "�к�" << endl;


	while (i < word_num) {
		//szl//cout << word_stream[i].id << "\t" << word_stream[i].symbol << "\t" << word_stream[i].name << "\t" << word_stream[i].rnum << "\t" << endl;
		i++;
	}
	if (end_sign == 1) {
		//szl//cout << "end of file!" << endl;
		for (int i = 0; i < lexicalError; i++)
			out_error << error_message[i].id << "\t" << error_message[i].symbol << " " << error_message[i].name << " ��" << error_message[i].rnum << "�У���" << error_message[i].cnum << "��"
			<< " ����׶Σ��ʷ�����" << endl;
		out_error << "����" << lexicalError << "������" << endl;
	}
	out_error.close();


	*/
	out.open(params.dst_path, ios::out);
	if (!error_stream.empty()) return true;
	for (int i = 0; i < (int)code_stream.size(); i++)
	{
		out << code_stream[i] << endl;
	}
	return true;
}
