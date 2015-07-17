%{
    #include "const_record.h"
	#include "string_data.h"
    #include "type_record.h"
	#include "var_record.h"
	#include "stmt.h"
    #include "common.h"
    #include "expr.h"
    #include "routine.h"
    #include "y.tab.h"
    extern int lineno = 0;
    extern bool flag = false;
%}

const "const"
var "var"
lp "(" 
rp ")"
lb "["
rb "]" 
dot "." 
comma ":"
colon "," 
mul "*"	
div "/" 
plus "+"
minus "-"
id [a-zA-Z][a-zA-Z0-9_]*
ge ">="  
gt ">"
le "<="
lt "<" 
mod "%"
writeln "writeln"
sys_proc read|write
function "function"
equal "="
assign ":=" 
integer [0-9]+ 
real [0-9]+"."[0-9]+ 
char "'"."'" 
string "'".*"'" 
semi ";" 
program "program"
type "type"
sys_type integer|string|real|char
record "record"
end "end"
array "array"
begin "begin"
casee "case"
iff "if"
doo "do"
to "to"
downto "downto"
until "until"
for "for"
while "while"
elsee "else"
repeat "repeat"
of "of"
goto "goto"
then "then"
procedure "procedure"
%%

{const}  {return CONST;}
{function} {return FUNCTION;}
{writeln} {return WRITELN;}
{program} {return PROGRAM;}
{sys_type} {yylval._str = strdup(yytext);return SYS_TYPE;}
{record} {return RECORD;}
{casee} {return CASEE;}
{mod} {return MOD;}
{iff} {return IFF;}
{doo} {return DOO;}
{elsee} {return ELSEE;}
{procedure} {return PROCEDURE;}
{of} {return OF;}
{repeat} {return REPEAT;}
{goto} {return GOTO;}
{then} {return THEN;}
{to} {return TO;}
{while} {return WHILE;}
{for} {return FOR;}
{until} {return UNTIL;}
{downto} {return DOWNTO;}
{end} {return END;}
{begin} {return BEGINN;}
{type} {return TYPE;}
{array} {return ARRAY;}
{var} {return VAR;}
{plus}  {return PLUS;}
{minus}  {return MINUS;}
{lp}  {return LP;}
{rp}  {return RP;}
{lb} {return LB;}
{rb} {return RB;}
{dot}  {return DOT;}
{comma} {return COMMA;}
{colon}  {return COLON;}
{mul}	{return MUL;}
{div} {return DIV;}
{id} {yylval._str = strdup(yytext);return ID;}
{ge}  {return GE;}
{gt}  {return GT;}
{le}  {return LE;}
{lt}  {return LT;}
{equal} {return EQUAL;}
{assign} {return ASSIGN;}
{integer}  {yylval._int = atoi(yytext);return INTEGER;}
{real} {yylval._double = atof(yytext);return REAL;}
{char}  {yylval._char = yytext[1];return CHAR;}
{string}  {
	int l = strlen(yytext);
	yylval._str = new char[l - 2];
	strncpy(yylval._str, yytext + 1, l - 2);return STRING;}
{semi} {return SEMI;}
\n {lineno++;}
\t {}
" " {}
. {return yytext[0];}

%%

int yywrap(){
	return 1;
}
