#include "compiler_design_qt.h"
#include <QtWidgets/QApplication>
#include "CodeEditor.h"
#include "head.h"

//全局变量
ifstream in;  //源代码输入流
ofstream out; //目标代码输出流
ofstream log_file; //日志输出流


vector<word>word_stream;//记号流
vector<string>code_stream;//生成代码流
vector<error_item>error_stream;//错误流


int a, b[10][10];

//错误流

//void process()
//{
//	lexical_analysis(); //词法分析
//	grammatical_analysis();//语法制导翻译
//
//}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    compiler_design_qt w;
    w.show();
    return app.exec();

	//settings params = preprocess(argc, argv);  //参数处理，根据用户输入的参数确定输入和输出方式

	//if (input(params))  //源代码读入
	//{
	//	process(); //处理函数
	//	output(params);  //目标代码和日志输出

	//	test_code_generation();
	//}
}
