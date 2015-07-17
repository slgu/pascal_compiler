#include "routine.h"
#include "enviroment.h"
#include "common.h"
#include "reg.h"
#include "string_data.h"
void func::gencode(){
	cout << name << ":" << endl;
	routine::gencode();
}
void procedure::gencode(){
	cout << name << ":" << endl;
	routine::gencode();
}
void routine::gencode(){
	if(type != FUNC_ROUTINE && type != PROC_ROUTINE){
		cout << "section .data\n";
		string_data::single() -> gencode();
		cout << "section .text\n";
		cout << "global main\n";
		cout << "main:\n";
		cout << "xor esi, esi" << endl;
		cout << "extern printr" << endl;
		cout << "extern prints" << endl;
		cout << "extern print" << endl;
	}
	cout << "sub esp, " << header -> v_r -> getstacksize() << endl;//to get the stack variable
	
	enviroment::single() -> insert(this -> header.get());
	for(int i = 0; i < stmt_vt -> vt.size(); ++i){
		stmt_vt -> vt[i] -> gencode();
	}
	cout << "add esp, " << header -> v_r -> getstacksize() << endl;//to release the stack variable
	if(type == FUNC_ROUTINE || type == PROC_ROUTINE){
		cout << "ret" << endl;
	}
	else{
		cout << "mov eax, 1\n";
		cout << "mov ebx, 0\n";
		cout << "int 80h\n";
	}
	for(int i = 0; i < header -> r_r -> vt.size(); ++i){
		header -> r_r -> vt[i] -> gencode();
	}
	enviroment::single() -> pop();
}
void routine::add_function_param(){
	for(int i = 0; i < header -> r_r -> vt.size(); ++i){
		header -> r_r -> vt[i] -> add_function_param();
	}
}
void procedure::add_function_param(){
	int l = param.size();
	shared_ptr <base_type> tmp(new base_type(INT_TYPE));
	header -> v_r -> insert_front("~nop5", tmp);
	for(int i = l - 1; i >= 0; --i){
		header -> v_r -> insert_front(param[i].second.first, param[i].second.second);
	}
	header -> v_r -> insert_front("~nop1", tmp);
	header -> v_r -> insert_front("~nop2", tmp);
	header -> v_r -> insert_front("~nop3", tmp);
	header -> v_r -> insert_front("~nop4", tmp);
	routine::add_function_param();
}
void func::add_function_param(){
	int l = param.size();
	shared_ptr <base_type> tmp(new base_type(INT_TYPE));
	header -> v_r -> insert_front("~nop5", tmp);
	for(int i = l - 1; i >= 0; --i){
		header -> v_r -> insert_front(param[i].second.first, param[i].second.second);
	}
	header -> v_r -> insert_front(name, ret);
	header -> v_r -> insert_front("~nop1", tmp);
	header -> v_r -> insert_front("~nop2", tmp);
	header -> v_r -> insert_front("~nop3", tmp);
	header -> v_r -> insert_front("~nop4", tmp);
	routine::add_function_param();
}
int routine::getsize(){
	return header -> v_r -> getsize();
}