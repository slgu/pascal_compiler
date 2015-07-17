#pragma once
#include "const_record.h"
#include "type_record.h"
#include "var_record.h"
#include "stmt.h"
#include "type_value.h"
#include "labelmanager.h"
#define FUNC_ROUTINE "func_routine"
#define PROC_ROUTINE "proc_routine"


class routine_head;

class routine{
public:
	string name;
	string type;
	shared_ptr <routine_head> header;
	shared_ptr <stmt_list> stmt_vt;
	virtual void gencode();
	string gettype(){
		return type;
	}
	virtual void add_function_param();
	int getsize();
};

class routine_record{
public:
	vector < shared_ptr <routine> > vt;
};

class routine_head{
public:
	shared_ptr <const_record> c_r;
	shared_ptr <var_record> v_r;
	shared_ptr <type_record> t_r;
	shared_ptr <routine_record> r_r;
	routine_head(){
		c_r = nullptr;
		v_r = nullptr;
		t_r = nullptr;
		r_r = nullptr;	
	}
};
class procedure: public routine{
public:
	vector < pair<int, pair <string, type_ptr> > > param;
	void gencode();
	procedure(){
		type = PROC_ROUTINE;
	}
	void add_function_param();
};

class func: public routine{
public:
	vector < pair<int, pair <string, type_ptr> > > param;
	shared_ptr <base_type> ret;
	void gencode();
	func(){
		type = FUNC_ROUTINE;
	}
	void add_function_param();
};
