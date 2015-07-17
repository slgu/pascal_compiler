#pragma once
#include "common.h"
#include "type_value.h"
#include "var_record.h"
#include "labelmanager.h"
#define UNDEFINE_EXPR "undefine_expr"
#define BASE_EXPR_TYPE "base_type"
#define UNARY_EXPR_TYPE "unary_type"
#define BINARY_EXPR_TYPE "binary_type"
#define CONST_EXPR_TYPE "leaf_node_value"
#define ID_EXPR_TYPE "id_node_value"
#define RECORD_EXPR_TYPE "record_node_value"
#define ARR_EXPR_TYPE "arr_node_value"
#define FUNC_EXPR_TYPE "func_node_value"
class base_expr{
public:
	string codestr;
	bool flag;
	string type;
	string gettype(){
		return type;
	}
	bool type_id;//表达式结果 整数或则浮点 true表示浮点数
	base_expr(){
		codestr = UNDEFINE_EXPR;
		flag = false;
		type = BASE_EXPR_TYPE;
	}
	virtual int gencode(bool _double = false){
	}//return the value is store in?
	void setcodestr(const string & _codestr){
		codestr = _codestr;
	}
	virtual ~base_expr(){
		
	}
	virtual bool isconst(){

	}
	virtual bool isstr(){
		return false;
	}
	virtual bool isconststr(string & s){
		return false;
	}
	virtual bool isdouble(){
		return false;
	}
	virtual bool checktype(bool _double	){

	}
	virtual bool isleaf(){
		return true;
	}	
};

class unary_expr: public base_expr{
public:
	int op;
	shared_ptr <base_expr> child;
	int gencode(bool _double);
	unary_expr(){
		type = UNARY_EXPR_TYPE;
	}
	bool isconst(){
		return child -> isconst();
	}
	bool isdouble(){
		return child -> isdouble();
	}
	bool checktype(bool _double){
		return child -> checktype(_double);
	}
	bool isleaf(){
		return false;
	}
};


class binary_expr: public base_expr{
public:
	int op;
	shared_ptr <base_expr>  lchild;
	shared_ptr <base_expr> rchild;
	int gencode(bool _double);
	binary_expr(){
		type = BINARY_EXPR_TYPE;
	}
	bool isconst(){
		return lchild -> isconst() && rchild -> isconst();
	}
	bool isdouble(){
		return lchild -> isdouble() && rchild -> isdouble();
	}
	bool checktype(bool _double){
		return lchild -> checktype(_double) && rchild -> checktype(_double);
	}
	bool isleaf(){
		return false;
	}
};
class leaf_node_value: public base_expr{
public:
	value_set value;
	int type_id;
	int gencode(bool _double);
	leaf_node_value(){
		type = CONST_EXPR_TYPE;
	}
	bool isconst(){
		return true;
	}
	bool isstr(){
		if(type_id == STR_TYPE){
			return true;
		}
		else
			return false;
	}
	bool isconststr(string & s){
		if(type_id == STR_TYPE){
			s = value._str;
			return true;
		}
		else
			return false;
	}
	bool isdouble(){
		return type_id == REAL_TYPE;
	}
	bool checktype(bool _double){
		if(_double)
			return type_id == REAL_TYPE || type_id == INT_TYPE;
		else
			return type_id == INT_TYPE;
	}
}; 
class record_node_value: public base_expr{
public:
	string id;
	string member;
	int gencode(bool _double);
	record_node_value(){
		type = RECORD_EXPR_TYPE;
	}
	bool isconst(){
		return false;
	}
	bool isstr();
	bool isdouble();
	bool checktype(bool _double);
};

class id_node_value: public base_expr{
public:
	string id;
	id_node_value(){
		type = ID_EXPR_TYPE;
	}
	int gencode(bool _double);
	bool isconst(){
		return false;
	}
	bool isstr();
	bool isdouble();
	bool checktype(bool _double);
};

class arr_node_value: public base_expr{
public:
	string id;
	shared_ptr <base_expr> index;
	arr_node_value(){
		type = ARR_EXPR_TYPE;
	}
	int gencode(bool _double);
	bool expr_value_type();
	bool isconst(){
		return false;
	}
	bool isstr();
	bool isdouble();
	bool checktype(bool _double);
};


