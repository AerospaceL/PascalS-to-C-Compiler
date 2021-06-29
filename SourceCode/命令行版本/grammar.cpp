
#include "head.h"

int cursor = 0;

//typedef map<string, attribute_value> attribute; //键是属性名,值是属性值

//typedef struct {
//	string nonterminal_symbol;
//	attribute attri;
//}nonterminal_attribute;

stack <attribute_value> attribute_stack;
stack <sem_stack_item> sem_stack; //krr


//设计一个栈，栈里的每个元素是相关的属性
//每个非终结符对应的函数刚开始时入栈，不再需要时出栈
//在进入其他函数前，设置好相应的继承属性

// 语法分析程序入口
void grammatical_analysis()
{
	analysis_preparation();
	// 可能会输出一些调试信息
	programstruct();
	print_grammer_error();
}
int last_match(int id) {
	int i = cursor;
	int count = 1;
	int plus, minus;
	////szl//cout << id << endl;
	switch (id) {
	case PUN_rbrkt:plus = PUN_rbrkt; minus = PUN_lbrkt; break;
	case PUN_rsbrkt:plus = PUN_rsbrkt; minus = PUN_lsbrkt; break;
	case PUN_rcbrkt:plus = PUN_rcbrkt; minus = PUN_lcbrkt; break;
	case KEY_end:plus = KEY_end; minus = KEY_begin; break;
	}
	for (;; i--) {
		if (i < 0) break;
		if (word_stream[i].id == minus) if (--count == 0) return i;
		if (word_stream[i].id == plus) count++;

	}
	return -1;
}
void grammar_error(string error_msg, string symbol = "", int last = -1)
{

	int i = word_stream[cursor].row;//记录该单词所在行,若出错跳到下一行
	if (error_msg == "expect 'begin'") error_msg = error_msg + " after '" + symbol +"'";
	else if (symbol != "") {
		symbol = symbol != "the end of input" ? " before '" + symbol + "'" : " before "+symbol;
		error_msg = error_msg + symbol;
	}
	if (last != -1)
		error_msg = error_msg + " to match '" + word_stream[last].value + "' at line " + to_string(word_stream[last].row);
	append_error(GRAMMAR, word_stream[cursor].value, word_stream[cursor].row, 0, error_msg);

}

void analysis_preparation()
{
	if (word_stream.size() > 0) {//szl
		word word_null = word_stream.back();
		word_null.id = -1;
		word_null.symbol = "";
		for (int i = 0; i < 10; i++) {
			word_stream.push_back(word_null);
		}
	}
}

void print_grammer_error()
{
	int flag = 1;
	for (int i = 0; i < error_stream.size(); i++) {
		////szl//cout << "Error type:" << error_stream[i].stage << ", line is:" << error_stream[i].row << ", word is:" ;//szl
		////szl//cout << error_stream[i].error_code << ", error_msg:" << error_stream[i].content << endl;//szl

		if (error_stream[i].stage == GRAMMAR) {
			if (flag == 1) {
				flag = 0;
				cout << " [Error type " << error_stream[i].stage << "] " << "line " << error_stream[i].row << " : " << error_stream[i].content << endl;
			}
		}
		else cout << " [Error type " << error_stream[i].stage << "] " << "line " << error_stream[i].row << " : " << error_stream[i].content << " " << error_stream[i].error_code << endl;
	}
}
// programstruct 语句=================================================
void programstruct()
{
	program_head();

	//append_code_line("#include<stdio.h>");

	append_code_line("#include<iostream>");
	append_code_line("typedef enum {False, True} boolean;");

	if (word_stream[cursor].id == PUN_semicolon) {	// 304 --> ;
		if (cursor < word_stream.size())
			cursor++;
		program_body();

		if (word_stream[cursor].id == PUN_dot) {	// 305 --> .
			return;
		}
		else {
			grammar_error("expect '.'", "the end of input");
		}
	}
	else {
		grammar_error("except ';'", word_stream[cursor].value);
	}
}

void program_head()
{
	if (word_stream[cursor].id == KEY_program) {	// 101 --> program
		cursor++;
		if (word_stream[cursor].id == ID) {	// 1 --> id
			cursor++;
			// 下面的分支是判断program有、无参数 
			if (word_stream[cursor].id == PUN_lbrkt) { // 301 --> (
				cursor++;

				idlist_ idlist_val;
				attribute_value tmp;
				tmp.idlist_val = idlist_val;
				attribute_stack.push(tmp);

				idlist();

				idlist_val = attribute_stack.top().idlist_val;
				attribute_stack.pop();

				if (word_stream[cursor].id == PUN_rbrkt) { // 302 --> )
					cursor++;
				}
				else {
					grammar_error("expect ')'", word_stream[cursor].value, last_match(PUN_rbrkt));
				}
			}
		}
		else {
			grammar_error("expect ID", word_stream[cursor].value);
		}
	}
	else {
		grammar_error("expect 'program'", word_stream[cursor].value);
	}
}

// program_body 语句==================================================
void program_body()
{
	record_declarations();
	const_declarations();
	var_declarations();
	subprogram_declarations();

	append_code_line("int main(int argc, char* argv[]){");
	tab_change(1);
	
	compound_statement();

	append_code_line("return 0;");
	tab_change(-1);
	append_code_line("}");

}

// idlist 系列语句=====================================================
void idlist()
{
	if (word_stream[cursor].id == ID) {
		string id = word_stream[cursor].value;
		////szl//cout << "[" << id << "]";
		attribute_stack.top().idlist_val.push_back(id);
		cursor++;
		//idlist的follow符号是），如果是逗号，则递归执行
		if (word_stream[cursor].id == PUN_comma) {
			cursor++;
			idlist();
		}
	}
	else {
		grammar_error("expect ID", word_stream[cursor].value);
	}

}
// ===================================================================

// record_declarations 系列语句 ========================================

void record_declarations()
{
	if (word_stream[cursor].id == KEY_type) {
		cursor++;
		// 翻译方案
		if (word_stream[cursor].id == ID) {
			string id = word_stream[cursor].value;
			cursor++;
			// 翻译方案
			if (word_stream[cursor].id == OPR_equal) {
				cursor++;
				// 翻译方案
				if (word_stream[cursor].id == KEY_record) {
					cursor++;
					// 翻译方案

					record_list_ record_list_val;
					attribute_value tmp;
					tmp.record_list_val = record_list_val;
					attribute_stack.push(tmp);

					record_list();					
					
					record_list_val = attribute_stack.top().record_list_val;
					attribute_stack.pop();

			
					append_code_line("typedef struct{");
					tab_change(1);

					record_member_list_ member_list;
					record_member_pair member_pair;

					for (int i = 0; i < record_list_val.size(); i++)
					{
						for (int j = 0; j < record_list_val[i].idlist_val.size(); j++)
						{
							string id = record_list_val[i].idlist_val[j];
							if (record_list_val[i].type_val.type == ARRAY)
							{
								member_pair.member_name = id;
								member_pair.member_type = ARRAY;
								sem_item_array_var array_info;

								string type; 
								if (record_list_val[i].type_val.array_type == RECORD) {
									type = record_list_val[i].type_val.array_id;
									array_info.array_typee = RECORD;
									array_info.record_type_name = type;
								}
								else {
									type = get_c_type(record_list_val[i].type_val.array_type);
									array_info.array_typee = get_se_gr_type(type);
								}

								string code_line = type + " " + id;
								for (int k = 0; k < record_list_val[i].type_val.dimension.size(); k++)
								{
									code_line += ("[" + record_list_val[i].type_val.dimension[k].upper + "]");
									range new_range;	// krr
									new_range.lower = record_list_val[i].type_val.dimension[j].lower;	// krr
									new_range.upper = record_list_val[i].type_val.dimension[j].upper;	// krr
									array_info.dimension.push_back(new_range);	// krr
									if (new_range.lower > new_range.upper) {
										append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "array's lower bound can not greater than higher bound");	// krr
									}
								}

								member_pair.array_info = array_info;
								member_list.push_back(member_pair);

								code_line += ";";
								append_code_line(code_line);
							}
							else if (record_list_val[i].type_val.type == RECORD)
							{
								string type = record_list_val[i].type_val.id;
								append_code_line(type+" "+id+";");
								member_pair.member_name = id;
								member_pair.member_type = RECORD;
								member_pair.record_type_name = type;
								member_list.push_back(member_pair);
							}
							else
							{
								string type = get_c_type(record_list_val[i].type_val.type);
								append_code_line(type + " " + id + ";");
								member_pair.member_name = id;
								member_pair.member_type = get_se_gr_type(type);
								member_list.push_back(member_pair);
							}
						}
					}
					tab_change(-1);
					append_code_line("}"+id+";");
					register_record_type(id, member_list, cursor);	//krr57

					if (word_stream[cursor].id == KEY_end) {
						cursor++;
						// 翻译方案

						if (word_stream[cursor].id == PUN_semicolon) {
							cursor++;
						}
						else grammar_error("expect ';'", word_stream[cursor].value);
					}
					else grammar_error("expect 'end'", word_stream[cursor].value, last_match(KEY_end));
				}
				else grammar_error("expect 'record'", word_stream[cursor].value);
			}
			else grammar_error("expect '='", word_stream[cursor].value);
		}
		else grammar_error("expect ID", word_stream[cursor].value);
	}
	if (word_stream[cursor].id == KEY_type) {
		record_declarations();
	}
}

void record_list()
{
	if (word_stream[cursor].id == ID) {

		item item;


		idlist_ idlist_val;
		attribute_value tmp;
		tmp.idlist_val = idlist_val;
		attribute_stack.push(tmp);

		idlist();

		idlist_val = attribute_stack.top().idlist_val;
		attribute_stack.pop();

		item.idlist_val = idlist_val;

		if (word_stream[cursor].id == PUN_colon) {
			cursor++;
			// 翻译方案
			type_ type_val;
			attribute_value tmp;
			tmp.type_val = type_val;
			attribute_stack.push(tmp);

			type();

			type_val = attribute_stack.top().type_val;
			attribute_stack.pop();

			item.type_val = type_val;

			attribute_stack.top().record_list_val.push_back(item);

			if (word_stream[cursor].id == PUN_semicolon) {
				cursor++;
				// 翻译方案
				record_list();
			}
			else grammar_error("expect ';'", word_stream[cursor].value);
		}
		else grammar_error("expect ':'", word_stream[cursor].value);
	}
}


// const_declarations 系列语句 ========================================
void const_declarations()
{
	if (word_stream[cursor].id == KEY_const) {	// 103 --> const
		cursor++;
		const_declaration();

		if (word_stream[cursor].id == PUN_semicolon) {	// 304 --> ;
			cursor++;
		}
		else {
			grammar_error("expect ';'", word_stream[cursor].value);
		}
	}
}

void const_declaration()
{
	if (word_stream[cursor].id == ID) {	// 1 --> id
		string id = word_stream[cursor].value;
		cursor++;
		

		if (word_stream[cursor].id == OPR_equal) {	// 202 --> =
			cursor++;

			const_value_ const_value_val;
			attribute_value tmp;
			tmp.const_value_val = const_value_val;
			attribute_stack.push(tmp);

			const_value();

			string type = attribute_stack.top().const_value_val.type;
			string value = attribute_stack.top().const_value_val.value;
			attribute_stack.pop();

			if (type == "char*")
			{
				value[0] = '\"';
				value[value.size()-1] = '\"';

			}

			append_code_line("const " + type + " " + id + " = " + value + ";");
			sem_item new_item;	// krr
			new_item.type = CONST;	// krr
			sem_item_const new_attri;	// krr
			new_attri.typee = get_se_gr_type(type);	// krr
			new_item.attri_const = new_attri;	// krr
			push_symbol_table_item(id, new_item, false, cursor);	//入符号表 krr
			// HINT: 我需要什么？—— 常量名字：id，常量类型：type（，常量值value）


			const_declarationT();
		}
		else {
			grammar_error("expect '='", word_stream[cursor].value);
		}
	}
	else {
		grammar_error("expect ID", word_stream[cursor].value);
	}
}

void const_value()
{
	if (word_stream[cursor].id == OPR_add || word_stream[cursor].id == OPR_sub) {
		// 203 4 --> + =
		// 需要修改,放进if里面
		string value = word_stream[cursor].value;
		string type;
		cursor++;
		if (word_stream[cursor].id == INT) {	// 2 --> num
			type = "int";
			value += word_stream[cursor].value;
			cursor++;
		}
		else if (word_stream[cursor].id == REL) {
			type = "double";
			value += word_stream[cursor].value;
			cursor++;
		}
		else {
			grammar_error("expect num", word_stream[cursor].value);
		}

		attribute_stack.top().const_value_val.value = value;
		attribute_stack.top().const_value_val.type = type;

	}
	else if (word_stream[cursor].id == INT) {
		string type = "int";
		string value = word_stream[cursor].value;
		cursor++;

		attribute_stack.top().const_value_val.value = value;
		attribute_stack.top().const_value_val.type = type;
	}
	else if (word_stream[cursor].id == REL) {
		string type = "double";
		string value = word_stream[cursor].value;
		cursor++;

		attribute_stack.top().const_value_val.value = value;
		attribute_stack.top().const_value_val.type = type;
	}
	else if (word_stream[cursor].id == STR) {
		string type = "char*";
		string value = word_stream[cursor].value;
		cursor++;

		attribute_stack.top().const_value_val.value = value;
		attribute_stack.top().const_value_val.type = type;

	}
	else if (word_stream[cursor].id == KEY_true) {	// 204 --> -
		string type = "boolean";
		string value = "True";
		cursor++;

		attribute_stack.top().const_value_val.value = value;
		attribute_stack.top().const_value_val.type = type;
	}
	else if (word_stream[cursor].id == KEY_false) {	// 204 --> -
		string type = "boolean";
		string value = "False";
		cursor++;

		attribute_stack.top().const_value_val.value = value;
		attribute_stack.top().const_value_val.type = type;
	}
	else {
		grammar_error("expect simple expression", word_stream[cursor].value);
	}

}

void const_declarationT()
{
	int forward = cursor + 1;
	if (word_stream[cursor].id == PUN_semicolon && word_stream[forward].id == ID) {
		//  ; ID
		string id = word_stream[forward].value;
		cursor += 2;

		if (word_stream[cursor].id == OPR_equal) {	// 202 --> =
			cursor++;

			const_value_ const_value_val;
			attribute_value tmp;
			tmp.const_value_val = const_value_val;
			attribute_stack.push(tmp);

			const_value();

			string type = attribute_stack.top().const_value_val.type;
			string value = attribute_stack.top().const_value_val.value;
			attribute_stack.pop();

			if (type == "char*")
			{
				value[0] = '\"';
				value[value.size() - 1] = '\"';

			}

			append_code_line("const " + type + " " + id + " = " + value + ";");
			sem_item new_item;	// krr
			new_item.type = CONST;	// krr
			sem_item_const new_attri;	// krr
			new_attri.typee = get_se_gr_type(type);	// krr
			new_item.attri_const = new_attri;	// krr
			push_symbol_table_item(id, new_item, false, cursor);	//入符号表 krr
			// HINT: 我需要什么？—— 常量名字：id，常量类型：type（，常量值value）

			const_declarationT();

		}
		else {
			grammar_error("expect '='", word_stream[cursor].value);
		}
	}
}
// ===================================================================

// var_declarations 系列语句 ==========================================
void var_declarations()
{
	if (word_stream[cursor].id == KEY_var) {	// 103 --> var
		cursor++;

		var_declaration();

		if (word_stream[cursor].id == PUN_semicolon) {	// 304 --> ;
			cursor++;
		}
		else {
			grammar_error("expect ';'", word_stream[cursor].value);
		}
	}
}

void var_declaration()
{
	idlist_ idlist_val;
	attribute_value tmp;
	tmp.idlist_val = idlist_val;
	attribute_stack.push(tmp);
	
	idlist();

	idlist_val = attribute_stack.top().idlist_val;
	for (int i = 0; i < idlist_val.size(); i++)
	{
		//szl//cout << idlist_val[i];
	}
	attribute_stack.pop();

	if (word_stream[cursor].id == PUN_colon) {	// 306 --> :
		cursor++;

		type_ type_val;
		attribute_value tmp;
		tmp.type_val = type_val;
		attribute_stack.push(tmp);

		type();

		type_val = attribute_stack.top().type_val;

		attribute_stack.pop();
		
		if (type_val.type == ARRAY) {
			
			string type_of_c;
			if (type_val.array_type == RECORD) {
				type_of_c = type_val.array_id;
				for (int i = 0; i < idlist_val.size(); i++)
				{
					string code_line = "";
					code_line = type_of_c + " " + idlist_val[i];
					sem_item new_item;	// krr
					new_item.type = ARRAY_VAR;	// krr
					sem_item_array_var new_attri;	// krr
					new_attri.array_typee = RECORD;	// krr
					for (int j = 0; j < type_val.dimension.size(); j++)
					{
						code_line += "[";
						code_line += type_val.dimension[j].upper;
						code_line += "]";
						range new_range;	// krr
						new_range.lower = type_val.dimension[j].lower;	// krr
						new_range.upper = type_val.dimension[j].upper;	// krr
						new_attri.dimension.push_back(new_range);	// krr
						if (new_range.lower > new_range.upper) {
							append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "array's lower bound can not greater than higher bound");	// krr
						}
					}
					code_line += ";";
					append_code_line(code_line);

					new_item.attri_array_var = new_attri;	// krr
					push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
					// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：数组，数组属性.数组上下界，数组属性.数组元素类型：都在attribute_stack.top().type_val里
				}
			}
			else {
				type_of_c = get_c_type(type_val.array_type);
				for (int i = 0; i < idlist_val.size(); i++)
				{
					string code_line = "";
					code_line = type_of_c + " " + idlist_val[i];
					sem_item new_item;	// krr
					new_item.type = ARRAY_VAR;	// krr
					sem_item_array_var new_attri;	// krr
					new_attri.array_typee = get_se_gr_type(type_of_c);	// krr
					for (int j = 0; j < type_val.dimension.size(); j++)
					{
						code_line += "[";
						code_line += type_val.dimension[j].upper;
						code_line += "]";
						range new_range;	// krr
						new_range.lower = type_val.dimension[j].lower;	// krr
						new_range.upper = type_val.dimension[j].upper;	// krr
						new_attri.dimension.push_back(new_range);	// krr
						if (new_range.lower > new_range.upper) {
							append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "array's lower bound can not greater than higher bound");	// krr
						}
					}
					code_line += ";";
					append_code_line(code_line);

					new_item.attri_array_var = new_attri;	// krr
					push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
					// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：数组，数组属性.数组上下界，数组属性.数组元素类型：都在attribute_stack.top().type_val里
				}
			}
		}
		else
		{	
			//!!!!!!!!!!!语法有问题
			string type_of_c;
			if (type_val.type == RECORD)
				type_of_c = type_val.id;
			else
				type_of_c = get_c_type(type_val.type);
			string code_line = type_of_c + " ";
			
			if (type_val.type == RECORD) {
				sem_item new_item;	// krr
				new_item.type = RECORDD;	// krr
				sem_item_record new_attri;	// krr
				new_attri.record_type_name = type_val.id;	// krr
				new_item.attri_record = new_attri;	// krr

				for (int i = 0; i < idlist_val.size(); i++)
				{
					code_line += idlist_val[i];

					push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr

					if (i != idlist_val.size() - 1)
					{
						code_line += ", ";
					}
					else
					{
						code_line += ";";
					}
				}
				append_code_line(code_line);
			}
			else {
				sem_item new_item;	// krr
				new_item.type = BASIC_VAR;	// krr
				sem_item_basic_var new_attri;	// krr
				new_attri.typee = get_se_gr_type(type_of_c);	// krr
				new_item.attri_basic_var = new_attri;	// krr

				for (int i = 0; i < idlist_val.size(); i++)
				{
					code_line += idlist_val[i];

					push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr

					if (i != idlist_val.size() - 1)
					{
						code_line += ", ";
					}
					else
					{
						code_line += ";";
					}
				}
				append_code_line(code_line);
			}
			
			
			// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：type_of_c
		}
		var_declarationT();
	}
	else {
		grammar_error("expect ':'", word_stream[cursor].value);
	}
}

void var_declarationT()
{
	int forward = cursor + 1;
	if (word_stream[cursor].id == PUN_semicolon && word_stream[forward].id == ID) {
		cursor++;

		idlist_ idlist_val;
		attribute_value tmp;
		tmp.idlist_val = idlist_val;
		attribute_stack.push(tmp);

		idlist();

		idlist_val = attribute_stack.top().idlist_val;
		attribute_stack.pop();

		for (int i = 0; i < idlist_val.size(); i++)
		{
			//szl//cout << idlist_val[i];
		}

		if (word_stream[cursor].id == PUN_colon) {	// 306 --> :
			cursor++;

			type_ type_val;
			attribute_value tmp;
			tmp.type_val = type_val;
			attribute_stack.push(tmp);

			type();

			type_val = attribute_stack.top().type_val;
			////szl//cout << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{" << type_val.type;
			attribute_stack.pop();

			if (type_val.type == ARRAY) {

				string type_of_c;
				if (type_val.array_type == RECORD) {
					type_of_c = type_val.array_id;
					for (int i = 0; i < idlist_val.size(); i++)
					{
						string code_line = "";
						code_line = type_of_c + " " + idlist_val[i];
						sem_item new_item;	// krr
						new_item.type = ARRAY_VAR;	// krr
						sem_item_array_var new_attri;	// krr
						new_attri.array_typee = RECORD;	// krr
						for (int j = 0; j < type_val.dimension.size(); j++)
						{
							code_line += "[";
							code_line += type_val.dimension[j].upper;
							code_line += "]";
							range new_range;	// krr
							new_range.lower = type_val.dimension[j].lower;	// krr
							new_range.upper = type_val.dimension[j].upper;	// krr
							new_attri.dimension.push_back(new_range);	// krr
							if (new_range.lower > new_range.upper) {
								append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "array's lower bound can not greater than higher bound");	// krr
							}
						}
						code_line += ";";
						append_code_line(code_line);

						new_item.attri_array_var = new_attri;	// krr
						push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
						// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：数组，数组属性.数组上下界，数组属性.数组元素类型：都在attribute_stack.top().type_val里
					}
				}
				else {
					type_of_c = get_c_type(type_val.array_type);
					for (int i = 0; i < idlist_val.size(); i++)
					{
						string code_line = "";
						code_line = type_of_c + " " + idlist_val[i];
						sem_item new_item;	// krr
						new_item.type = ARRAY_VAR;	// krr
						sem_item_array_var new_attri;	// krr
						new_attri.array_typee = get_se_gr_type(type_of_c);	// krr
						for (int j = 0; j < type_val.dimension.size(); j++)
						{
							code_line += "[";
							code_line += type_val.dimension[j].upper;
							code_line += "]";
							range new_range;	// krr
							new_range.lower = type_val.dimension[j].lower;	// krr
							new_range.upper = type_val.dimension[j].upper;	// krr
							new_attri.dimension.push_back(new_range);	// krr
							if (new_range.lower > new_range.upper) {
								append_error(SEMANTEME, "array", word_stream[cursor].row, word_stream[cursor].col, "数组下界大于上界");	// krr
							}
						}
						code_line += ";";
						append_code_line(code_line);

						new_item.attri_array_var = new_attri;	// krr
						push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
						// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：数组，数组属性.数组上下界，数组属性.数组元素类型：都在attribute_stack.top().type_val里
					}
				}
			}
			else
			{
				//!!!!!!!!!!!语法有问题

				string type_of_c;
				if (type_val.type == RECORD)
					type_of_c = type_val.id;
				else
					type_of_c = get_c_type(type_val.type);
				string code_line = type_of_c + " ";

				if (type_val.type == RECORD) {
					sem_item new_item;	// krr
					new_item.type = RECORDD;	// krr
					sem_item_record new_attri;	// krr
					new_attri.record_type_name = type_val.id;	// krr
					new_item.attri_record = new_attri;	// krr

					for (int i = 0; i < idlist_val.size(); i++)
					{
						code_line += idlist_val[i];

						push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr

						if (i != idlist_val.size() - 1)
						{
							code_line += ", ";
						}
						else
						{
							code_line += ";";
						}
					}
					append_code_line(code_line);
				}
				else {
					sem_item new_item;	// krr
					new_item.type = BASIC_VAR;	// krr
					sem_item_basic_var new_attri;	// krr
					new_attri.typee = get_se_gr_type(type_of_c);	// krr
					new_item.attri_basic_var = new_attri;	// krr

					for (int i = 0; i < idlist_val.size(); i++)
					{
						code_line += idlist_val[i];

						push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr

						if (i != idlist_val.size() - 1)
						{
							code_line += ", ";
						}
						else
						{
							code_line += ";";
						}
					}
					append_code_line(code_line);
				}


				// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：type_of_c
			}

			var_declarationT();

		}
		else {
			grammar_error("expect ':'", word_stream[cursor].value);
		}
	}
}

void type()
{
	// array [ period ] of basic_type
	if (word_stream[cursor].id == KEY_array) {	// 108 --> array
		attribute_stack.top().type_val.type = ARRAY;
		cursor++;
		if (word_stream[cursor].id == PUN_lsbrkt) {	// 308 --> [
			cursor++;

			period();

			if (word_stream[cursor].id == PUN_rsbrkt) {	// 309 --> ]
				cursor++;

				if (word_stream[cursor].id == KEY_of) {	// 109 --> of
					cursor++;

					if (word_stream[cursor].id == ID)
					{
						attribute_stack.top().type_val.array_type = RECORD;
						attribute_stack.top().type_val.array_id = word_stream[cursor].value;
						cursor++;
					}
					else {
						attribute_value tmp;
						attribute_stack.push(tmp);

						basic_type();

						int basic_type_val = attribute_stack.top().ival;
						attribute_stack.pop();
						attribute_stack.top().type_val.array_type = basic_type_val;
					}
				}
				else {
					grammar_error("expect 'of'", word_stream[cursor].value);
				}

			}
			else {
				grammar_error("expect ']'", word_stream[cursor].value, last_match(PUN_rsbrkt));
			}
		}
		else {
			grammar_error("expect '['", word_stream[cursor].value);
		}
	}
	else if (word_stream[cursor].id == ID) {
		attribute_stack.top().type_val.type = RECORD;
		attribute_stack.top().type_val.id = word_stream[cursor].value;
		cursor++;
	}
	else {
		attribute_value tmp;
		attribute_stack.push(tmp);

		basic_type();

		int basic_type_val = attribute_stack.top().ival;
		attribute_stack.pop();
		attribute_stack.top().type_val.type = basic_type_val;
	}
}

void basic_type()
{
	if (word_stream[cursor].id == KEY_integer) { // 4种数据类型
		attribute_stack.top().ival = INTEGER;
		cursor++;
	}
	else if (word_stream[cursor].id == KEY_real) {
		attribute_stack.top().ival = REAL;
		cursor++;
	}
	else if (word_stream[cursor].id == KEY_boolean) {
		attribute_stack.top().ival = BOOLEAN;
		cursor++;
	}
	else if (word_stream[cursor].id == KEY_char) {
		attribute_stack.top().ival = CHAR;
		cursor++;
	}
	else if (word_stream[cursor].id == KEY_string) {
		attribute_stack.top().ival = STRR;
		cursor++;
	}
	else {
		grammar_error("expect basic type", word_stream[cursor].value);
	}
}

void period()
{
	////szl//cout << "period" << endl;
	if (word_stream[cursor].id == INT) {	// 2 --> num
		string lower = word_stream[cursor].value; //数组下界
		cursor++;
		if (word_stream[cursor].id == PUN_ddot) {	// 312 --> ..
			cursor++;
			if (word_stream[cursor].id == INT) {	// 2 --> num
				string upper = word_stream[cursor].value; //数组上界
				cursor++;
					
				range range_val;
				range_val.lower = lower;
				range_val.upper = upper;

				//szl//cout <<"["<< lower <<".."<< upper <<"]"<< endl;
				attribute_stack.top().type_val.dimension.push_back(range_val);

				if (word_stream[cursor].id == PUN_comma) {	// 303 --> ,
					cursor++;
					period();
				}
			}
			else {
				grammar_error("expect digit of array", word_stream[cursor].value);
			}
		}
		else {
			grammar_error("expect '..'", word_stream[cursor].value);
		}
	}
	else {
		grammar_error("expect digit of array", word_stream[cursor].value);
	}
}
// ====================================================================

// subprogram_declarations 系列语句=====================================
void subprogram_declarations()
{
	if (word_stream[cursor].id == KEY_procedure || word_stream[cursor].id == KEY_funtion) {
		// 104 5 --> procedure func
		subprogram();
		if (word_stream[cursor].id == PUN_semicolon) {
			cursor++;
		}
		else {
			grammar_error("expect ';'", word_stream[cursor].value);
		}

		if (word_stream[cursor].id == KEY_procedure || word_stream[cursor].id == KEY_funtion) {
			subprogram_declarations();
		}
	}

}

void subprogram()
{
	/*
	type_ type_val;
	attribute_value tmp;
	tmp.type_val = type_val;
	attribute_stack.push(tmp);

	type();

	type_val = attribute_stack.top().type_val;
	attribute_stack.pop();
	*/

	subprogram_head_ subprogram_head_val;
	attribute_value tmp;
	tmp.subprogram_head_val = subprogram_head_val;
	attribute_stack.push(tmp);

	subprogram_head();

	subprogram_head_val = attribute_stack.top().subprogram_head_val;
	attribute_stack.pop();

	string temp_line = get_temp_line();
	clean_temp_line();

	if (temp_line.size() > 2)
	{
		temp_line.erase(temp_line.size() - 2);
	}


	string code_line = get_c_type(subprogram_head_val.type) + " " + subprogram_head_val.id + "(" + temp_line + "){";
	
	append_code_line(code_line);

	sem_item new_item;	// krr
	if (subprogram_head_val.type != VOID) {	//函数
		new_item.type = FUNCTION;	// krr
		sem_item_function new_attri;	// krr
		new_attri.return_type = subprogram_head_val.type;	// krr
		vector<para_item_pair> split_result = split_para_line(temp_line);	// krr
		for (int k = 0; k < split_result.size(); k++) {	// krr
			new_attri.para_type_list.push_back(split_result.at(k).para_type);	// krr
			new_attri.ref_flag_list.push_back(split_result.at(k).ref_flag);
		}
		new_item.attri_function = new_attri;	// krr
		push_symbol_table_item(subprogram_head_val.id, new_item, true, cursor);	//函数名入符号表,并定位 krr

		//参数入符号表
		for (int k = 0; k < split_result.size(); k++) {	// krr
			new_item.type = BASIC_VAR;	// krr
			sem_item_basic_var new_attri;	// krr
			new_attri.typee = split_result.at(k).para_type;	// krr
			new_item.attri_basic_var = new_attri;	// krr
			push_symbol_table_item(split_result.at(k).para_name, new_item, false, cursor);	//参数入符号表 krr
		}
	}
	else {	//过程
		new_item.type = PROCEDURE;	// krr
		sem_item_procedure new_attri;	// krr
		vector<para_item_pair> split_result = split_para_line(temp_line);	// krr
		for (int k = 0; k < split_result.size(); k++) {	// krr
			new_attri.para_type_list.push_back(split_result.at(k).para_type);	// krr
			new_attri.ref_flag_list.push_back(split_result.at(k).ref_flag);
		}
		new_item.attri_procedure = new_attri;	// krr
		push_symbol_table_item(subprogram_head_val.id, new_item, true, cursor);	//过程名入符号表,并定位 krr

		//参数入符号表
		for (int k = 0; k < split_result.size(); k++) {	// krr
			new_item.type = BASIC_VAR;	// krr
			sem_item_basic_var new_attri;	// krr
			new_attri.typee = split_result.at(k).para_type;	// krr
			new_item.attri_basic_var = new_attri;	// krr
			push_symbol_table_item(split_result.at(k).para_name, new_item, false, cursor);	//参数入符号表 krr
		}
	}
	// HINT: 我需要什么？—— 函数/过程名字：subprogram_head_val.id，返回类型：subprogram_head_val.type， 参数列表：需要想办法切分temp_line
	// 注意参数和函数/过程名要放在符号表的下一块内，即要进行定位操作
	// TO THINK 函数名和"result"的处理
	tab_change(1);	
	if(subprogram_head_val.type!=VOID)
		append_code_line(get_c_type(subprogram_head_val.type) + " result;");



	if (word_stream[cursor].id == PUN_semicolon) {
		cursor++;
	}
	else {
		grammar_error("expect ';'", word_stream[cursor].value);
	}
	subprogram_body();

	if (subprogram_head_val.type != VOID)
		append_code_line("return result;");
	else
		append_code_line("return;");

	tab_change(-1);
	relocate_symbol_table();
	// 注意要重定位了
	append_code_line("}");
}

void subprogram_head()
{
	if (word_stream[cursor].id == KEY_procedure) {	// 104 --> procedure
		cursor++;
		if (word_stream[cursor].id == ID) {	// 1 --> id

			attribute_stack.top().subprogram_head_val.type = VOID;
			attribute_stack.top().subprogram_head_val.id = word_stream[cursor].value;

			cursor++;
			formal_parameter();
		}
		else {
			grammar_error("expect ID", word_stream[cursor].value);
		}
	}
	else if (word_stream[cursor].id == KEY_funtion) {	// 104 --> function
		cursor++;
		if (word_stream[cursor].id == ID) {	// 1 --> id
			attribute_stack.top().subprogram_head_val.id = word_stream[cursor].value;
			cursor++;
			formal_parameter();

			if (word_stream[cursor].id == PUN_colon) {	// 306 --> :
				cursor++;
				// function return type

				attribute_value tmp;
				attribute_stack.push(tmp);

				basic_type();

				int basic_type_val = attribute_stack.top().ival;
				attribute_stack.pop();

				attribute_stack.top().subprogram_head_val.type = basic_type_val;
			}
			else {
				grammar_error("expect ':'", word_stream[cursor].value);
			}
		}
		else {
			grammar_error("expect ID", word_stream[cursor].value);
		}
	}
}

void subprogram_body()
{
	const_declarations();
	////szl//cout << "subprogram_body running" << endl;
	var_declarations();
	compound_statement();
}

void formal_parameter()
{
	if (word_stream[cursor].id == PUN_lbrkt) {	// 301 --> (
		cursor++;
		parameter_list();
		if (word_stream[cursor].id == PUN_rbrkt) {	// 302 --> )
			cursor++;
		}
		else {
			grammar_error("expect ')'", word_stream[cursor].value, last_match(PUN_rbrkt));
		}
	}
}

void parameter_list()
{
	parameter();

	if (word_stream[cursor].id == PUN_semicolon) {	// 304 --> ;
		cursor++;
		parameter_list();
	}
}

void parameter()
{
	if (word_stream[cursor].id == KEY_var) {	// 103 --> var
		var_parameter();
	}
	else {
		value_parameter();
	}
}

void var_parameter()
{
	if (word_stream[cursor].id == KEY_var) {	// 103 --> var
		cursor++;
		set_var_flag(1);
		value_parameter();
		
	}
}

void value_parameter() //形参填写
{
	int var_flag = get_var_flag();
	set_var_flag(0);
	
	idlist_ idlist_val;
	attribute_value tmp;
	tmp.idlist_val = idlist_val;
	attribute_stack.push(tmp);

	idlist();

	idlist_val = attribute_stack.top().idlist_val;
	attribute_stack.pop();

	if (word_stream[cursor].id == PUN_colon) {	// 306 --> :
		cursor++;

		//int type_val;
		//string id;

		/*
		if (word_stream[cursor].id == ID)
		{
			id = word_stream[cursor].value;
			type_val = RECORD;
			cursor++;
		}
		else {
			attribute_value tmp;
			attribute_stack.push(tmp);

			basic_type();

			type_val = attribute_stack.top().ival;
			attribute_stack.pop();
		}

		string code_line;

		for (int i = 0; i < idlist_val.size(); i++)
		{
			if (type_val == RECORD)
			{
				code_line += (id + " " + idlist_val[i]);
				code_line += ", ";
			}
			else {
				code_line += (get_c_type(type_val) + " " + idlist_val[i]);
				code_line += ", ";
			}
		}
		*/
		type_ type_val;
		attribute_value tmp;
		tmp.type_val = type_val;
		attribute_stack.push(tmp);

		type();

		type_val = attribute_stack.top().type_val;

		attribute_stack.pop();

		if (type_val.type == ARRAY) {

			string type_of_c;
			if (type_val.array_type == RECORD) {
				type_of_c = type_val.array_id;
				for (int i = 0; i < idlist_val.size(); i++)
				{
					string code_line = "";
					if (var_flag == 1)
						code_line = type_of_c +" " + "&" + idlist_val[i];
					else
						code_line = type_of_c + " " + idlist_val[i];
					sem_item new_item;	// krr
					new_item.type = ARRAY_VAR;	// krr
					sem_item_array_var new_attri;	// krr
					new_attri.array_typee = RECORD;	// krr
					for (int j = 0; j < type_val.dimension.size(); j++)
					{
						code_line += "[";
						code_line += type_val.dimension[j].upper;
						code_line += "]";
						range new_range;	// krr
						new_range.lower = type_val.dimension[j].lower;	// krr
						new_range.upper = type_val.dimension[j].upper;	// krr
						new_attri.dimension.push_back(new_range);	// krr
						if (new_range.lower > new_range.upper) {
							append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "array's lower bound can not greater than higher bound");	// krr
						}
					}
					code_line += ", ";
					append_partial_code_line(code_line);

					new_item.attri_array_var = new_attri;	// krr
					//push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
					// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：数组，数组属性.数组上下界，数组属性.数组元素类型：都在attribute_stack.top().type_val里
				}
			}
			else {
				type_of_c = get_c_type(type_val.array_type);
				for (int i = 0; i < idlist_val.size(); i++)
				{
					string code_line = "";
					if (var_flag == 1)
						code_line = type_of_c + " " + "&" + idlist_val[i];
					else
						code_line = type_of_c + " " + idlist_val[i];
					sem_item new_item;	// krr
					new_item.type = ARRAY_VAR;	// krr
					sem_item_array_var new_attri;	// krr
					new_attri.array_typee = get_se_gr_type(type_of_c);	// krr
					for (int j = 0; j < type_val.dimension.size(); j++)
					{
						code_line += "[";
						code_line += type_val.dimension[j].upper;
						code_line += "]";
						range new_range;	// krr
						new_range.lower = type_val.dimension[j].lower;	// krr
						new_range.upper = type_val.dimension[j].upper;	// krr
						new_attri.dimension.push_back(new_range);	// krr
						if (new_range.lower > new_range.upper) {
							append_error(SEMANTEME, "", word_stream[cursor].row, word_stream[cursor].col, "array's lower bound can not greater than higher bound");	// krr
						}
					}
					code_line += ", ";
					append_partial_code_line(code_line);

					new_item.attri_array_var = new_attri;	// krr
					//push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
					// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：数组，数组属性.数组上下界，数组属性.数组元素类型：都在attribute_stack.top().type_val里
				}
			}
		}
		else
		{
			string type_of_c;

			if (type_val.type == RECORD)
				type_of_c = type_val.id;
			else
				type_of_c = get_c_type(type_val.type);
			string code_line;


			if (type_val.type == RECORD) {
				sem_item new_item;	// krr
				new_item.type = RECORDD;	// krr
				sem_item_record new_attri;	// krr
				new_attri.record_type_name = type_val.id;	// krr
				new_item.attri_record = new_attri;	// krr

				for (int i = 0; i < idlist_val.size(); i++)
				{
					if (var_flag==1)
						code_line += (type_of_c + " " + "&" + idlist_val[i]);
					else
						code_line += (type_of_c + " " + idlist_val[i]);

					//push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
					code_line += ", ";
				}
				append_partial_code_line(code_line);
			}
			else {
				sem_item new_item;	// krr
				new_item.type = BASIC_VAR;	// krr
				sem_item_basic_var new_attri;	// krr
				new_attri.typee = get_se_gr_type(type_of_c);	// krr
				new_item.attri_basic_var = new_attri;	// krr

				for (int i = 0; i < idlist_val.size(); i++)
				{
					if (var_flag == 1)
						code_line += (type_of_c + " " +"&" + idlist_val[i]);
					else
						code_line += (type_of_c + " " + idlist_val[i]);

					//push_symbol_table_item(idlist_val[i], new_item, false, cursor);	//入符号表 krr
					code_line += ", ";
				}
				append_partial_code_line(code_line);
			}


			// HINT: 我需要什么？—— 变量名字：idlist_val中每个元素，变量类型：type_of_c
		}



	}
	else {
		grammar_error("expect ':'", word_stream[cursor].value);
	}
}
// ================================================================

// compound_statement 系列语句======================================
void compound_statement() {
	


	if (word_stream[cursor].id == KEY_begin) {	// 106 --> begin

		//append_code_line("{");
		//tab_change(1);
		cursor++;
		statement_list();
		if (word_stream[cursor].id == KEY_end) {	// 107 --> end
			//tab_change(-1);
			//append_code_line("}");

			cursor++;
		}
		else {
			grammar_error("expect 'end'", word_stream[cursor].value, last_match(KEY_end));
		}
	}
	else {
		grammar_error("expect 'begin'", word_stream[cursor].value);
	}
}

void statement_list()
{
	statement();
	if (word_stream[cursor].id == PUN_semicolon) {	// 304 --> ;
		cursor++;
		statement_list();
	}
}

void statement()
{
	int forwardWord = cursor + 1;
	// procedure_call or assign_expression
	if (word_stream[cursor].id == ID) { // 1 --> id
		// assign_expression
		if (word_stream[forwardWord].id == OPR_assign || word_stream[forwardWord].id == PUN_lsbrkt || word_stream[forwardWord].id == PUN_dot) {
			// 201 308 --> :=  [
			assign_expression();
		}
		// procedure_call
		else {
			// wordNumber++;	// ++前是id 
			procedure_call();
		}

	}
	// compound_statement
	else if (word_stream[cursor].id == KEY_begin) {	// 106 --> begin
		compound_statement();
	}
	// if_statement
	else if (word_stream[cursor].id == KEY_if) {	// 114 --> if
		if_expression();
	}
	// for_statement
	else if (word_stream[cursor].id == KEY_for) {	// 117 --> for
		for_expression();
	}
	else if (word_stream[cursor].id == KEY_while) {	// 117 --> while
		while_expression();
	}
	// read_statement
	else if (word_stream[cursor].id == KEY_read) {	// 120 --> read
		read_expression();
	}
	// write_statement
	else if (word_stream[cursor].id == KEY_write) {	// 121 --> write
		write_expression();
	}
}

// 7种类型语句格式
void procedure_call()
{
	string id = word_stream[cursor].value;//检查ID！！！！！！！！！！！！！！！！！！
	string codeline = id;
	expression_type_list_ expression_type_list_val;	// krr

	cursor++;	// ++前是id 
	if (word_stream[cursor].id == PUN_lbrkt) {	// 301 --> (
		codeline += "(";
		cursor++;

		attribute_value tmp;
		expression_list_ expression_list_val;
		tmp.expression_list_val = expression_list_val;
		attribute_stack.push(tmp);
		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type_list = expression_type_list_val;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		expression_list();

		expression_list_val = attribute_stack.top().expression_list_val;
		expression_type_list_val = sem_stack.top().expression_type_list;	// krr
		attribute_stack.pop();
		sem_stack.pop();	// krr

		for (int i = 0; i < expression_list_val.size(); i++)
		{
			codeline += expression_list_val[i];
			//szl//cout << "[["<<expression_list_val[i]<<"]]"<<endl;
			if (i != expression_list_val.size() - 1)
			{
				codeline += ", ";
			}
		}

		if (word_stream[cursor].id == PUN_rbrkt) {	// 302 --> )
			codeline += ");";
			append_code_line(codeline);
			judge_func_and_proc(id, expression_type_list_val, cursor);	// krr
			cursor++;

		}
		else {
			grammar_error("expect ')'", word_stream[cursor].value, last_match(PUN_rbrkt));
		}
	}
	else if(word_stream[cursor].id == PUN_semicolon){

		replace_function_return(id, OTHER_GRAMMERS, cursor);

		append_code_line(id+"();");
		judge_func_and_proc(id, expression_type_list_val, cursor);	// krr

		// 翻译方案
	}
	
}

void assign_expression()
{
	attribute_value tmp;
	tmp.sval = "";
	attribute_stack.push(tmp);

	sem_stack_item sem_tmp;	// krr
	sem_tmp.expression_type = -2;	// krr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krr

	variable();

	string variable_val = attribute_stack.top().sval;
	attribute_stack.pop();

	int variable_type = sem_stack.top().expression_type;	// krr
	sem_stack.pop();	// krr

	string codeline;

	if (word_stream[cursor].id == OPR_assign) {	// 201 --> :=
		cursor++;

		attribute_value tmp;
		tmp.sval = "";
		attribute_stack.push(tmp);

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr
		
		expression();
		
		string expression_val = attribute_stack.top().sval;
		attribute_stack.pop();

		int expression_type = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		codeline = variable_val + " = " + expression_val + ";";
		append_code_line(codeline);
		// 这里只需检索符号表，检查标识符是否存在；(只要检查variable_type是不是大于0就行) krr
		if (variable_type > 0) {	//这里保证了 variable 存在且有类型（不是过程名），但不知道是不是常量 krr
			judge_assign_type(variable_val, variable_type, expression_type, cursor)	;//若存在，检查是否为常量，以及赋值左右类型是否合法 krr
		}

	}
	else {
		grammar_error("expect ':='", word_stream[cursor].value);
	}
}

// 如果你想找compound statement, 它在上面

void if_expression()
{
	cursor++; // 此时token是if

	attribute_value tmp;
	tmp.sval = "";
	attribute_stack.push(tmp);

	sem_stack_item sem_tmp;	// krr
	sem_tmp.expression_type = -2;	// krr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krr

	expression();

	string expression_val = attribute_stack.top().sval;
	attribute_stack.pop();

	int expression_type = sem_stack.top().expression_type;	// krr
	sem_stack.pop();	// krr

	string codeline = "if(" + expression_val + "){";
	judge_if_expression(expression_type, cursor);	// krr
	append_code_line(codeline);
	tab_change(1);

	if (word_stream[cursor].id == KEY_then) {	// 115 --> then
		cursor++;
		statement();
		
		tab_change(-1);	
		append_code_line("}");
		

		if (word_stream[cursor].id == KEY_else) {	// 116 --> else
			cursor++;
			append_code_line("else{");
			tab_change(1);

			statement();

			tab_change(-1);
			append_code_line("}");
			
		}		
		

	}
	else {
		grammar_error("expect 'then'", word_stream[cursor].value);
	}



}

void while_expression()
{

	cursor++;

	attribute_value tmp;
	tmp.sval = "";
	attribute_stack.push(tmp);

	sem_stack_item sem_tmp;	// krr
	sem_tmp.expression_type = -2;	// krr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krr

	expression();

	string expression_val_1 = attribute_stack.top().sval;
	attribute_stack.pop();

	int expression_type = sem_stack.top().expression_type;	// krr
	sem_stack.pop();	// krr

	string codeline = "while(";
	codeline += (expression_val_1 + "){");

	judge_while_expression(expression_type, cursor);	// krr

	append_code_line(codeline);
	tab_change(1);

	if (word_stream[cursor].id == KEY_do) {
		cursor++;
		statement();

		tab_change(-1);
		append_code_line("}");
	}
	else {
		grammar_error("expect 'do'", word_stream[cursor].value);
	}
}

void for_expression()
{
	int expression_1_type = -2;
	int expression_2_type = -2;
	cursor++;	// for
	if (word_stream[cursor].id == ID) {	// id
		string id = word_stream[cursor].value;
		cursor++;
		if (word_stream[cursor].id == OPR_assign) {	// assign
			cursor++;

			attribute_value tmp;
			tmp.sval = "";
			attribute_stack.push(tmp);

			sem_stack_item sem_tmp;	// krr
			sem_tmp.expression_type = -2;	// krr
			sem_tmp.valid_flag = true;
			sem_stack.push(sem_tmp);	// krr
			
			expression();
			
			string expression_val_1 = attribute_stack.top().sval;
			attribute_stack.pop();

			expression_1_type = sem_stack.top().expression_type;	// krr
			sem_stack.pop();	// krr

			if (word_stream[cursor].id == KEY_to) {	// to
				cursor++;

				attribute_value tmp;
				tmp.sval = "";
				attribute_stack.push(tmp);

				sem_stack_item sem_tmp;	// krr
				sem_tmp.expression_type = -2;	// krr
				sem_tmp.valid_flag = true;
				sem_stack.push(sem_tmp);	// krr

				expression();

				string expression_val_2 = attribute_stack.top().sval;
				attribute_stack.pop();

				expression_2_type = sem_stack.top().expression_type;	// krr
				sem_stack.pop();	// krr

				string codeline = "for(" + id + " = " + expression_val_1 + "; " + id + " <= " + expression_val_2 + "; " + id + "++" + "){";
				judge_for_expression(id, expression_1_type, expression_2_type, cursor);

				append_code_line(codeline);
				tab_change(1);

				if (word_stream[cursor].id == KEY_do) {	// do
					cursor++;
					statement();

					tab_change(-1);
					append_code_line("}");

				}
				else {
					grammar_error("expect 'do'", word_stream[cursor].value);
				}
			}
			else {
				grammar_error("expect 'to'", word_stream[cursor].value);
			}
		}
		else {
			grammar_error("expect ':='", word_stream[cursor].value);
		}
	}
	else {
		grammar_error("expect ID", word_stream[cursor].value);
	}
}

void read_expression()
{
	cursor++;	// read

	string codeline = "scanf(\"";

	if (word_stream[cursor].id == PUN_lbrkt) {	// 301 --> (
		cursor++;


		variable_list_ variable_list_val;
		attribute_value tmp;
		tmp.variable_list_val = variable_list_val;
		attribute_stack.push(tmp);

		sem_stack_item sem_tmp;	// krrkrr
		expression_type_list_ variable_type_list_val;	// krrkrr
		sem_tmp.expression_type_list = variable_type_list_val;	// krrkrr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krrkrr

		variable_list();

		variable_list_val = attribute_stack.top().variable_list_val;
		attribute_stack.pop();

		variable_type_list_val = sem_stack.top().expression_type_list;	// krrkrr
		sem_stack.pop();	// krrkrr

		//这个地方之后要根据语义分析的结果进行修改。(改了krrkrr)
		for (int i = 0; i < variable_list_val.size(); i++)
		{
			switch (variable_type_list_val.at(i)) {
			case INTEGER:
				codeline += "%d";	//int krrkrr
				break;
			case REAL:
				codeline += "%lf";	//double krrkrr
				break;
			case CHAR:
				codeline += "%c";	//char 写是这么写了，不过咱们能不能识别char？ krrkrr
				break;
			case STRR:
				codeline += "%s";	//char* 写是这么写了，不过咱们能不能识别string？ krrkrr
				break;
			default:
				codeline += "%d";	//其余奇奇怪怪的情况就还写%d吧 krrkrr
				break;
			}
		}
		codeline += "\"";

		for (int i = 0; i < variable_list_val.size(); i++)
		{
			string address = string("\&");
			codeline += (", "+ address + variable_list_val[i]);
			judge_read_item(variable_list_val[i], cursor);	// krrkrr
		}
		codeline += ");";

		append_code_line(codeline);

		if (word_stream[cursor].id == PUN_rbrkt) {	// 302 --> )
			cursor++;
		}
		else {
			grammar_error("expect ')'", word_stream[cursor].value, last_match(PUN_rbrkt));
		}
	}
	else {
		grammar_error("expect '('", word_stream[cursor].value);
	}
}

void write_expression()
{
	cursor++;	// write

	string codeline = "printf(\"";

	if (word_stream[cursor].id == PUN_lbrkt) {	// 301 --> (
		cursor++;

		attribute_value tmp;
		expression_list_ expression_list_val;
		tmp.expression_list_val = expression_list_val;
		attribute_stack.push(tmp);

		sem_stack_item sem_tmp;	// krrkrr
		expression_type_list_ expression_type_list_val;	// krrkrr
		sem_tmp.expression_type_list = expression_type_list_val;	// krrkrr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krrkrr

		expression_list();
		
		expression_list_val = attribute_stack.top().expression_list_val;
		attribute_stack.pop();

		expression_type_list_val = sem_stack.top().expression_type_list;	// krrkrr
		sem_stack.pop();	// krrkrr

		//这个地方之后要根据语义分析的结果进行修改。(改了krrkrr)
		for (int i = 0; i < expression_list_val.size(); i++)
		{
			switch (expression_type_list_val.at(i)) {
			case INTEGER:
				codeline += "%d";	//int krrkrr
				break;
			case REAL:
				codeline += "%lf";	//double krrkrr
				break;
			case CHAR:
				codeline += "%c";	//char 写是这么写了，不过咱们能不能识别char？ krrkrr
				break;
			case STRR:
				codeline += "%s";	//char* 写是这么写了，不过咱们能不能识别string？ krrkrr
				break;
			default:
				codeline += "%d";	//其余奇奇怪怪的情况就还写%d吧 krrkrr
				break;
			}
			if (i != expression_list_val.size() - 1)
			{
				codeline += " ";
			}
		}
		codeline += "\"";

		for (int i = 0; i < expression_list_val.size(); i++)
		{
			codeline += (", " + expression_list_val[i]);
		}
		codeline += ");";

		append_code_line(codeline);

		if (word_stream[cursor].id == PUN_rbrkt) {	// 302 --> )
			cursor++;
		}
		else {
			grammar_error("expect ')'", word_stream[cursor].value, last_match(PUN_rbrkt));
		}
	}
	else {
		grammar_error("expect '('", word_stream[cursor].value);
	}
}

// 变量列表
void variable_list()
{
	attribute_value tmp;
	tmp.sval = "";
	attribute_stack.push(tmp);

	sem_stack_item sem_tmp;	// krrkrr
	sem_tmp.expression_type = -2;	// krrkrr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krrkrr

	variable();

	string variable_val = attribute_stack.top().sval;
	attribute_stack.pop();

	int variable_type = sem_stack.top().expression_type;	// krrkrr
	sem_stack.pop();	// krrkrr

	attribute_stack.top().variable_list_val.push_back(variable_val);
	sem_stack.top().expression_type_list.push_back(variable_type);	// krrkrr


	if (word_stream[cursor].id == PUN_comma) {	// 303 --> ,
		cursor++;
		variable_list();
	}
}

void variable()
{
	string id = word_stream[cursor].value;
	cursor++; // 此时一定是id，故++
	if (word_stream[cursor].id == PUN_lsbrkt) {	// 308 --> [
		cursor++;

		attribute_value tmp;
		expression_list_ expression_list_val;
		tmp.expression_list_val = expression_list_val;
		attribute_stack.push(tmp);

		sem_stack_item sem_tmp;	// krr
		expression_type_list_ expression_type_list_val;	// krr
		sem_tmp.expression_type_list = expression_type_list_val;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		expression_list();

		expression_list_val = attribute_stack.top().expression_list_val;
		expression_type_list_val = sem_stack.top().expression_type_list;	// krr
		attribute_stack.pop();
		sem_stack.pop();	// krr

		if (word_stream[cursor].id == PUN_rsbrkt) {	// 309 --> ]
			cursor++;

			attribute_stack.top().sval += id;
			for (int i = 0; i < expression_list_val.size(); i++)
			{
				attribute_stack.top().sval += ("[" + expression_list_val[i] + "]");
			}
			if (!include_ch(attribute_stack.top().sval, '.')) {
				sem_stack.top().expression_type = judge_arrary_range(id, expression_type_list_val, cursor);	// krr
			}

		}
		else {
			grammar_error("expect ']'", word_stream[cursor].value, last_match(PUN_rsbrkt));
		}
	}
	else
	{
		if (replace_function_return(id, OTHER_GRAMMERS, cursor)) {
			attribute_stack.top().sval += "result";
		}
		else {
			attribute_stack.top().sval += id;
		}
		//如果这个变量和函数名一致，则变量名改为result
		//语义分析说：改的时候别直接改id





		if (!include_ch(attribute_stack.top().sval, '.')) {
			sem_stack.top().expression_type = judge_var(id, cursor);	// krr
		}
		//szl//cout << "[variable:" << attribute_stack.top().sval << endl;
	}

	if (word_stream[cursor].id == PUN_dot) {
		cursor++;
		// 翻译方案
		attribute_stack.top().sval +=  ".";

		//sem_stack.top().expression_type = MERVEILLEUS;

		variable();

		if (!attribute_stack.top().sval.empty())
		{
			if (sem_stack.top().valid_flag) {	//如果还没出错
				pair<bool, int> judge_pair = judge_record(attribute_stack.top().sval, cursor);
				sem_stack.top().valid_flag = judge_pair.first;	// krr
				sem_stack.top().expression_type = judge_pair.second;
			}
		}
	}
}

/*
void record()
{
	if (word_stream[cursor].id == PUN_dot) {
		// 翻译方案
		cursor++;

		if (word_stream[cursor].id == ID) {
			// 翻译方案
			attribute_stack.top().sval += ("." + word_stream[cursor].value);
			cursor++;

		}
		else {
			grammar_error("Lack of ID");
		}
		record();
	}
}
*/

// 表达式列表
void expression_list()
{	
	attribute_value tmp;
	tmp.sval = "";
	attribute_stack.push(tmp);
	sem_stack_item sem_tmp;	// krr
	sem_tmp.expression_type = -2;	// krr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krr

	expression();	

	string expression_val = attribute_stack.top().sval;
	attribute_stack.pop();
	//szl//cout << "{[" << expression_val << "]]}" << endl;
	int expression_type = sem_stack.top().expression_type;	// krr
	sem_stack.pop();	// krr

	attribute_stack.top().expression_list_val.push_back(expression_val);
	sem_stack.top().expression_type_list.push_back(expression_type);	// krr


	if (word_stream[cursor].id == PUN_comma) {	// 303 --> ,
		cursor++;
		expression_list();
	}
}

void expression()
{
	int expression_type_l = -1;	// krr
	int expression_type_r = -1;	// krr

	sem_stack_item sem_tmp;	// krr
	sem_tmp.expression_type = -2;	// krr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krr

	simple_expression();

	expression_type_l = sem_stack.top().expression_type;	// krr
	sem_stack.pop();	// krr

	if (word_stream[cursor].id == OPR_great) { 
		attribute_stack.top().sval += " > ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, ">", cursor);	// krr
	}
	else if (word_stream[cursor].id == OPR_less) {
		attribute_stack.top().sval += " < ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "<", cursor);	// krr
	}
	else if (word_stream[cursor].id == OPR_gequal) {
		attribute_stack.top().sval += " >= ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, ">=", cursor);	// krr
	}
	else if (word_stream[cursor].id == OPR_lequal) {
		attribute_stack.top().sval += " <= ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "<=", cursor);	// krr
	}
	else if (word_stream[cursor].id == OPR_nequal) {
		attribute_stack.top().sval += " != ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "!=", cursor);	// krr
	}
	else if (word_stream[cursor].id == OPR_equal) {
		attribute_stack.top().sval += " == ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "==", cursor);	// krr
	}
	else {
		sem_stack.top().expression_type = expression_type_l;
	}

}

void simple_expression()
{
	int expression_type_l = -1;	// krr
	int expression_type_r = -1;	// krr

	sem_stack_item sem_tmp;	// krr
	sem_tmp.expression_type = -2;	// krr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krr

	term();

	expression_type_l = sem_stack.top().expression_type;	// krr
	sem_stack.pop();	// krr

	// 分着写是为了语义分析方便
	if (word_stream[cursor].id == OPR_add) { // 203 4 --> + -
		attribute_stack.top().sval += " + ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "+", cursor);	// krr
	}
	else if(word_stream[cursor].id == OPR_sub){
		attribute_stack.top().sval += " - ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "-", cursor);	// krr
	}
	else if (word_stream[cursor].id == KEY_or) {	// 122 --> or
		attribute_stack.top().sval += " || ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		simple_expression();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "||", cursor);	// krr
	}
	else {
		sem_stack.top().expression_type = expression_type_l;
	}
}

void term()
{
	int expression_type_l = -1;	// krr
	int expression_type_r = -1;	// krr

	sem_stack_item sem_tmp;	// krr
	sem_tmp.expression_type = -2;	// krr
	sem_tmp.valid_flag = true;
	sem_stack.push(sem_tmp);	// krr

	factor();

	expression_type_l = sem_stack.top().expression_type;	// krr
	sem_stack.pop();	// krr

	// 分着写条件是为了语义分析方便
	if (word_stream[cursor].id == OPR_mul) { // 210 --> *
		attribute_stack.top().sval += " * ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		term();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "*", cursor);	// krr
	}
	else if (word_stream[cursor].id == OPR_div) {	// 211 --> /
		attribute_stack.top().sval += " / ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		term();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "/", cursor);	// krr
	}
	else if (word_stream[cursor].id == KEY_div) {	// 210 --> div
		attribute_stack.top().sval += " / ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		term();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "/", cursor);	// krr
	}
	else if (word_stream[cursor].id == KEY_mod) {	// 210 --> mod
		attribute_stack.top().sval += " % ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		term();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "%", cursor);	// krr
	}
	else if (word_stream[cursor].id == KEY_and) {	// 210 --> and
		attribute_stack.top().sval += " && ";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		term();

		expression_type_r = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = judge_lr_expression_type(expression_type_l, expression_type_r, "&&", cursor);	// krr
	}
	else {
		sem_stack.top().expression_type = expression_type_l;	// krr
	}
}

void factor()
{
	// factor -> num
	if (word_stream[cursor].id == INT) {	// 2 --> num
		attribute_stack.top().sval += word_stream[cursor].value;
		sem_stack.top().expression_type = INTEGER;	// krr
		cursor++;
	}
	else if (word_stream[cursor].id == REL) {
		attribute_stack.top().sval += word_stream[cursor].value;
		sem_stack.top().expression_type = REAL;	// krr
		cursor++;
	}
	else if (word_stream[cursor].id == CHR) { //jn add
		attribute_stack.top().sval += word_stream[cursor].value;
		sem_stack.top().expression_type = CHAR;	// krr
		cursor++;
	}
	else if (word_stream[cursor].id == STR) {

		string value = word_stream[cursor].value;
		value[0] = '\"';
		value[value.size() - 1] = '\"';//'改为"
		attribute_stack.top().sval += value;
		sem_stack.top().expression_type = STRR;	// krr
		cursor++;
	}
	// factor -> variable | id ( expression_list )
	else if (word_stream[cursor].id == ID) {
		string id = word_stream[cursor].value;
		cursor++;	// ++前是id
		if (word_stream[cursor].id == PUN_lbrkt) {	// 301 --> (
			attribute_stack.top().sval += id;
			attribute_stack.top().sval += "(";
			cursor++;

			attribute_value tmp;
			expression_list_ expression_list_val;
			tmp.expression_list_val = expression_list_val;
			attribute_stack.push(tmp);

			sem_stack_item sem_tmp;	// krr
			expression_type_list_ expression_type_list_val;	// krr
			sem_tmp.expression_type_list = expression_type_list_val;	// krr
			sem_tmp.valid_flag = true;
			sem_stack.push(sem_tmp);	// krr

			expression_list();

			expression_list_val = attribute_stack.top().expression_list_val;
			expression_type_list_val = sem_stack.top().expression_type_list;	// krr
			attribute_stack.pop();
			sem_stack.pop();	// krr

			
			for (int i = 0; i < expression_list_val.size(); i++)
			{
				attribute_stack.top().sval += expression_list_val[i];
				if (i != expression_list_val.size() - 1)
				{
					attribute_stack.top().sval += ", ";
				}
			}

			if (word_stream[cursor].id == PUN_rbrkt) {	// 302 --> )
				attribute_stack.top().sval += ")";
				cursor++;

				sem_stack.top().expression_type = judge_func_and_proc(id, expression_type_list_val, cursor);	// krr57
				//sem_stack.top().expression_type = id对应函数的返回值类型;
				// 这里只需检索符号表，检查函数/过程名是否存在；若存在，检查参数数量和类型是否合法
				// HINT: 我需要什么？—— 函数/过程名字：id = word_stream[cursor].value，参数类型列表：expression_type_list_val
			}
			else {
				grammar_error("expect')'", word_stream[cursor].value, last_match(PUN_rbrkt));
			}
		}
		else {
			cursor--;	// variable函数会做一次自加操作，所以这里自减
			attribute_value tmp;
			tmp.sval = "";
			attribute_stack.push(tmp);

			sem_stack_item sem_tmp;	// krr
			sem_tmp.expression_type = -2;	// krr
			sem_tmp.valid_flag = true;
			sem_stack.push(sem_tmp);	// krr

			variable();

			string variable_val = attribute_stack.top().sval;
			attribute_stack.pop();

			int expression_type = sem_stack.top().expression_type;	// krr
			sem_stack.pop();	// krr

			sem_stack.top().expression_type = expression_type;	// krr
			attribute_stack.top().sval += variable_val;

		}
	}
	// factor -> ( expression )
	else if (word_stream[cursor].id == PUN_lbrkt) {	// 301 --> (
		attribute_stack.top().sval += "(";
		cursor++;

		attribute_value tmp;
		tmp.sval = "";
		attribute_stack.push(tmp);

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		expression();

		string expression_val = attribute_stack.top().sval;
		attribute_stack.pop();

		int expression_type = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		attribute_stack.top().sval += expression_val;
		sem_stack.top().expression_type = expression_type;	// krr

		if (word_stream[cursor].id == PUN_rbrkt) {	// 302 --> )
			attribute_stack.top().sval += ")";
			cursor++;
		}
		else {
			grammar_error("expect')'", word_stream[cursor].value, last_match(PUN_rbrkt));
		}
	}
	// factor -> not factor
	else if (word_stream[cursor].id == KEY_not) {	// 126 --> not
		attribute_stack.top().sval += "!";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		factor();

		int expression_type = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = expression_type;	// krr
	}
	// factor -> uminus factor
	else if (word_stream[cursor].id == OPR_sub) {	// 204 --> -
		attribute_stack.top().sval += "-";
		cursor++;

		sem_stack_item sem_tmp;	// krr
		sem_tmp.expression_type = -2;	// krr
		sem_tmp.valid_flag = true;
		sem_stack.push(sem_tmp);	// krr

		factor();

		int expression_type = sem_stack.top().expression_type;	// krr
		sem_stack.pop();	// krr

		sem_stack.top().expression_type = expression_type;	// krr
	}
	else if (word_stream[cursor].id == KEY_true) {	// 204 --> -
		attribute_stack.top().sval += "True";
		sem_stack.top().expression_type = BOOLEAN;
		cursor++;
	}
	else if (word_stream[cursor].id == KEY_false) {	// 204 --> -
		attribute_stack.top().sval += "False";
		sem_stack.top().expression_type = BOOLEAN;
		cursor++;
	}
	else {
		sem_stack.top().expression_type = -1;
		grammar_error("Wrong factor");
	}
}

void init_grammar()
{
	cursor = 0;
	if (!attribute_stack.empty())
	{
		attribute_stack.pop();
	}

	if (!sem_stack.empty())
	{
		sem_stack.pop();
	}
}