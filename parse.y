%{
	void yyerror(char * s);
	extern int yylex();
	#include "common.h"
	#include "const_record.h"
	#include "type_record.h"
	#include "var_record.h"
	#include "stmt.h"
	#include "expr.h"
	#include "type_value.h"
	#include "routine.h"
	#include "enviroment.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include "string_data.h"	
%}

%union{
	char * _str;
	int _int;
	double _double;
	base_type * _type;
	char _char;
	key_value_tuple * _tuple;
	vector <string> * _vt;
	vector < pair <string, type_ptr> > * _field;
	base_expr * _expr;
	base_stmt * _stmt;
	int _direction;
	case_expr_list * _case_list;
	case_expr * _case_expr;
	stmt_list * _stmt_list;
	routine * _routine;
	const_record * _c_r;
	var_record * _v_r;
	type_record * _t_r;
	routine_head * _r_h;
	routine_record * _r_r;
	procedure * _procedure_decl;
	func * _function;
	pair<int, pair < vector <string>, type_ptr > > * _para_type_list;
	vector < pair<int, pair <vector <string>, type_ptr> > > * _para_decl_list;
	vector < shared_ptr <base_expr> > *  _args_list;
	proc_stmt * _proc_stmt;
}

%token  LP RP LB RB DOT COMMA COLON MUL DIV PLUS MINUS ID GE GT LE LT EQUAL ASSIGN INTEGER REAL CHAR STRING CONST SEMI VAR PROGRAM TYPE SYS_TYPE RECORD ARRAY BP END BEGINN MOD UNEQUAL DR NOT AND CASEE IFF DOO TO DOWNTO UNTIL FOR WHILE ELSEE OF REPEAT GOTO THEN PROCEDURE FUNCTION SYS_PROC WRITELN

%type <_str> ID
%type <_tuple> const_value
%type <_int> INTEGER
%type <_double> REAL
%type <_char> CHAR
%type <_str> STRING
%type <_type> array_type_decl
%type <_type> type_decl
%type <_type> record_type_decl
%type <_str> SYS_TYPE
%type <_type> simple_type_decl
%type <_field> field_decl
%type <_field> field_decl_list
%type <_vt> name_list
%type <_expr> term
%type <_expr> factor
%type <_expr> expr
%type <_expr> expression
%type <_stmt> assign_stmt
%type <_stmt> non_label_stmt
%type <_stmt> stmt
%type <_stmt_list> else_clause
%type <_stmt> if_stmt
%type <_stmt> for_stmt
%type <_stmt> while_stmt
%type <_stmt> goto_stmt
%type <_stmt> repeat_stmt
%type <_stmt> case_stmt
%type <_stmt> sys_stmt
%type <_direction> direction
%type <_stmt_list> stmt_list
%type <_case_list> case_expr_list
%type <_case_expr> case_expr
%type <_c_r> const_part
%type <_c_r> const_expr_list
%type <_t_r> type_decl_list
%type <_t_r> type_part
%type <_v_r> var_decl_list
%type <_v_r> var_part
%type <_stmt_list> routine_body
%type <_r_h> routine_head
%type <_routine> routine
%type <_r_r> routine_part
%type <_vt> var_para_list
%type <_vt> val_para_list
%type <_routine> sub_routine
%type <_para_type_list> para_type_list
%type <_para_decl_list> para_decl_list
%type <_procedure_decl> procedure_decl
%type <_para_decl_list> 	parameters
%type <_function> function_decl
%type <_args_list> args_list
%type <_proc_stmt> proc_stmt
%%
program : PROGRAM ID SEMI routine DOT{
	$4 -> name = $2;
	if(flag){
		return 1;
	}
	enviroment::single() -> clear();
	$4 -> gencode();
	return 0;
}
routine: routine_head routine_body{
	$$ = new routine();
	$$ -> header.reset($1);
	$$ -> stmt_vt.reset($2);
}
routine_head: const_part{
	$$ = new routine_head();
	$$ -> c_r.reset($1);
	enviroment::single() -> insert($$);
	} type_part {
		enviroment::single() -> top() -> t_r.reset($3);
	}var_part {
	enviroment::single() -> top() -> v_r.reset($5);
	}
	routine_part{
	enviroment::single() -> top() -> r_r.reset($7);
	$$ = enviroment::single() -> top();
	}
;
sub_routine: routine_head routine_body{
		$$ = new routine();
		$$ -> header.reset($1);
		$$ -> stmt_vt.reset($2);
		enviroment::single() -> pop();
	}

const_part : CONST  const_expr_list 
		{
			$$ = $2;
		}
		|{
		 	$$ = new const_record();
		}
		;
const_expr_list: const_expr_list  ID EQUAL const_value SEMI {
		$1 -> insert($2, $4 -> first, $4 -> second);
		$$ = $1;
	}
	| ID EQUAL const_value SEMI {
		$$ = new const_record();
		$$ -> insert($1, $3 -> first, $3 -> second);
	}
	;
const_value: INTEGER {
		value_set value;
		value._int = $1;
		type_ptr ptr(new base_type(INT_TYPE));
		$$ = new key_value_tuple();
		$$ -> first = ptr;
		$$ -> second = value;
}
	| REAL{
		value_set value;
		value._double = $1;
		type_ptr ptr(new base_type(REAL_TYPE));
		$$ = new key_value_tuple();
		$$ -> first = ptr;
		$$ -> second = value;
	}
	| CHAR{
		value_set value;
		value._char = $1;
		type_ptr ptr(new base_type(CHAR_TYPE));
		$$ = new key_value_tuple();
		$$ -> first = ptr;
		$$ -> second = value;
	}
	| STRING{
		value_set value;
		value._str = $1;
		type_ptr ptr(new base_type(STR_TYPE));
		$$ = new key_value_tuple();
		$$ -> first = ptr;
		$$ -> second = value;
	}
	;
type_part: TYPE type_decl_list{
		$$ = $2;
	}
	|{
		 	$$ = new type_record();
		}
	;
name_list: name_list COLON ID {
		$1 -> push_back($3);
		$$ = $1;
	}
	| ID{
		$$ = new vector <string> ();
		$$ -> push_back($1);
	}
	;
type_decl_list : type_decl_list ID EQUAL type_decl SEMI{
		shared_ptr <base_type> tmp($4);
		$1 -> insert($2, tmp);	
		$$ = $1;
	}	
	| ID EQUAL type_decl SEMI{
		$$ = new type_record();
		shared_ptr <base_type> tmp($3);
		$$ -> insert($1, tmp);	
	}
	;
type_decl: simple_type_decl {
		$$ = $1;
	}
	| array_type_decl{
		$$ = $1;
	}
	| record_type_decl{
		$$ = $1;
	}
	;
array_type_decl : ARRAY LB simple_type_decl RB OF type_decl{
		arr_type * tmp = new arr_type();
		tmp -> nxt.reset($6);
		tmp -> index.reset($3);
		$$ = tmp;
	}
	;
record_type_decl: RECORD field_decl_list END{
		$$ = new record_type();
		((record_type * )$$) -> vt = *$2;
	}
	;
field_decl_list: field_decl_list field_decl {
		for(auto i = $2 -> begin(); i != $2 -> end(); ++i){
			$1 -> push_back(*i);
		}
		delete $2;
		$$ = $1;
	}
	| field_decl{
		$$ = $1;
	}
	;
field_decl: name_list  COMMA type_decl SEMI{
		$$ = new vector < pair <string, type_ptr> >();
		for(auto i = $1 -> begin(); i != $1 -> end(); ++i){
			type_ptr tmp($3);
			$$ -> push_back(make_pair(*i, tmp));
		}
	}
	;

simple_type_decl : SYS_TYPE
	{
		if(strcmp($1, "integer") == 0){
			$$ = new base_type(INT_TYPE);
		}
		else
			if(strcmp($1, "real") == 0){
				$$ = new base_type(REAL_TYPE);
			}
			else
			if(strcmp($1, "char") == 0){
				$$ = new base_type(CHAR_TYPE);
			}
			else
			if(strcmp($1, "string") == 0){
				$$ = new base_type(STR_TYPE);
			}
	}
	| LP name_list RP

	{
		discrete_type * tmp = new discrete_type();
		for(auto i = $2 -> begin(); i != $2 -> end(); ++i){
			tmp -> index.push_back(*i);
		}
		$$ = tmp;
	}
	| const_value DOT DOT const_value{
		continue_type * tmp = new continue_type();
		tmp -> left = *$1;
		tmp -> right = *$4;
		$$ = tmp;
		delete $1;
		delete $4;
	}
	| MINUS const_value DOT DOT const_value{
			if($2 -> first -> gettype() == INT_TYPE){
				$2 -> second._int = -$2 -> second._int;
			}
			if($2 -> first -> gettype() == REAL_TYPE){
				$2 -> second._double = -$2 -> second._double;
			}
			continue_type * tmp = new continue_type();
			tmp -> left = *$2;
			tmp -> right = *$5;
			$$ = tmp;
			delete $5;
			delete $2;
	}
	| MINUS const_value DOT DOT MINUS const_value{
			if($2 -> first -> gettype() == INT_TYPE){
				$2 -> second._int = -$2 -> second._int;
			}
			if($2 -> first -> gettype() == REAL_TYPE){
				$2 -> second._double = -$2 -> second._double;
			}
			if($6 -> first -> gettype() == INT_TYPE){
				$6 -> second._int = -$6 -> second._int;
			}

			if($6 -> first -> gettype() == REAL_TYPE){
				$6 -> second._double = -$6 -> second._double;
			}
			continue_type * tmp = new continue_type();
			tmp -> left = *$2;
			tmp -> right = *$6;
			$$ = tmp;
			delete $2;
			delete $6;
	}
	;



var_part : VAR  var_decl_list {
		$$ = $2;
	}
	| {
		 	$$ = new var_record();
		}
	;
var_decl_list : var_decl_list name_list COMMA type_decl SEMI{
	for(auto i = $2 -> begin(); i != $2 -> end(); ++i){
			type_ptr tmp($4);
			if(!$1 -> insert(*i, tmp)){
				yyerror("var_decl error");
			} 
		}	
	$$ = $1;
	}
	| var_decl_list name_list COMMA ID SEMI{
		auto res= enviroment::single() -> searchtype($4);	if(res == nullptr){
		cerr << lineno << ": type not found: " << $4 << endl;
		}	
		else{	
		for(auto i = $2 -> begin(); i != $2 -> end(); ++i){
			if(!$1 -> insert(*i, res)){
				yyerror("var_decl error");
			}
		}	
		}
	$$ = $1;
	}
	| name_list COMMA ID SEMI{
		$$ = new var_record();
		auto res = enviroment::single() -> searchtype($3);
		if(res == nullptr){
		cerr << lineno << ": type not found: " << $3 << endl;
		}	
		else{
		for(auto i = $1 -> begin(); i != $1 -> end(); ++i){
			if(!$$ -> insert(*i, res)){
				yyerror("var_decl error");
			} 
			}	
		}
	}
	| name_list COMMA type_decl SEMI{
		$$ = new var_record();
	for(auto i = $1 -> begin(); i != $1 -> end(); ++i){
			type_ptr tmp($3);
			if(!$$ -> insert(*i, tmp)){
				yyerror("var_decl error");
			} 
		}	
	}
	;

routine_body: BEGINN stmt_list END{
		$$ = $2;
	}
	;

expression_list : expression_list COMMA expression
	| expression
	;

expression: expression GE expr{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = GE_TYPE;
		$$ = tmp;
	}
	| expression GT expr{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = GT_TYPE;
		$$ = tmp;
	}
	| expression LE expr{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = LE_TYPE;
		$$ = tmp;
	}
	| expression LT expr{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = LT_TYPE;
		$$ = tmp;	
	}
	| expression EQUAL expr{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = EQUAL_TYPE;
		$$ = tmp;
	}
	| expression UNEQUAL expr{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = UNEQUAL_TYPE;
		$$ = tmp;
	}
	| expr{
		$$ = $1;
	}
	;

expr: expr PLUS term {
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = PLUS_TYPE;
		$$ = tmp;
	}
	| expr MINUS term{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = MINUS_TYPE;
		$$ = tmp;
	}
	| term{
		$$ = $1;
	}
	;

term: term MUL factor{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = MUL_TYPE;
		$$ = tmp;
	}
	| term DIV factor{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = DIV_TYPE;
		$$ = tmp;
	}
	| term MOD factor{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = MOD_TYPE;
		$$ = tmp;
	}
	| term AND factor{
		binary_expr * tmp = new binary_expr();
		tmp -> lchild.reset($1);
		tmp -> rchild.reset($3);
		tmp -> op = AND_TYPE;
		$$ = tmp;
	}
	| factor{
		$$ = $1;
	}
	;

factor: ID {
		auto res1 = enviroment::single() -> searchconst($1);
		if(res1.first == nullptr){
			auto res2 = enviroment::single() -> search($1);
			id_node_value * tmp = new id_node_value();
			tmp -> id = $1;
			$$ = tmp;
		}
		//const replace
		else{
			leaf_node_value * tmp = new leaf_node_value();
			tmp -> type_id = res1.first -> gettype();
			tmp -> value = res1.second;
			$$ = tmp;
		}
	}
	| ID LP args_list RP{		
          	proc_stmt * tmp= new proc_stmt();
          	tmp -> proc_id = $1;
          	tmp -> param = *$3;
          	delete $3;
          	func_node_value * tmp2 = new func_node_value();
          	tmp2 -> func_stmt.reset(tmp);
	    	$$ = tmp2;  
	    }
	| LP expression RP{
		$$ = $2;
	}
	| NOT factor{
		unary_expr * tmp = new unary_expr();
		tmp -> child.reset($2);
		tmp -> op = NOT_TYPE;
		$$ = tmp;
	}
	| MINUS factor{
		unary_expr * tmp = new unary_expr();
		tmp -> child.reset($2);
		tmp -> op = MINUS_TYPE;
		$$ = tmp;
	}
	| ID LB expression RB{
		arr_node_value * tmp = new arr_node_value();
		tmp -> id = $1;
		tmp -> index.reset($3);
		$$ = tmp;
	}	
	| ID DOT ID{
		record_node_value * tmp = new record_node_value();
		tmp -> id = $1;
		tmp -> member = $3;
		$$ = tmp;
	}
	|
	INTEGER{
		leaf_node_value * tmp = new leaf_node_value();
		tmp -> type_id = INT_TYPE;
		tmp -> value._int = $1;
		$$ = tmp;
	}	
	|
	REAL{
		leaf_node_value * tmp = new leaf_node_value();
		tmp -> type_id = REAL_TYPE;
		tmp -> value._double = $1;
		$$ = tmp;
	}
	|STRING{
		string str = string_data::single() -> insert($1);
		leaf_node_value * tmp = new leaf_node_value();
		tmp -> type_id = STR_TYPE;
		tmp -> value._str = strdup(str.c_str());
		$$ = tmp;
	}
	;

arg_list: arg_list COLON expression
	| expression
	;



stmt_list: stmt_list stmt SEMI{
		shared_ptr<base_stmt> tmp($2);
		$1 -> vt.push_back(tmp);
		$$ = $1;
	}
	|
	{
		$$ = new stmt_list();
	}
	;

stmt: INTEGER COLON non_label_stmt{
		$$ = $3;
		$$ -> label = $1;
		labelmanager::store($1);
	} | non_label_stmt{
	}

non_label_stmt:
	assign_stmt{
		$$ = $1;
	}
	| if_stmt{
		$$ = $1;
	}
	| repeat_stmt{
		$$ = $1;
	}
	| for_stmt{
		$$ = $1;

	}
	| while_stmt{
		$$ = $1;

	}
	| case_stmt{
		$$ = $1;

	}
	| goto_stmt{
		$$ = $1;

	}
	| proc_stmt {
		$$ = $1;
	}
	| sys_stmt{

	}
	;

assign_stmt: ID ASSIGN expression{
		normal_assign * tmp = new normal_assign();	
		tmp -> value.reset($3);
		tmp -> id = $1;
		$$ = tmp;
	}
	| ID LB expression RB ASSIGN expression{
		arr_assign * tmp = new arr_assign();
		tmp -> id = $1;
		tmp -> index.reset($3);
		tmp -> value.reset($6);
		$$ = tmp;
	}
	| ID DOT ID ASSIGN expression{
		record_assign * tmp = new record_assign();
		tmp -> id = $1;
		tmp -> member = $3;
		tmp -> value.reset($5);
		$$ = tmp;
	}
	;
if_stmt : IFF expression THEN  routine_body else_clause{
		if_stmt * tmp = new if_stmt();
		tmp -> judge.reset($2);
		tmp -> lchild.reset($4);
		tmp -> rchild.reset($5);
		$$ = tmp;
	}
		;
else_clause : ELSEE routine_body{
		$$ = $2;
	}
		|{
		 $$ = nullptr;
		}
		;

repeat_stmt : REPEAT stmt_list UNTIL expression{
		repeat_stmt * tmp = new repeat_stmt();
		tmp -> stmt_vt.reset($2);
		tmp -> judge.reset($4);
		$$ = tmp;
	}
		;

while_stmt : WHILE expression DOO BEGINN stmt_list END{
		while_stmt * tmp = new while_stmt();
		tmp -> judge.reset($2);
		tmp -> stmt.reset($5);
		$$ = tmp;
	};

for_stmt : FOR ID ASSIGN expression direction expression DOO routine_body {
		
		for_stmt * tmp = new for_stmt();
		tmp -> id = $2;
		tmp -> end.reset($6);
		tmp -> start.reset($4);
		tmp -> dic = $5;
		tmp -> stmt.reset($8);
		$$ = tmp;
	}
		;

direction : TO{
			$$ = 0;
		}
		| DOWNTO{
			$$ = 1;
		}
		;

case_stmt : CASEE expression OF case_expr_list 
		END{
			if(!$2 -> isleaf() && !$2 -> checktype($2 -> isdouble())){
				cout << lineno << ": type not match" << endl;
				flag = true;
			}
			case_stmt * tmp = new case_stmt();
			tmp -> expr.reset($2);
			tmp -> case_list.reset($4);
			$$ = tmp;
		}
		;
case_expr_list : case_expr_list  case_expr{
		shared_ptr <case_expr> tmp($2);
		$1 -> case_vt.push_back(tmp);
		$$ = $1;
	}
		| case_expr{
			shared_ptr <case_expr> tmp($1);
			$$ = new case_expr_list();
			$$ -> case_vt.push_back(tmp);
		}
		;

case_expr : const_value
 COMMA stmt SEMI{	
		case_expr * tmp = new case_expr();
		tmp -> value.reset($1);
		tmp -> stmt.reset($3);
		$$ = tmp;
	}
		| ID COMMA stmt SEMI{
			auto res = enviroment::single() -> searchconst($1);
			case_expr * tmp = new case_expr();
			key_value_tuple * tp = new key_value_tuple();
			tp -> first = res.first;
			tp -> second = res.second;
			tmp -> value.reset(tp);
			tmp -> stmt.reset($3);
			$$ = tmp;
		}
		;

goto_stmt : GOTO INTEGER{
		goto_stmt * tmp = new goto_stmt();
		tmp -> addr = $2;
		$$ = tmp;
	}
	;

proc_stmt : ID{
          	$$ = new proc_stmt();
          	$$ -> proc_id = $1;
          }
          | ID LP args_list RP{
          	$$ = new proc_stmt();
          	$$ -> proc_id = $1;
          	$$ -> param = *$3;
          	delete $3;
          }
          | SYS_PROC LP args_list RP
          ;
sys_stmt : WRITELN LP expression RP{
		sys_write_stmt * tmp = new sys_write_stmt();
		tmp -> expr.reset($3);
		$$ = tmp;
	}
args_list : args_list  COLON  expression{
	shared_ptr <base_expr> tmp($3);
	$1 -> push_back(tmp);
	$$ = $1;
	}
	|expression{
	$$ = new vector < shared_ptr <base_expr> > ();
	shared_ptr <base_expr> tmp($1);
	$$ -> push_back(tmp);
	}
	;
routine_part : routine_part procedure_decl{
		shared_ptr <routine> tmp($2);
		$1 -> vt.push_back(tmp);
		$$ = $1;
	}
	|routine_part function_decl{
		shared_ptr <routine> tmp($2);
		$1 -> vt.push_back(tmp);
		$$ = $1;
	}
		| {
			$$ = new routine_record();
		}
		;
procedure_decl : PROCEDURE ID  parameters SEMI sub_routine SEMI{
		$$ = new procedure();
		$$ -> name = $2;
		$$ -> header = $5 -> header;
		$$ -> stmt_vt = $5 -> stmt_vt;
		for(auto i = $3 -> begin(); i != $3 -> end(); ++i){
			for(auto j = i -> second.first.begin(); j != i -> second.first.end(); ++j){
				$$ -> param.push_back(make_pair(i -> first, make_pair(*j, i -> second.second)));
			}
		}
		$$ -> add_function_param();
	}
		;


parameters : LP para_decl_list RP{
		$$ = $2;
	}
	|{
		$$ = new vector < pair<int, pair <vector <string>, type_ptr> > >();
	}
	;

para_decl_list : para_decl_list  SEMI para_type_list {
		$1 -> push_back(*$3);
		$$ = $1;
	}
		| para_type_list{
			$$ = new vector < pair<int, pair <vector <string>, type_ptr> > >();
			$$ -> push_back(*$1);
		}
		;

para_type_list : var_para_list COMMA  simple_type_decl{
			$$ = new pair<int, pair <vector <string>, type_ptr> >();
			$$ -> first = 1;
			point_type * tmp = new point_type();
			tmp -> nxt.reset($3);
			$$ -> second.first = *$1;
			$$ -> second.second.reset(tmp);
		}
		| val_para_list COMMA simple_type_decl{
			$$ = new pair<int, pair <vector <string>, type_ptr> >();
			$$ -> first = 0; 
			$$ -> second.first = *$1;
			$$ -> second.second.reset($3);
		}
		;

var_para_list : VAR name_list{
		$$ = $2;		
	}
		;

val_para_list : name_list{
		$$ = $1;
	}
		;


function_decl :  FUNCTION  ID  parameters  COMMA  				simple_type_decl   SEMI  sub_routine  SEMI{
		$$ = new func();
		$$ -> name = $2;
		$$ -> header = $7 -> header;
		$$ -> stmt_vt = $7 -> stmt_vt;
		for(auto i = $3 -> begin(); i != $3 -> end(); ++i){
			for(auto j = i -> second.first.begin(); j != i -> second.first.end(); ++j){
				$$ -> param.push_back(make_pair(i -> first, make_pair(*j, i -> second.second)));
			}
		}
		$$ -> ret.reset($5);
		$$ -> add_function_param();
	}
	;
%%
void yyerror(char * s){
	printf("%s\n", s);
	exit(0);
}
int main()
{
	yyparse();
}
 


