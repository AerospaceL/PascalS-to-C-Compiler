//通用模块
#ifndef _UTILS_
#define _UTILS_
#include "head.h"
using namespace std;

typedef struct {
	string scr_path;  //源代码路径
	string dst_path;  //目标代码路径
	string log_path;  //日志路径
	bool file_output;  //是否提供文件输出
}settings;

settings preprocess(int argc, char* argv[]);  //参数处理，根据用户输入的参数确定输入和输出方式
bool input(settings params);  //源代码读入
bool output(settings params);  //目标代码和日志输出

void init();
#endif