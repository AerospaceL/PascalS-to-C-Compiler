program err2(input,output);{数组越界}
var a1 : array[0..2] of int;
begin
 a1[3] := 1;
 end.
 {无报错}{对于数组越界，C语言和Pascal语言编译器都不会报错}