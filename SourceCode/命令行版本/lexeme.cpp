/*
	错误文件在主程序中进行打开和关闭
	category表示记号类别
	sub_category表示记号子类
	content表示记号内容
	row为行号
	col为列号 回车算到下一行0列
	空格,\t,\n统一为 pointless无意义符号
	end_sign为文件结束标记 1表示结束
*/
#include "head.h"

char cur_c;                     // 记录当前读入的字符 
int cur_col = 0;                // 记录当前列数  
int cur_row = 1;                // 记录当前行数  
int cur_character = 0;          // 记录当前字符数  
int cur_word = 0;               // 记录当前单词数  
string cur_category;            // 记录记号类别 
string cur_sub_category;        // 记录记号子类 
string cur_content;             // 记录记号内容 

int end_sign = 0;			// 程序结束标志，1表示结束（遇到EOF） 
int is_int = 1;				//判断数是否问整数 

int modt = 0;				//处理双点数 
int break_sign = 0;			//处理双点数 

// 利用string数组存储关键字  
string KEY[] = {
	"program",   "const",   "var",   "procedure",   "function",
	"begin",   "end",   "array",   "of",   "integer",
	"real",   "boolean",   "char",   "if",   "then",
	"else",   "for",   "to",   "do",   "read",
	"write",   "or",   "div",   "mod",   "and",
	"not",   "string", "true",	"false", "while",
	"type",  "record"
};
// 将关键字数组存贮到全局变量vector中便于查询  
vector<string> Key(KEY, KEY + sizeof(KEY) / sizeof(KEY[0]));

void lexical_analysis()
{
	cur_c = in.get();

	while (end_sign != 1) {
		analysis();
	}

	lexical_output();
}

/*
	词法分析函数：进行文件分析
*/
void analysis()
{
	string d_str;// 存储双点前的数 
	string str;       // 创建储存当前单词的对象  
	str.clear();
	string error_str;
	error_str.clear();
	modt = 0;
	if (!in.is_open()) {    // 如果文件未正常打开，及时报错  
		cout << "打开文件失败！" << endl;
	}
	else {
		if (cur_c != EOF) {
			switch (cur_c)
			{
			case 'A':   case 'B':   case 'C':   case 'D':
			case 'E':   case 'F':   case 'G':   case 'H':   case 'I':   case 'J':
			case 'K':   case 'L':   case 'M':   case 'N':   case 'O':   case 'P':
			case 'Q':   case 'R':   case 'S':   case 'T':   case 'U':   case 'V':
			case 'W':   case 'X':   case 'Y':   case 'Z':
				cur_c = cur_c + 32;
			case 'a':   case 'b':   case 'c':   case 'd':   case 'e':   case 'f':
			case 'g':   case 'h':   case 'i':   case 'j':   case 'k':   case 'l':
			case 'm':   case 'n':   case 'o':   case 'p':   case 'q':   case 'r':
			case 's':   case 't':   case 'u':   case 'v':   case 'w':   case 'x':
			case 'y':   case 'z':
			case '_':
				cur_word++;
				while (isalpha(cur_c) || isdigit(cur_c) || cur_c == '_') {
					if (isalpha(cur_c) && cur_c >= 'A' && cur_c <= 'Z')
						cur_c += 32;
					error_str += cur_c;
					str += cur_c;
					get_char();
				}
				if (cur_c == '~' || cur_c == '@' || cur_c == '$' || cur_c == '?') {//处理变量命名错误  含非法字符 
					int tag = cur_col;
					while (isalpha(cur_c) || isdigit(cur_c) || cur_c == '_' || cur_c == '~' || cur_c == '@' || cur_c == '$' || cur_c == '?') {
						error_str += cur_c;
						if (isalpha(cur_c) || isdigit(cur_c))
							str += cur_c;
						get_char();
					}
					cur_category = "ERROR";
					cur_sub_category = "wrong id";
					cur_content = error_str;
					record_error();
					error_str.clear();
				}
				if (is_key(str)) {
					cur_category = "RESERVED";
					cur_sub_category = "KEY_" + str;
					cur_content = str;
					if (str == "program") record_word(101);
					else if (str == "const") record_word(102);
					else if (str == "var") record_word(103);
					else if (str == "procedure") record_word(104);
					else if (str == "function") record_word(105);
					else if (str == "begin") record_word(106);
					else if (str == "end") record_word(107);
					else if (str == "array") record_word(108);
					else if (str == "of") record_word(109);
					else if (str == "integer") record_word(110);
					else if (str == "real") record_word(111);
					else if (str == "boolean") record_word(112);
					else if (str == "char") record_word(113);
					else if (str == "if") record_word(114);
					else if (str == "then") record_word(115);
					else if (str == "else") record_word(116);
					else if (str == "for") record_word(117);
					else if (str == "to") record_word(118);
					else if (str == "do") record_word(119);
					else if (str == "read") record_word(120);
					else if (str == "write") record_word(121);
					else if (str == "or") record_word(122);
					else if (str == "div") record_word(123);
					else if (str == "mod") record_word(124);
					else if (str == "and") record_word(125);
					else if (str == "not") record_word(126);
					else if (str == "string") record_word(127);
					else if (str == "true") record_word(128);
					else if (str == "false") record_word(129);
					else if (str == "while") record_word(130);
					else if (str == "type") record_word(131);
					else if (str == "record") record_word(132);
				}
				else {
					//int i = isId(str);
					cur_category = "ID";
					cur_sub_category = "ID";
					cur_content = str;
					record_word(1);
				}
				str.clear();
				break;
			case '0':   case '1':   case '2':
			case '3':   case '4':   case '5':
			case '6':   case '7':   case '8':
			case '9':
				is_int = 1;
				cur_word++;
				while (isdigit(cur_c) || cur_c == '.' || cur_c == 'e' || cur_c == 'E') {
					if (cur_c == '.' && modt == 0) {
						modt = 1;
						d_str = str;
					}
					else if (cur_c == '.' && modt == 1) {
						str.clear();
						get_char();
						/*while (isdigit(cur_c)) {
							str += cur_c;
							get_char();
						}
						cur_category = "INT";*/
						cur_sub_category = "INT";
						cur_content = d_str;
						record_word(2);
						cur_category = "PUN";
						cur_sub_category = "PUN_ddot";
						cur_content = "..";
						record_word(312);
						/*cur_category = "INT";
						cur_sub_category = "INT";
						cur_content = str;
						record_word(2);*/
						break_sign = 1;
						break;
					}
					str += cur_c;
					if (!isdigit(cur_c))
						is_int = 0;
					if (cur_c == 'e' || cur_c == 'E') {
						get_char();
						str += cur_c;
					}
					get_char();
				}
				if (break_sign == 1) {
					break_sign = 0;
					break;
				}
				if (isalpha(cur_c)) {//1A1 处理数字中出现字母字符的错误，或是以数字开头的变量名错误 
					cur_category = "ERROR";
					cur_sub_category = "wrong id";
					cur_content = str;
					record_error();
					str.clear();
					break;
				}
				if (is_int == 0) {
					cur_category = "REL";
					cur_sub_category = "REL";
					cur_content = str;
					record_word(3);
				}
				else {
					cur_category = "INT";
					cur_sub_category = "INT";
					cur_content = str;
					record_word(2);
				}
				str.clear();
				break;
			case '>':
				get_char();
				if (cur_c == '=') {
					cur_category = "OPERATOR";
					cur_sub_category = "OPR_gequal";
					cur_content = ">=";
					record_word(207);
				}
				else {
					cur_category = "OPERATOR";
					cur_sub_category = "OPR_great";
					cur_content = ">";
					record_word(205);
					break;
				}
				get_char();
				break;
			case '<':
				get_char();
				if (cur_c == '=') {
					cur_category = "OPERATOR";
					cur_sub_category = "OPR_lequal";
					cur_content = "<=";
					record_word(208);
				}

				else if (cur_c == '>') {
					cur_category = "OPERATOR";
					cur_sub_category = "OPR_nequal";
					cur_content = "<>";
					record_word(209);
				}

				else {
					cur_category = "OPERATOR";
					cur_sub_category = "OPR_less";
					cur_content = "<";
					record_word(206);
					break;
				}
				get_char();
				break;
			case '=':
				get_char();
				cur_category = "OPERATOR";
				cur_sub_category = "OPR_equal";
				cur_content = "=";
				record_word(202);
				break;
			case '+':
				get_char();
				cur_category = "OPERATOR";
				cur_sub_category = "OPR_add";
				cur_content = "+";
				record_word(203);
				break;
			case '-':
				get_char();
				cur_category = "OPERATOR";
				cur_sub_category = "OPR_sub";
				cur_content = "-";
				record_word(204);
				break;
			case '*':
				get_char();
				cur_category = "OPERATOR";
				cur_sub_category = "OPR_mul";
				cur_content = "*";
				record_word(210);
				break;
			case '\'':
				str += cur_c;
				get_char();
				while (cur_c != '\'' && cur_c != '\n') {
				//while (isdigit(cur_c) || isalpha(cur_c)) {
					str += cur_c;
					get_char();
				}
				if (cur_c != '\'') {
					cur_category = "ERROR";
					cur_sub_category = "string end signal lost";
					cur_content = str;
					record_error();
					str += '\'';
				}
				if (cur_c == '\'') {
					str += cur_c;
					get_char();
				}

				if (str.length() > 3)//jn add
				{
					cur_category = "STRING";
					cur_sub_category = "string";
					cur_content = str;
					record_word(STR);
				}
				else
				{
					cur_category = "CHAR";
					cur_sub_category = "char";
					cur_content = str;
					record_word(CHR);
				}
				str.clear();
				break;
			case '\"':
				str += cur_c;
				get_char();
				while (cur_c != '\"' && cur_c != '\n') {
				//while (isdigit(cur_c) || isalpha(cur_c)) {
					str += cur_c;
					get_char();
				}
				if (cur_c != '\"') {
					cur_category = "ERROR";
					cur_sub_category = "string end signal lost";
					cur_content = str;
					record_error();
					str += '\"';
				}
				if (cur_c == '\"') {
					str += cur_c;
					get_char();
				}
				cur_category = "STRING";
				cur_sub_category = "string";
				cur_content = str;
				record_word(5);
				str.clear();
				break;
			case '/':
				get_char();
				cur_category = "OPERATOR";
				cur_sub_category = "OPR_div";
				cur_content = "/";
				record_word(211);
				break;
			case ':':
				get_char();
				if (cur_c == '=') {
					cur_category = "OPERATOR";
					cur_sub_category = "OPR_assign";
					cur_content = ":=";
					record_word(201);
				}
				else {
					cur_category = "PUNCTUATION";
					cur_sub_category = "PUN_colon";
					cur_content = ":";
					record_word(306);
					break;
				}
				get_char();
				break;
			case '(':
				get_char();
				cur_category = "PUNCTUATION";
				cur_sub_category = "PUN_lbrkt";
				cur_content = "(";
				record_word(301);
				break;
			case ')':
				get_char();
				cur_category = "PUNCTUATION";
				cur_sub_category = "PUN_rbrkt";
				cur_content = ")";
				record_word(302);
				break;
			case ',':
				get_char();
				cur_category = "PUNCTUATION";
				cur_sub_category = "PUN_comma";
				cur_content = ",";
				record_word(303);
				break;
			case ';':
				get_char();
				cur_category = "PUNCTUATION";
				cur_sub_category = "PUN_semicolon";
				cur_content = ";";
				record_word(304);
				break;
			case '.':
				get_char();
				if (cur_c == '.') {
					cur_category = "PUNCTUATION";
					cur_sub_category = "PUN_ddot";
					cur_content = "..";
					record_word(312);
				}
				else {
					cur_category = "PUNCTUATION";
					cur_sub_category = "PUN_dot";
					cur_content = ".";
					record_word(305);
					break;
				}
				get_char();
				break;
			case '[':
				get_char();
				cur_category = "PUNCTUATION";
				cur_sub_category = "PUN_lsbrkt";
				cur_content = "[";
				record_word(308);
				break;
			case ']':
				get_char();
				cur_category = "PUNCTUATION";
				cur_sub_category = "PUN_rsbrkt";
				cur_content = "]";
				record_word(309);
				break;
			case '{':
				get_char();
				str += cur_c;
				get_char();
				while (cur_c != '}' && cur_c != EOF) {
					if (cur_c == '\n') {
						cur_row++;
						cur_col = 0;
					}
					str += cur_c;
					get_char();
				}
				if (cur_c == EOF) {
					cur_category = "ERROR";
					cur_sub_category = "comment end signal lost";
					cur_content = "notes";
					end_sign = 1;
					record_error();
					break;
				}
				str += cur_c;
				cur_category = "NOTES";
				cur_sub_category = "notes";
				cur_content = "notes";
				str.clear();
				get_char();
				break;
			case ' ':
			case '\t':
				get_char();
				cur_category = "POINTLESS";
				cur_sub_category = "pointless";
				cur_content = "pointless";
				break;
			case '\n':
				get_char();
				cur_category = "POINTLESS";
				cur_sub_category = "pointless";
				cur_content = "pointless";
				cur_row++;
				cur_col = 0;
				break;
			default:
				cur_category = "ERROR";
				cur_sub_category = "unkonwn char";
				cur_content = cur_c;
				record_error();
				get_char();
				break;
			}
		}
		else {
			end_sign = 1;
			in.close();
		}

	}
}

/*
	读取字符函数：读取下一个字符
*/
void get_char()
{
	cur_character++;
	cur_c = in.get();
	cur_col++;
}

/*
	关键字函数：判断识别出来的单词是否是关键字
*/
bool is_key(string word)
{
	for (int i = 0; i < (int)Key.size(); i++) {
		if (Key[i] == word)
			return true;
	}
	return false;
}



void record_word(int record_id)
{
	word tmp;
	tmp.id = record_id;
	tmp.symbol = cur_sub_category;
	tmp.value = cur_content;
	tmp.col = cur_col;
	tmp.row = cur_row;
	word_stream.push_back(tmp);

}

void record_error()
{
	string error_signal;
	if (cur_sub_category == "comment end signal lost")
		error_signal = "";
	else error_signal = "'" + cur_content + "'";
	append_error(LEXEME, cur_sub_category, cur_row, cur_col, error_signal);
}




void lexical_output()//这是一个测试函数
{
	int i = 0;
	ofstream out_error;
	out_error.open("error.txt", ios::out);
	out_error << "类别编码" << "\t" << "助记符" << "\t" << "单词" << "\t" << "行号" << "\t" << "列号" << endl;


	while (i < word_stream.size()) {
		//cout << setw(15) << word_stream[i].id << setw(15) << word_stream[i].symbol << setw(15) << word_stream[i].value << setw(15) << word_stream[i].row << setw(15) << endl;
		i++;
	}

	//cout << "AAAAAAAAAAA1";


	for (int i = 0; i < error_stream.size(); i++)
		out_error << error_stream[i].error_code << " " << error_stream[i].content << " 第" << error_stream[i].row << "行，第" << error_stream[i].col << "列"
		<< " 错误阶段：词法分析" << endl;
	out_error << "共有" << error_stream.size() << "处错误。" << endl;


	out_error.close();
}

void init_lexeme()
{
	cur_c;                     // 记录当前读入的字符 
	cur_col = 0;                // 记录当前列数  
	cur_row = 1;                // 记录当前行数  
	cur_character = 0;          // 记录当前字符数  
	cur_word = 0;               // 记录当前单词数  
	cur_category.clear();            // 记录记号类别 
	cur_sub_category.clear();        // 记录记号子类 
	cur_content.clear();             // 记录记号内容 

	end_sign = 0;			// 程序结束标志，1表示结束（遇到eof） 
	is_int = 1;				//判断数是否问整数 

	modt = 0;				//处理双点数 
	break_sign = 0;			//处理双点数 
}

