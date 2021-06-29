#include "head.h"

vector <symbol_list_item> symbol_list;	//栈式符号表

vector <int> block_index_list;	//块索引表 

int hash_table[HASH_SIZE]; 	//散列表

map<string, record_member_list_> all_record_types;	//所有record类型


void push_symbol_table_item(string name, sem_item item, bool tab_flag, int cursor) {
	if (tab_flag) {
		locate_symbol_table();
	}
	else {
		// TO THINK: 什么也不做吧?
	}

	symbol_list_item tmp;
	tmp.name = name;
	tmp.item_attri = item;
	tmp.next_chain_item_index = -1;

	if (item.type == RECORDD) {
		if (all_record_types.find(item.attri_record.record_type_name) != all_record_types.end()) {
			int hash_index = string_hash(name);
			if (hash_table[hash_index] == -1) {	//如果无冲突，直接压栈并更新散列表
				hash_table[hash_index] = symbol_list.size();
				symbol_list.push_back(tmp);	//符号表插入新表项
			}
			else {	//如果有冲突
				if (not_redu_name(name, cursor)) {	//检查重名
					tmp.next_chain_item_index = hash_table[hash_index];	//连接原链
					hash_table[hash_index] = symbol_list.size();	//更新链头
					symbol_list.push_back(tmp);	//符号表插入新表项
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
		if (hash_table[hash_index] == -1) {	//如果无冲突，直接压栈并更新散列表
			hash_table[hash_index] = symbol_list.size();
			symbol_list.push_back(tmp);	//符号表插入新表项
		}
		else {	//如果有冲突
			if (not_redu_name(name, cursor)) {	//检查重名
				tmp.next_chain_item_index = hash_table[hash_index];	//连接原链
				hash_table[hash_index] = symbol_list.size();	//更新链头
				symbol_list.push_back(tmp);	//符号表插入新表项
			}
			else {
				append_error(SEMANTEME, "'" + name + "'", word_stream[cursor].row, word_stream[cursor].col, "conflict types for");
			}
		}
	}
}

int search_symbol_table(string name) {
	name = split(name, "[").at(0);	//分割可能存在的[]
	int hash_index = string_hash(name);
	if (hash_table[hash_index] == -1) {	//为空
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
	// 简单的测试版本
	//for (int i = symbol_list.size() - 1; i >= 0; i--) {
	//	if (name == symbol_list.at(i).name) {
	//		return i;
	//	}
	//}
	//return -1;
}

void locate_symbol_table() {
	block_index_list.push_back(symbol_list.size());	//块索引表更新
}

void relocate_symbol_table() {
	test_print_all();

	int top = block_index_list.back();
	if (block_index_list.size() > 0) {//szl
		while (symbol_list.size() != top + 1) {
			hash_table[string_hash(symbol_list.back().name)] = symbol_list.back().next_chain_item_index;	//从逻辑上摘除
			symbol_list.pop_back();	//从物理上摘除
		}
	}
	block_index_list.pop_back();

	// 简单的测试版本
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

	//简单的测试版本
	//for (int i = symbol_list.size() - 1; i >= block_index_list.at(block_index_list.size()-1); i--) {
	//	if (name == symbol_list.at(i).name) {
	//		////szl//cout << Xsplit << "语义错误 " << "变量重命名" << "at row:col " << word_stream[cursor].row << word_stream[cursor].col << endl << Xsplit;
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
			if (left == INTEGER && right == INTEGER)	//整形运算结果为整形
				return INTEGER;
			else if ((left == REAL || left == INTEGER) && (right == REAL || right == INTEGER))	//有浮点且都是数值的话结果就是浮点
				return REAL;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//其余类型不能做加减乘
			}
		}
		else if (opr == "/") {
			if ((left == REAL || left == INTEGER) && (right == REAL || right == INTEGER))	//都是数值才能除，返回REAL
				return REAL;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//其余类型不能
			}
		}
		else if (opr == "div") {
			if (left == INTEGER && right == INTEGER)	//都是int才能div，返回int
				return INTEGER;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//其余类型不能
			}
		}
		else if (opr == ">" || opr == "<" || opr == ">=" || opr == "<=" || opr == "==" || opr == "!=") {
			if ((left == INTEGER && right == REAL) || (left == REAL && right == INTEGER))	//数值可以比较
				return BOOLEAN;
			else if (left == right)	//其余类型如果一样的话也可以比较
				return BOOLEAN;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//类型不一样且不都是数值就不能比较
			}
		}
		else if (opr == "||" || opr == "&&") {
			if (left == BOOLEAN && right == BOOLEAN)	//只有两个bool才能或/且运算
				return BOOLEAN;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//其余报错
			}
		}
		else if (opr == "%") {
			if (left == INTEGER && right == INTEGER)	//只有两个int才能取余运算
				return INTEGER;
			else {
				append_error(SEMANTEME, get_c_type(left) + opr + get_c_type(right), word_stream[cursor].row, word_stream[cursor].col, "expression type wrong:");
				return -1;	//其余报错
			}
		}
	}
	else {
		return -1;	//left或right有负数，说明要不我写错了，要不子表达式已经出了问题。
	}
}

int judge_arrary_range(string name, expression_type_list_ expression_type_list_val, int cursor) {
	//return -1;	//测试语句
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
	//return -1;	//测试语句
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
	if (index != -1) {	// 虽说调用时保证了存在 但为保证健壮性，还是做个判断
		if (symbol_list.size() > 0) {
			if (symbol_list.at(index).item_attri.type != CONST) {	//如果不是常量才能赋值
				//判断赋值类型
				if (variable_type != expression_type) {	//左右类型不一样
					if (variable_type == MERVEILLEUS || expression_type == MERVEILLEUS) {
						// 如果出现record导致的MERVEILLEUS，就认为可以赋值，出问题就是运行时问题了
					}
					else if (variable_type == REAL && expression_type == INTEGER) {
						// 如果是int赋值给real，可以赋值
					}
					else if (expression_type < 0) {
						string vt = get_c_type(variable_type);
						
						string error_msg = "assignment to " + vt + " from someting have no value";
						append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, error_msg);
					}
					else {	//其余不能赋值
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
					// 正确
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
			if (symbol_list.at(index).item_attri.type != CONST) {	//如果不是常量才能read赋值

			}
			else {
				append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "const value '" + name + "' can not be read");
			}
		}
	}
	else {
		//append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "找不到标识符");
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
					//	// 这里说明是数组
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
	//		return_ = MERVEILLEUS;	//测试语句
	//		//append_error(SEMANTEME, record_type_name, word_stream[cursor].row, word_stream[cursor].col, "record类型不存在");
	//	}
	//}
	//else {
	//	append_error(SEMANTEME, split_result[0], word_stream[cursor].row, word_stream[cursor].col, "找不到标识符");
	//}
	//return return_;
}

unsigned int string_hash(string name) {
	// BKDRHash 算法
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	char* str = (char*)name.data();
	while (*str)
	{
		//szl//cout << *str << " ";
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF) % 100;	//返回0-100的BKDR哈希值

	//简单的测试版本
	//if (name[0] <= 90 && name[0] >= 65)
	//	return name[0] - 65;
	//else
	//	return name[0] - 97;
}

vector<para_item_pair>split_para_line(string para_line) {
	vector<para_item_pair> result;
	para_item_pair one_pair;
	if (!para_line.empty()) {
		vector<string> split_result = split(para_line, ",");	//分割各个参数
		if (split_result.size() > 0) {
			for (int i = 0; i < split_result.size(); i++) {
				vector<string> one_split = split(trim_head(split_result.at(i)), " ");	//分割名字和类型
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
	str += pattern;//扩展字符串以方便操作
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
			if (symbol_list.at(block_index_list.back()).item_attri.type == FUNCTION) {	//如果现在在函数里
				if (name == symbol_list.at(block_index_list.back()).name) {	//如果同名
					switch (from_grammar_type) {
					case PROCEDURE_CALL:
						append_error(SEMANTEME, name, word_stream[cursor].row, word_stream[cursor].col, "invalid expression：can not use function without () inside itself:");
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
		//szl//cout << record_type_name << "已定义" << endl;
	}
	else {
		// 检查成员类型是否都存在
		if (record_info.size() > 0) {
			for (int i = 0; i < record_info.size(); i++) {
				if (record_info.at(i).member_type == RECORD) {
					if (all_record_types.find(record_info.at(i).record_type_name) == all_record_types.end()) {
						append_error(SEMANTEME, record_type_name, word_stream[cursor].row, word_stream[cursor].col, "unknown type name '" + record_info.at(i).record_type_name + "' for member '" + record_info.at(i).member_name + "' of record");
						/*
							append_error(SEMANTEME, record_type_name, word_stream[cursor].row, word_stream[cursor].col, "record类型重定义");
							cout << record_type_name << "已定义" << endl;
						}
						else {
							// 检查成员类型是否都存在
							for (int i = 0; i < record_info.size(); i++) {
								if (record_info.at(i).member_type == RECORD) {
									if (all_record_types.find(record_info.at(i).record_type_name) == all_record_types.end()) {
										append_error(SEMANTEME, record_info.at(i).member_name, word_stream[cursor].row, word_stream[cursor].col, "使用了不存在的record类型");*/

										/*TO 沈:
										record_type_name是当前正在声明的record类型名，
										record_info.at(i).member_name是当前正在声明的record类型的某个成员名，
										record_info.at(i).record_type_name是这个成员的类型名。
										给你一个例子：
										program err4_2(input,output);
										type r1=record
										x,y:integer;
										end;
										type r2=record
										x,y:r1;
										z:r3;{这里有错，r3这个类型不存在}
										end;
										var i:real;
										begin
										end.
										报错为“Error type:3, line is:8, word is:z, error_msg:使用了不存在的record类型”
										*/
					}
				}
			}
			if (all_record_types.insert(pair<string, record_member_list_>(record_type_name, record_info)).second) {
				//cout << "插入成功" << endl;
			}
		}
	}

}

void test_print_all() {
	//szl
	/*
	//szl//cout << "------------符号表------------" << endl;
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
		//szl//cout << "------------空  表------------" << endl;
	//szl//cout << "-----------------------------" << endl << endl;

	//szl//cout << "------------索引表------------" << endl;
	if (!block_index_list.empty()) {
		for (int i = block_index_list.size() - 1; i >= 0; i--) {
			//szl//cout << block_index_list.at(i) << endl;
		}
	}
	else
		//szl//cout << "------------空  表------------" << endl;
	//szl//cout << "-----------------------------" << endl;
	*/
	////szl//cout << "------------散列表------------" << endl;
	//for (int i = 0; i < 26; i++)
	//	//szl//cout << hash_table[i] << endl;
	////szl//cout << "-----------------------------" << endl;
}

void init_semanteme() {
	// 清除各种容器的内容
	symbol_list.clear();
	block_index_list.clear();
	all_record_types.clear();

	block_index_list.push_back(0);
	for (int i = 0; i < HASH_SIZE; i++) {
		hash_table[i] = -1;
	}
}