program err3(input,output);{参与运算的表达式类型不匹配}
var 
	i : real;
	j : integer;
	k : char;
begin
	i := 1;
	k := 'c';
	j := i+k;
end.
{ [Error type 3] line 9 : expression type wrong: double+char
 [Error type 3] line 9 : assignment to int from someting have no value}