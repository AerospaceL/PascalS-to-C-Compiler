#include "head.h"

void init()
{
	//公共流清空
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
	//这里为了测试方便暂时将输入、输出写死
	//之后应该根据用户输入的参数设置settings

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
		//szl//cout << "打开文件失败。" << endl;
		success = false;
	}
	else
	{
		//szl//cout << "打开文件成功！" << endl;
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
	out_error << "类别编码" << "\t" << "助记符" << "\t" << "单词" << "\t" << "行号" << "\t" << "列号" << endl;


	while (i < word_num) {
		//szl//cout << word_stream[i].id << "\t" << word_stream[i].symbol << "\t" << word_stream[i].name << "\t" << word_stream[i].rnum << "\t" << endl;
		i++;
	}
	if (end_sign == 1) {
		//szl//cout << "end of file!" << endl;
		for (int i = 0; i < lexicalError; i++)
			out_error << error_message[i].id << "\t" << error_message[i].symbol << " " << error_message[i].name << " 第" << error_message[i].rnum << "行，第" << error_message[i].cnum << "列"
			<< " 错误阶段：词法分析" << endl;
		out_error << "共有" << lexicalError << "处错误。" << endl;
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
