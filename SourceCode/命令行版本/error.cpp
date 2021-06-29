#include "head.h"



void append_error(int stage, string error_code, int row, int col, string content)
{
		error_item tmp;
		tmp.stage = stage;
		tmp.row = row;
		tmp.col = col;
		tmp.content = content;
		tmp.error_code = error_code;
		error_stream.push_back(tmp);

}
