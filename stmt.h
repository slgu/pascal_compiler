#pragma once
#include "expr.h"
#include "labelmanager.h"
class base_stmt{
public:
	string codestr;
	int label;
	base_stmt(){
		codestr = "";
		label = -1;
	}
	virtual void gencode(){
		if(label != -1){
			cout << labelmanager::searchlabel(label) << ":" << endl;
		}
	}
	virtual void emitcode(){
		cout << codestr << endl;
	}
	void setcode(const string & _codestr){
		codestr = _codestr;
	}
	virtual void debug(){

	}
};


class stmt_list{
public:
	vector < shared_ptr <base_stmt> > vt;
};

class if_stmt:public base_stmt{
public:
	shared_ptr <base_expr>  judge;
	shared_ptr <stmt_list> lchild;
	shared_ptr <stmt_list> rchild;
	void gencode();
};

class assign_stmt:public base_stmt{
public:
	shared_ptr <base_expr> value;
	void debug(){

	}
};
class record_assign: public assign_stmt{
public:
	string id;
	string member;
	void gencode();
	void debug(){
		puts("record_assign");
		int i = 2;
		cout << id << " " << member << endl;
		value -> gencode();
	}
};
class normal_assign: public assign_stmt{
public:
	string id;
	void gencode();
	void debug(){
		puts("normal_assign:");
		int i = 2;
		cout << id << endl;
		value -> gencode();
	}
};
class arr_assign: public assign_stmt{
public:
	shared_ptr <base_expr> index;
	string id;
	void debug(){
		puts("arr_assign");
		cout << id << "\nindex:\n";
		int i = 2;
		index -> gencode();
		value -> gencode();
	}
	void gencode();
};

class while_stmt: public base_stmt{
public:
	shared_ptr <base_expr> judge;
	shared_ptr <stmt_list> stmt;
	void debug(){
		puts("while:");
		int i = 2;
		judge -> gencode();
	}
	void gencode();
};

class for_stmt: public base_stmt{
public:
	string id;
	shared_ptr <base_expr> start;
	shared_ptr <base_expr> end;
	shared_ptr <stmt_list> stmt;
	int dic;
	void gencode();
};
class repeat_stmt: public base_stmt{
public:
	shared_ptr <stmt_list> stmt_vt;
	shared_ptr <base_expr> judge;
	void debug(){
		cout << "repeat" << endl;
		int j = 2;
		for(int i = 0; i < stmt_vt -> vt.size(); ++i){
			stmt_vt -> vt[i] -> debug();
		}
		cout << "judge" << endl;
		judge -> gencode();
	}
	void gencode();
};

class case_expr{
public:
	shared_ptr <key_value_tuple> value;
	shared_ptr <base_stmt> stmt;
	void debug(){
		cout << value_set_to_str(value -> first -> gettype(),
			value -> second) << endl;
	}
};

class case_expr_list{
public:
	vector < shared_ptr<case_expr> >  case_vt;
	void debug(){ 
		for(int i = 0; i < case_vt.size();++i)
			case_vt[i] -> debug();
	}
};
class case_stmt: public base_stmt{
public:
	shared_ptr <base_expr> expr;
	shared_ptr <case_expr_list> case_list;
	void debug(){
		cout << "fuck case" << endl;
		cout <<" case_expr" << endl;
		case_list -> debug();
	}
	void gencode();
};

class goto_stmt:public base_stmt{
public:
	int addr;//address to go
	void gencode();
};

class proc_stmt: public base_stmt{
public:
	string proc_id;
	vector < shared_ptr <base_expr> > param;
	void debug();
	void gencode();
};

class func_node_value: public base_expr{
public:
	shared_ptr <proc_stmt> func_stmt;
	func_node_value(){
		type = FUNC_EXPR_TYPE;
	}
	int gencode(bool _double);
};

class sys_write_stmt:public base_stmt{
public:
	shared_ptr <base_expr> expr;
	void gencode();
};


