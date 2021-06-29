#include "head.h"

vector <symbol_list_item> symbol_list;	//ջʽ���ű�

vector <int> block_index_list;	//�������� 

int hash_table[HASH_SIZE]; 	//ɢ�б�

map<string, record_member_list_> all_record_types;	//����record����


void push_symbol_table_item(string name, sem_item item, bool tab_flag, int cursor) {
	if (tab_flag) {
		locate_symbol_table();
	}
	else {
		// TO THINK: ʲôҲ������?
	}

	symbol_list_item tmp;
	tmp.name = name;
	tmp.item_attri = item;
	tmp.next_chain_item_index = -1;

	if (item.type == RECORDD) {
		if (all_record_types.find(item.attri_record.record_type_name) != all_record_types.end()) {
			int hash_index = string_hash(name);
			if (hash_table[hash_index] == -1) {	//����޳�ͻ��ֱ��ѹջ������ɢ�б�
				hash_table[hash_index] = symbol_list.size();
				symbol_list.push_back(tmp);	//���ű�����±���
			}
			else {	//����г�ͻ
				if (not_redu_name(name, cursor)) {	//�������
					tmp.next_chain_item_index = hash_table[hash_index];	//����ԭ��
					hash_table[hash_index] = symbol_list.size();	//������ͷ
					symbol_list.push_back(tmp);	//���ű�����±���
				}
				else {
					append_error(SEMANTEME, "'" + name + "'", word_stream[cursor].row, word_stream[cursor].col, "conflict types for");
				}
			}
		}
		else {
			// "unknown type name '" + item.attri_record.record_type_name + "' for member '" + record_info.at(i).member_name + "' of record");
			append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "unknown type name '" + item.attri_record.record_type_name + "' for");
		}
	}
	else {
		int hash_index = string_hash(name);
		if (hash_table[hash_index] == -1) {	//����޳�ͻ��ֱ��ѹջ������ɢ�б�
			hash_table[hash_index] = symbol_list.size();
			symbol_list.push_back(tmp);	//���ű�����±���
		}
		else {	//����г�ͻ
			if (not_redu_name(name, cursor)) {	//�������
				tmp.next_chain_item_index = hash_table[hash_index];	//����ԭ��
				hash_table[hash_index] = symbol_list.size();	//������ͷ
				symbol_list.push_back(tmp);	//���ű�����±���
			}
			else {
				append_error(SEMANTEME, "'" + name + "'", word_stream[cursor].row, word_stream[cursor].col, "conflict types for");
			}
		}
	}
}

int search_symbol_table(string name) {
	name = split(name, "[").at(0);	//�ָ���ܴ��ڵ�[]
	int hash_index = string_hash(name);
	if (hash_table[hash_index] == -1) {	//Ϊ��
		return -1;
	}
	else {
		int table_index = hash_table[hash_index];
		do {
			if (symbol_list.size() > 0) {//szl
				if (name == symbol_list.at(table_index).name) {
					return table_index;
				}
				table_index = symbol_list.at(table_index).next_chain_item_index;
			}
		} while (table_index != -1);
		return -1;
	}
	// �򵥵Ĳ��԰汾
	//for (int i = symbol_list.size() - 1; i >= 0; i--) {
	//	if (name == symbol_list.at(i).name) {
	//		return i;
	//	}
	//}
	//return -1;
}

void locate_symbol_table() {
	block_index_list.push_back(symbol_list.size());	//�����������
}

void relocate_symbol_table() {
	test_print_all();

	int top = block_index_list.back();
	if (block_index_list.size() > 0) {//szl
		while (symbol_list.size() != top + 1) {
			hash_table[string_hash(symbol_list.back().name)] = symbol_list.back().next_chain_item_index;	//���߼���ժ��
			symbol_list.pop_back();	//��������ժ��
		}
	}
	block_index_list.pop_back();

	// �򵥵Ĳ��԰汾
	//int top = block_index_list.back();
	//while (symbol_list.size() != top + 1) {
	//	symbol_list.pop_back();
	//}
	//block_index_list.pop_back();
}

bool not_redu_name(string name, int cursor) {
	int hash_index = string_hash(name);
	if (block_index_list.size() > 0) {//szl
		while (hash_table[hash_index] >= block_index_list.back() && hash_index != -1) {
			if (name == symbol_list.at(hash_table[hash_index]).name) {
				return false;
			}
			hash_index = symbol_list.at(hash_table[hash_index]).next_chain_item_index;
		}
	}
	return true;

	//�򵥵Ĳ��԰汾
	//for (int i = symbol_list.size() - 1; i >= block_index_list.at(block_index_list.size()-1); i--) {
	//	if (name == symbol_list.at(i).name) {
	//		////szl//cout << Xsplit << "������� " << "����������" << "at row:col " << word_stream[cursor].row << word_stream[cursor].col << endl << Xsplit;
	//		return false;
	//	}
	//}
	//return true;
}

int judge_lr_expression_type(int left, int right, string opr, int cursor) {
	if (left >= 0 && right >= 0) {
		if (left == MERVEILLEUS || right == MERVEILLEUS) {
			return MERVEILLEUS;
		}
		else if (opr == "+" || opr == "-" || opr == "*") {
			if (left == INTEGER && right == INTEGER)	//����������Ϊ����
				return INTEGER;
			else if ((left == REAL || left == INTEGER) && (right == REAL || right == INTEGER))	//�и����Ҷ�����ֵ�Ļ�������Ǹ���
				return REAL;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//�������Ͳ������Ӽ���
			}
		}
		else if (opr == "/") {
			if ((left == REAL || left == INTEGER) && (right == REAL || right == INTEGER))	//������ֵ���ܳ�������REAL
				return REAL;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//�������Ͳ���
			}
		}
		else if (opr == "div") {
			if (left == INTEGER && right == INTEGER)	//����int����div������int
				return INTEGER;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//�������Ͳ���
			}
		}
		else if (opr == ">" || opr == "<" || opr == ">=" || opr == "<=" || opr == "==" || opr == "!=") {
			if ((left == INTEGER && right == REAL) || (left == REAL && right == INTEGER))	//��ֵ���ԱȽ�
				return BOOLEAN;
			else if (left == right)	//�����������һ���Ļ�Ҳ���ԱȽ�
				return BOOLEAN;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//���Ͳ�һ���Ҳ�������ֵ�Ͳ��ܱȽ�
			}
		}
		else if (opr == "||" || opr == "&&") {
			if (left == BOOLEAN && right == BOOLEAN)	//ֻ������bool���ܻ�/������
				return BOOLEAN;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//���౨��
			}
		}
		else if (opr == "%") {
			if (left == INTEGER && right == INTEGER)	//ֻ������int����ȡ������
				return INTEGER;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//���౨��
			}
		}
	}
	else {
		return -1;	//left��right�и�����˵��Ҫ����д���ˣ�Ҫ���ӱ��ʽ�Ѿ��������⡣
	}
}

int judge_arrary_range(string name, expression_type_list_ expression_type_list_val, int cursor) {
	//return -1;	//�������
	int index = search_symbol_table(name);
	if (index != -1) {
		if (symbol_list.size() > 0) {
			if (symbol_list.at(index).item_attri.type == ARRAY_VAR) {
				if (expression_type_list_val.size() == symbol_list.at(index).item_attri.attri_array_var.dimension.size()) {
					for (int i = 0; i < expression_type_list_val.size(); i++) {
						if (expression_type_list_val.at(i) != INTEGER && expression_type_list_val.at(i) != MERVEILLEUS) {
							append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "array subscript is not an integer");
							break;
							return -1;
						}
					}
					if (symbol_list.at(index).item_attri.attri_array_var.array_typee != RECORD)
						return symbol_list.at(index).item_attri.attri_array_var.array_typee;
					else
						return MERVEILLEUS;
				}
				else {
					string error_msg = "subscripted value '" + name + "' is not an array";
					append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
					return -1;
				}
			}

			else {
				string error_msg = "subscripted value '" + name + "' is not an array";
				append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
				return -1;
			}
		}
	}
	else {
		string error_msg = "'" + name + "'" + " undeclared";
		append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
		return -1;
	}
}

int judge_var(string name, int cursor) {
	//return -1;	//�������
	int index = search_symbol_table(name);
	if (index != -1) {
		if (symbol_list.size() > 0) {
			if (symbol_list.at(index).item_attri.type == CONST)
				return symbol_list.at(index).item_attri.attri_const.typee;
			else if (symbol_list.at(index).item_attri.type == BASIC_VAR)
				return symbol_list.at(index).item_attri.attri_basic_var.typee;
			else if (symbol_list.at(index).item_attri.type == FUNCTION)
				return symbol_list.at(index).item_attri.attri_function.return_type;
			else if (symbol_list.at(index).item_attri.type == RECORDD)
				return MERVEILLEUS;
			else {
				string error_msg = "'" + name + "' type error";
				append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
				return -1;
			}
		}
	}
	else {
		string error_msg = "'" + name + "' undeclared";
		append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
		return -1;
	}
}

void judge_assign_type(string name, int variable_type, int  expression_type, int cursor) {
	int index = search_symbol_table(name);
	if (index != -1) {	// ��˵����ʱ��֤�˴��� ��Ϊ��֤��׳�ԣ����������ж�
		if (symbol_list.size() > 0) {
			if (symbol_list.at(index).item_attri.type != CONST) {	//������ǳ������ܸ�ֵ
				//�жϸ�ֵ����
				if (variable_type != expression_type) {	//�������Ͳ�һ��
					if (variable_type == MERVEILLEUS || expression_type == MERVEILLEUS) {
						// �������record���µ�MERVEILLEUS������Ϊ���Ը�ֵ���������������ʱ������
					}
					else if (variable_type == REAL && expression_type == INTEGER) {
						// �����int��ֵ��real�����Ը�ֵ
					}
					else if (expression_type < 0) {
						string vt = get_c_type(variable_type);
						
						string error_msg = "assignment to " + vt + " from someting have no value";
						append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
					}
					else {	//���಻�ܸ�ֵ
						string vt = get_c_type(variable_type);
						string et = get_c_type(expression_type);
						string error_msg = "assignment to " + vt + " from " + et + " without a cast";
						append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
						
					}

				}
			}
			else {
				append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "can not assign for const value");
			}
		}
	}
}

int judge_func_and_proc(string name, expression_type_list_ expression_type_list_val, int cursor) {
	int index = search_symbol_table(name);
	int return_ = -1;
	if (index != -1) {
		if (symbol_list.size() > 0) {
			if (symbol_list.at(index).item_attri.type == FUNCTION) {
				if (symbol_list.at(index).item_attri.attri_function.para_type_list.size() == expression_type_list_val.size()) {
					for (int i = 0; i < expression_type_list_val.size(); i++) {
						if (symbol_list.at(index).item_attri.attri_function.para_type_list.at(i) != expression_type_list_val.at(i)) {
							string atype = get_c_type(symbol_list.at(index).item_attri.attri_function.para_type_list.at(i));//szl

							string etype = get_c_type(expression_type_list_val.at(i));
							string error_msg = "passing argument " + to_string(i + 1) + " of " + name + " makes " + atype + " from " + etype + " without a cast";
							append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
							break;
						}
					}
					return_ = symbol_list.at(index).item_attri.attri_function.return_type;
				}
				else {
					append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "argument number wrong of");
				}
			}
			else if (symbol_list.at(index).item_attri.type == PROCEDURE) {
				if (symbol_list.at(index).item_attri.attri_procedure.para_type_list.size() == expression_type_list_val.size()) {
					for (int i = 0; i < expression_type_list_val.size(); i++) {
						if (symbol_list.at(index).item_attri.attri_procedure.para_type_list.at(i) != expression_type_list_val.at(i)) {
							string atype = get_c_type(symbol_list.at(index).item_attri.attri_procedure.para_type_list.at(i));//szl

							string etype = get_c_type(expression_type_list_val.at(i));
							string error_msg = "passing argument " + to_string(i + 1) + " of " + name + " makes " + atype + " from " + etype + " without a cast";
							append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
						}
					}
				}
				else {
					append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "argument number wrong of");
				}
			}
			else {
				string error_msg = "'" + name + "'" + " is not a function or procedure";
				append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
			}
		}
	}
	else {
		string error_msg = "'" + name + "' undeclared";
		append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
	}
	return return_;
}

void judge_if_expression(int expression_type, int cursor) {
	if (expression_type != BOOLEAN && expression_type != MERVEILLEUS) {
		append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "branch condition must be boolean");
	}
}

void judge_while_expression(int expression_type, int cursor) {
	if (expression_type != BOOLEAN && expression_type != MERVEILLEUS) {
		append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "branch condition must be boolean");
	}
}

void judge_for_expression(string name, int from, int to, int cursor) {
	int index = search_symbol_table(name);
	if (index != -1) {
		if (symbol_list.size() > 0) {
			if (symbol_list.at(index).item_attri.type == INTEGER) {
				if ((from == INTEGER || from == MERVEILLEUS) && (to == INTEGER || to == MERVEILLEUS)) {
					// ��ȷ
				}
				else {
					append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "iterator range must from integer to integrt");
				}
			}
			else {
				append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "iterator must be integer");
			}
		}
	}
	else {
		append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "'" + name + "' undeclared");
	}
}

void judge_read_item(string name, int cursor) {
	int index = search_symbol_table(name);
	if (index != -1) {
		if (symbol_list.size() > 0) {
			if (symbol_list.at(index).item_attri.type != CONST) {	//������ǳ�������read��ֵ

			}
			else {
				append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "const value '" + name + "' can not be read");
			}
		}
	}
	else {
		//append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "�Ҳ�����ʶ��");
	}
}

pair<bool, int> judge_record(string name, int cursor) {
	bool return_ = true;
	record_member_list_ now_member_list;
	int i, j;
	int return_type = -1;

	vector<string> split_result = split(name, ".");
	int index = search_symbol_table(split_result[0]);

	if (index != -1) {
		if (symbol_list.size() > 0) {
			string record_type_name = symbol_list.at(index).item_attri.attri_record.record_type_name;
			if (all_record_types.find(record_type_name) != all_record_types.end()) {
				now_member_list = all_record_types[record_type_name];
				for (i = 1; i < split_result.size() && return_ == true; i++) {
					string now_member_name = split(split_result[i], "[")[0];
					bool is_array = false;
					if (split(split_result[i], "[").size() != 1)
						is_array = true;
					return_ = false;
					for (j = 0; j < now_member_list.size(); j++) {
						if (now_member_list.at(j).member_name == now_member_name) {
							return_ = true;
							break;
						}
					}
					if (return_ == true && now_member_list.at(j).member_type == RECORD) {
						if (all_record_types.find(now_member_list.at(j).record_type_name) != all_record_types.end()) {
							now_member_list = all_record_types[now_member_list.at(j).record_type_name];
						}
						else {
							append_error(SEMANTEME, now_member_list.at(j).record_type_name, word_stream[cursor].row, word_stream[cursor].col, "invalid record type:");
							return_ = false;
						}
					}
					//else if (return_ == true && now_member_list.at(j).member_type == ARRAY) {
					//	// ����˵��������
					//}
					else {
						if (i == split_result.size() - 1) {
							if (return_) {
								if (now_member_list.at(j).member_type == ARRAY) {
									if (is_basic_type(now_member_list.at(j).array_info.array_typee))
										return_type = now_member_list.at(j).array_info.array_typee;
									else
										return_type = MERVEILLEUS;
								}
								else {
									return_type = now_member_list.at(j).member_type;
								}
							}
							else {
								string tname = split(name, ".")[0];
								append_error(SEMANTEME, tname, word_stream[cursor].row, word_stream[cursor].col, "'" + now_member_name + "' is not a member of");
								return_ = false;
							}
						}
						else {
							if (return_) {
								if (is_array) {}
								else {
									append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "request for '" + now_member_name + "' in something not a record");
									return_ = false;
								}
							}
							else {
								string tname = split(name, ".")[0];
								append_error(SEMANTEME, tname, word_stream[cursor].row, word_stream[cursor].col, "'" + now_member_name + "' is not a member of");
								return_ = false;
							}
						}
					}
				}
			}
			else {
				//append_error(SEMANTEME, record_type_name, word_stream[cursor].row, word_stream[cursor].col, "unknown type name '"+ now_member_list.at(i).record_type_name+"' for member '"+ now_member_list.at(i).member_name+"' of record");
				//return_ = false;
			}
		}
	}
	else {
		append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "'" + split_result[0] + "' undeclared");
		return_ = false;
	}

	return make_pair(return_, return_type);

	//int index = search_symbol_table(split_result[0]);
	//if (index != -1) {
	//	string record_type_name = symbol_list.at(index).item_attri.attri_record.record_type_name;
	//	if (all_record_types.find(record_type_name) != all_record_types.end()) {
	//		return_ = MERVEILLEUS;
	//	}
	//	else {
	//		return_ = MERVEILLEUS;	//�������
	//		//append_error(SEMANTEME, record_type_name, word_stream[cursor].row, word_stream[cursor].col, "record���Ͳ�����");
	//	}
	//}
	//else {
	//	append_error(SEMANTEME, split_result[0], word_stream[cursor].row, word_stream[cursor].col, "�Ҳ�����ʶ��");
	//}
	//return return_;
}

unsigned int string_hash(string name) {
	// BKDRHash �㷨
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	char* str = (char*)name.data();
	while (*str)
	{
		//szl//cout << *str << " ";
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF) % 100;	//����0-100��BKDR��ϣֵ

	//�򵥵Ĳ��԰汾
	//if (name[0] <= 90 && name[0] >= 65)
	//	return name[0] - 65;
	//else
	//	return name[0] - 97;
}

vector<para_item_pair>split_para_line(string para_line) {
	vector<para_item_pair> result;
	para_item_pair one_pair;
	if (!para_line.empty()) {
		vector<string> split_result = split(para_line, ",");	//�ָ��������
		if (split_result.size() > 0) {
			for (int i = 0; i < split_result.size(); i++) {
				vector<string> one_split = split(trim_head(split_result.at(i)), " ");	//�ָ����ֺ�����
				if (one_split.size() > 0) {
					if (one_split.at(1)[0] == '&') {
						one_pair.ref_flag = true;
						one_pair.para_name = one_split.at(1).erase(0, 1);
					}
					else {
						one_pair.ref_flag = false;
						one_pair.para_name = one_split.at(1);
					}
					one_pair.para_type = get_se_gr_type(one_split.at(0));
					result.push_back(one_pair);
				}
			}
		}
	}

	return result;
}

string get_se_type(int type) {
	if (type == CONST) {
		return string("const");
	}
	else if (type == BASIC_VAR) {
		return string("basic var");
	}
	else if (type == ARRAY_VAR) {
		return string("array var");
	}
	else if (type == FUNCTION) {
		return string("function");
	}
	else if (type == PROCEDURE) {
		return string("procedure");
	}
	else {
		return "";
	}
}

int get_se_gr_type(string type) {
	if (type == "int")
		return INTEGER;
	else if (type == "double")
		return REAL;
	else if (type == "char")
		return CHAR;
	else if (type == "boolean")
		return BOOLEAN;
	else if (type == "void")
		return VOID;
	else if (type == "char*")
		return STRR;
	else if (type == "char *")
		return STRR;
	else if (type == "char* ")
		return STRR;
	else if (type == "char * ")
		return STRR;

	else
		return -1;
}

vector<string> split(string str, string pattern) {
	string::size_type pos;
	vector<string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

void trim(string& s) {
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
}

string trim_head(string s) {
	int index = 0;
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" "));
	}
	return s;
}

bool include_ch(string s, char ch) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ch)
			return true;
	}
	return false;
}

bool is_basic_type(int type) {
	switch (type) {
	case INTEGER:
	case REAL:
	case CHAR:
	case BOOLEAN:
	case STRR:
		return true;
	default:
		return false;
	}
}

bool replace_function_return(string name, int from_grammar_type, int cursor) {
	bool is_change = false;
	if (block_index_list.size() > 0) {//szl
		if (block_index_list.size() > 0 && symbol_list.size() > 0) {
			if (symbol_list.at(block_index_list.back()).item_attri.type == FUNCTION) {	//��������ں�����
				if (name == symbol_list.at(block_index_list.back()).name) {	//���ͬ��
					switch (from_grammar_type) {
					case PROCEDURE_CALL:
						append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "invalid expression��can not use function without () inside itself:");
						break;
					default:
						is_change = true;
					}
				}
			}
		}
	}
	return is_change;
}

void register_record_type(string record_type_name, record_member_list_ record_info, int cursor) {
	if (all_record_types.find(record_type_name) != all_record_types.end()) {
	append_error(SEMANTEME, "'"+record_type_name+"'", word_stream[cursor].row, word_stream[cursor].col, "conflict types for");//record
		//szl//cout << record_type_name << "�Ѷ���" << endl;
	}
	else {
		// ����Ա�����Ƿ񶼴���
		if (record_info.size() > 0) {
			for (int i = 0; i < record_info.size(); i++) {
				if (record_info.at(i).member_type == RECORD) {
					if (all_record_types.find(record_info.at(i).record_type_name) == all_record_types.end()) {
						append_error(SEMANTEME, record_type_name, word_stream[cursor].row, word_stream[cursor].col, "unknown type name '" + record_info.at(i).record_type_name + "' for member '" + record_info.at(i).member_name + "' of record");
						/*
							append_error(SEMANTEME, record_type_name, word_stream[cursor].row, word_stream[cursor].col, "record�����ض���");
							cout << record_type_name << "�Ѷ���" << endl;
						}
						else {
							// ����Ա�����Ƿ񶼴���
							for (int i = 0; i < record_info.size(); i++) {
								if (record_info.at(i).member_type == RECORD) {
									if (all_record_types.find(record_info.at(i).record_type_name) == all_record_types.end()) {
										append_error(SEMANTEME, record_info.at(i).member_name, word_stream[cursor].row, word_stream[cursor].col, "ʹ���˲����ڵ�record����");*/

										/*TO ��:
										record_type_name�ǵ�ǰ����������record��������
										record_info.at(i).member_name�ǵ�ǰ����������record���͵�ĳ����Ա����
										record_info.at(i).record_type_name�������Ա����������
										����һ�����ӣ�
										program err4_2(input,output);
										type r1=record
										x,y:integer;
										end;
										type r2=record
										x,y:r1;
										z:r3;{�����д�r3������Ͳ�����}
										end;
										var i:real;
										begin
										end.
										����Ϊ��Error type:3, line is:8, word is:z, error_msg:ʹ���˲����ڵ�record���͡�
										*/
					}
				}
			}
			if (all_record_types.insert(pair<string, record_member_list_>(record_type_name, record_info)).second) {
				//cout << "����ɹ�" << endl;
			}
		}
	}

}

void test_print_all() {
	//szl
	/*
	//szl//cout << "------------���ű�------------" << endl;
	if (!symbol_list.empty()) {
		for (int i = symbol_list.size() - 1; i >= 0; i--) {
			if (symbol_list.at(i).item_attri.type == CONST) {
				//szl//cout << i << "\t" + symbol_list.at(i).name + "\t" + "const: " + get_c_type(symbol_list.at(i).item_attri.attri_const.typee) << "\t" << symbol_list.at(i).next_chain_item_index << endl;
			}
			else if (symbol_list.at(i).item_attri.type == BASIC_VAR) {
				//szl//cout << i << "\t" + symbol_list.at(i).name + "\t" + "basic var: " + get_c_type(symbol_list.at(i).item_attri.attri_basic_var.typee) << "\t" << symbol_list.at(i).next_chain_item_index << endl;
			}
			else if (symbol_list.at(i).item_attri.type == ARRAY_VAR) {
				//szl//cout << i << "\t" + symbol_list.at(i).name + "\t" + "array var: " + get_c_type(symbol_list.at(i).item_attri.attri_array_var.array_typee) << "\t" << symbol_list.at(i).next_chain_item_index << endl;
			}
			else if (symbol_list.at(i).item_attri.type == FUNCTION) {
				//szl//cout << i << "\t" + symbol_list.at(i).name + "\t" + "function: " + get_c_type(symbol_list.at(i).item_attri.attri_function.return_type) << "\t" << symbol_list.at(i).next_chain_item_index << endl;
			}
			else if (symbol_list.at(i).item_attri.type == PROCEDURE) {
				//szl//cout << i << "\t" + symbol_list.at(i).name + "\t" + "procedure" << "\t" << symbol_list.at(i).next_chain_item_index << endl;
			}
			else if (symbol_list.at(i).item_attri.type == RECORDD) {
				//szl//cout << i << "\t" + symbol_list.at(i).name + "\t" + "record" << "\t" << symbol_list.at(i).next_chain_item_index << endl;
			}
		}
	}
	else
		//szl//cout << "------------��  ��------------" << endl;
	//szl//cout << "-----------------------------" << endl << endl;

	//szl//cout << "------------������------------" << endl;
	if (!block_index_list.empty()) {
		for (int i = block_index_list.size() - 1; i >= 0; i--) {
			//szl//cout << block_index_list.at(i) << endl;
		}
	}
	else
		//szl//cout << "------------��  ��------------" << endl;
	//szl//cout << "-----------------------------" << endl;
	*/
	////szl//cout << "------------ɢ�б�------------" << endl;
	//for (int i = 0; i < 26; i++)
	//	//szl//cout << hash_table[i] << endl;
	////szl//cout << "-----------------------------" << endl;
}

void init_semanteme() {
	// �����������������
	symbol_list.clear();
	block_index_list.clear();
	all_record_types.clear();

	block_index_list.push_back(0);
	for (int i = 0; i < HASH_SIZE; i++) {
		hash_table[i] = -1;
	}
}