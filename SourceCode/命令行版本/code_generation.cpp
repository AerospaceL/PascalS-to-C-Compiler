#include "head.h"

int tab_num = 0;
int var_flag = 0;

//向code_stream添加一行新语句

string temp_line = "";

void set_var_flag(int flag)
{
	var_flag = flag;
}

int get_var_flag()
{
	return var_flag;
}

void append_partial_code_line(string partial_code_line)
{
	temp_line += partial_code_line;
}

string get_temp_line()
{
	return temp_line;
}

void clean_temp_line()
{
	temp_line = "";
}

void append_code_line(string code_line)
{
	string tab_str = "";
	for (int i = 0; i < tab_num; i++)
	{
		tab_str += "    ";
	}
	code_line = tab_str + code_line;
	code_stream.push_back(code_line);
	////szl//cout << code_line <<endl;
}

void tab_change(int tab_change)
{
	tab_num += tab_change;
}

//---------------------------------XR 2021 05 03-------------------------------
//取得tab_num
int get_tab_num()
{
	return tab_num;
}


//输出生成的全部语句，测试用函数
void test_code_generation()
{
	if (!error_stream.empty()) return;
	for (int i = 0; i < (int)code_stream.size(); i++)
	{
		cout<<code_stream[i]<<endl;
	}

}


string get_c_type(int basic_type)
{
	if (basic_type == INTEGER) {
		return string("int");
	}
	else if (basic_type == REAL) {
		return string("double");
	}
	else if (basic_type == CHAR) {
		return string("char");
	}
	else if (basic_type == BOOLEAN) {
		return string("boolean");
	}
	else if (basic_type == VOID) {
		return string("void");
	}
	else if (basic_type == STRR) {
		return string("char*");
	}
	else
	{
		return "";
	}
}


void init_code_generation()
{
	tab_num = 0;
	temp_line = "";
}
