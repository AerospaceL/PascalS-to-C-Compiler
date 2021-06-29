/*
	�����ļ����������н��д򿪺͹ر�
	category��ʾ�Ǻ����
	sub_category��ʾ�Ǻ�����
	content��ʾ�Ǻ�����
	rowΪ�к�
	colΪ�к� �س��㵽��һ��0��
	�ո�,\t,\nͳһΪ pointless���������
	end_signΪ�ļ�������� 1��ʾ����
*/
#include "head.h"

char cur_c;                     // ��¼��ǰ������ַ� 
int cur_col = 0;                // ��¼��ǰ����  
int cur_row = 1;                // ��¼��ǰ����  
int cur_character = 0;          // ��¼��ǰ�ַ���  
int cur_word = 0;               // ��¼��ǰ������  
string cur_category;            // ��¼�Ǻ���� 
string cur_sub_category;        // ��¼�Ǻ����� 
string cur_content;             // ��¼�Ǻ����� 

int end_sign = 0;			// ���������־��1��ʾ����������EOF�� 
int is_int = 1;				//�ж����Ƿ������� 

int modt = 0;				//����˫���� 
int break_sign = 0;			//����˫���� 

// ����string����洢�ؼ���  
string KEY[] = {
	"program",   "const",   "var",   "procedure",   "function",
	"begin",   "end",   "array",   "of",   "integer",
	"real",   "boolean",   "char",   "if",   "then",
	"else",   "for",   "to",   "do",   "read",
	"write",   "or",   "div",   "mod",   "and",
	"not",   "string", "true",	"false", "while",
	"type",  "record"
};
// ���ؼ������������ȫ�ֱ���vector�б��ڲ�ѯ  
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
	�ʷ����������������ļ�����
*/
void analysis()
{
	string d_str;// �洢˫��ǰ���� 
	string str;       // �������浱ǰ���ʵĶ���  
	str.clear();
	string error_str;
	error_str.clear();
	modt = 0;
	if (!in.is_open()) {    // ����ļ�δ�����򿪣���ʱ����  
		cout << "���ļ�ʧ�ܣ�" << endl;
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
				if (cur_c == '~' || cur_c == '@' || cur_c == '$' || cur_c == '?') {//���������������  ���Ƿ��ַ� 
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
				if (isalpha(cur_c)) {//1A1 ���������г�����ĸ�ַ��Ĵ��󣬻��������ֿ�ͷ�ı��������� 
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
	��ȡ�ַ���������ȡ��һ���ַ�
*/
void get_char()
{
	cur_character++;
	cur_c = in.get();
	cur_col++;
}

/*
	�ؼ��ֺ������ж�ʶ������ĵ����Ƿ��ǹؼ���
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




void lexical_output()//����һ�����Ժ���
{
	int i = 0;
	ofstream out_error;
	out_error.open("error.txt", ios::out);
	out_error << "������" << "\t" << "���Ƿ�" << "\t" << "����" << "\t" << "�к�" << "\t" << "�к�" << endl;


	while (i < word_stream.size()) {
		//cout << setw(15) << word_stream[i].id << setw(15) << word_stream[i].symbol << setw(15) << word_stream[i].value << setw(15) << word_stream[i].row << setw(15) << endl;
		i++;
	}

	//cout << "AAAAAAAAAAA1";


	for (int i = 0; i < error_stream.size(); i++)
		out_error << error_stream[i].error_code << " " << error_stream[i].content << " ��" << error_stream[i].row << "�У���" << error_stream[i].col << "��"
		<< " ����׶Σ��ʷ�����" << endl;
	out_error << "����" << error_stream.size() << "������" << endl;


	out_error.close();
}

void init_lexeme()
{
	cur_c;                     // ��¼��ǰ������ַ� 
	cur_col = 0;                // ��¼��ǰ����  
	cur_row = 1;                // ��¼��ǰ����  
	cur_character = 0;          // ��¼��ǰ�ַ���  
	cur_word = 0;               // ��¼��ǰ������  
	cur_category.clear();            // ��¼�Ǻ���� 
	cur_sub_category.clear();        // ��¼�Ǻ����� 
	cur_content.clear();             // ��¼�Ǻ����� 

	end_sign = 0;			// ���������־��1��ʾ����������eof�� 
	is_int = 1;				//�ж����Ƿ������� 

	modt = 0;				//����˫���� 
	break_sign = 0;			//����˫���� 
}

