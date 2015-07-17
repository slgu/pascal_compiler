#include "stmt.h"
#include "enviroment.h"
#include "reg.h"
void record_assign::gencode(){
	base_stmt::gencode();
	auto _reg = reg::single();
	_reg -> clear();
	string str;
	if(value -> isconststr(str)){
		auto in = enviroment::single() -> top() -> v_r -> search(id, member);
		if(in.second == -1){	
			auto in2 = enviroment::single() -> search(id, member);
			if(in2.second == -1){
				cout << id << " " << member << " not found" << endl;
				return;
			}
			else{
				codestr = "mov ebp, esp";
				codestr += "\nmov eax, " + str;
				codestr += "\nmov [ebp + esi + " + itoa(in2.first) + "], eax";
				cout  << codestr << endl;
				return;
			}
		}
		else{
			codestr = "mov ebp, esp";
			codestr += "\nmov eax, " + str;
			codestr += "\nmov [ebp + " + itoa(in.first) + "], eax";
			cout  << codestr << endl;
			return;
		}
	}
	bool flag = enviroment::single() -> search(id, member).second == REAL_TYPE;
	int k = value -> gencode(flag);
	auto in = enviroment::single() -> top() -> v_r -> search(id, member);
	if(in.second == POINT_TYPE){
		cout << "mov ebp, esp" << endl;
	 	cout << "mov ebp, [ebp + " << in.first << "]" << endl;	
		cout << "mov [ebp], " << _reg -> finde(k) << endl;
		_reg -> setfree(k);
		return;
	} 
	if(in.second == -1){	
		auto in2 = enviroment::single() -> search(id, member);
		if(in2.second == -1){
			cout << id << " " << member << " not found" << endl;
			return;
		}
		else{
			codestr = "mov ebp, esp";
			if(flag)
				codestr += "\nfstp dword [ebp + esi + " + itoa(in2.first) + "]";
			else{
				codestr += "\nmov [ebp + esi + " + itoa(in2.first) + "], " + _reg -> finde(k);
				_reg -> setfree(k);
			}
			cout  << codestr << endl;
		}
	}
	else{
		codestr = "mov ebp, esp";
		if(flag)
			codestr += "\nfstp dword [ebp + " + itoa(in.first) + "]";
		else{
			codestr += "\nmov [ebp + " + itoa(in.first) + "], " + _reg -> finde(k);
			_reg -> setfree(k);
		}
		cout  << codestr << endl;
	}
}
void normal_assign::gencode(){
	base_stmt::gencode();
	auto _reg = reg::single();
	_reg -> clear();
	string str;
	if(value -> isconststr(str)){
		auto in = enviroment::single() -> top() -> v_r -> search(id);
		if(in.second == -1){	
			auto in2 = enviroment::single() -> search(id);
			if(in2.second == -1){
				cout << id << " not found" << endl;
				return;
			}
			else{
				codestr = "mov ebp, esp";
				codestr += "\nmov eax, " + str;
				codestr += "\nmov [ebp + esi + " + itoa(in2.first) + "], eax";
				cout  << codestr << endl;
				return;
			}
		}
		else{
			codestr = "mov ebp, esp";
			codestr += "\nmov eax, " + str;
			codestr += "\nmov [ebp + " + itoa(in.first) + "], eax";
			cout  << codestr << endl;
			return;
		}
	}
	bool flag = enviroment::single() -> search(id).second == REAL_TYPE;
	int k  = value -> gencode(flag);
	auto in = enviroment::single() -> top() -> v_r -> search(id);
	if(in.second == POINT_TYPE){
		cout << "mov ebp, esp" << endl;
	 	cout << "mov ebp, [ebp + " << in.first << "]" << endl;	
		cout << "mov [ebp], " << _reg -> finde(k) << endl;
		_reg -> setfree(k);
		return;
	} 
	if(in.second == -1){	
		auto in2 = enviroment::single() -> search(id);
		if(in2.second == -1){
			cout << id << " not found" << endl;
			return;
		}
		else{
			codestr = "mov ebp, esp";
			if(flag)
				codestr += "\nfstp dword [ebp + esi + " + itoa(in2.first) + "]";
			else{
				codestr += "\nmov [ebp + esi + " + itoa(in2.first) + "], " + _reg -> finde(k);
			_reg -> setfree(k);
			}
			cout  << codestr << endl;
		}	
		
	}
	else{
		codestr = "mov ebp, esp";
		if(flag)
			codestr += "\nfstp dword [ebp + " + itoa(in.first) + "]";
		else{
			codestr += "\nmov [ebp + " + itoa(in.first) + "], " + _reg -> finde(k);
			_reg -> setfree(k);
		}	
		cout << codestr << endl;
	}
}
void proc_stmt::debug(){
	
}
void proc_stmt::gencode(){
	base_stmt::gencode();
	auto res = enviroment::single() -> searchfunc(proc_id);
	auto rout = res.first;
	auto off = res.second;
	if(rout == nullptr){
		cout << "no function:" << proc_id << "is founded" << endl;
		return;
	}
	if(rout -> gettype() == PROC_ROUTINE){
		int call_length = param.size() * 4;
		auto fun = (procedure *)rout.get();
		codestr = "mov ebp, esp";
		if(fun -> param.size() != param.size()){
			cout << "proc param size error" << endl;
			return;
		}
		for(int i = 0; i < param.size(); ++i){
			if(fun -> param[i].first == 1){
				bool flag = false;
				bool point_flag = false;
				pair <int, int> offset;
				if(param[i] -> gettype() == ARR_EXPR_TYPE){
					auto res = (arr_node_value *)param[i].get();
				 	offset = enviroment::single() -> search(res -> id, 0);
				 	if(enviroment::single() -> top() -> v_r -> search(res -> id, 0).second != -1)
				 		flag = true;	
				}
				else
				if(param[i] -> gettype() == ID_EXPR_TYPE){
					auto res = (id_node_value *)param[i].get();
					offset = enviroment::single() -> search(res -> id);
					if(offset.second == POINT_TYPE){
						point_flag = true;
					}	
					if(enviroment::single() -> top() -> v_r -> search(res -> id).second != -1)
				 		flag = true;
					
				}
				else
				if(param[i] -> gettype() == RECORD_EXPR_TYPE){
					auto res = (record_node_value *)param[i].get();
					offset = enviroment::single() -> search(res -> id, res -> member);
					if(enviroment::single() -> top() -> v_r -> search(res -> id, res -> member).second != -1)
				 		flag = true;
				}
				if(point_flag){
					codestr += "\nmov eax, [ebp + " + itoa(offset.first) + "]";
					codestr += "\nmov [ebp - " + itoa((i + 5) * 4) + "], eax";
				}
				else{
					codestr += "\nmov eax, ebp";
					codestr += "\nadd eax ," + itoa(offset.first);
					if(!flag){
						codestr += "\nadd eax, esi";
						codestr += "\nadd eax, " + itoa(off);
					}
					codestr += "\nmov [ebp - " + itoa((i + 5) * 4) + "], eax";
				}
			
			}
			else{
				cout << codestr << endl;
				int ret = param[i] -> gencode();
				codestr = "";
				codestr += "\nmov [ebp - " + itoa((i + 5) * 4)  + "], " + reg::single() -> finde(ret);
				reg::single() -> setfree(ret);
			}	
		}
		cout << codestr << endl;
		reg::single() -> push();
		cout << "add esi, " << off << endl;
		cout << "sub esp, " + itoa(call_length) << endl;
		cout << "call " + fun -> name << endl;
		cout << "add esp, " + itoa(call_length) << endl;
	}
	else{
		int call_length = (param.size() + 1) * 4;
		codestr = "mov ebp, esp";
		auto fun = (func * )rout.get();
		if(fun -> param.size() != param.size()){
			cout << "func param size error" << endl;
			//cout << fun -> name << endl;
			cout << fun -> param.size() << " " << param.size() << endl;
			return;
		}
		for(int i = 0; i < param.size(); ++i){
			pair<int, int> offset;
			bool flag = false;
			bool point_flag = false;
			if(fun -> param[i].first == 1){
				if(param[i] -> gettype() == ARR_EXPR_TYPE){
					auto res = (arr_node_value *)param[i].get();
					offset = enviroment::single() -> search(res -> id, 0);
					if(enviroment::single() -> top() -> v_r -> search(res -> id, 0).second != -1)
				 		flag = true;
					
				}
				else
				if(param[i] -> gettype() == ID_EXPR_TYPE){
					auto res = (id_node_value *)param[i].get();
					offset = enviroment::single() -> search(res -> id);
					if(enviroment::single() -> top() -> v_r -> search(res -> id).second != -1)
				 		flag = true;
					if(offset.second == POINT_TYPE){
						point_flag = true;
					}	
				}
				else
				if(param[i] -> gettype() == RECORD_EXPR_TYPE){
					auto res = (record_node_value *)param[i].get();
					offset = enviroment::single() -> search(res -> id, res -> member);
					if(enviroment::single() -> top() -> v_r -> search(res -> id, res -> member).second != -1)
				 		flag = true;
					
				}
				if(point_flag){
					codestr += "\nmov eax, [ebp + " + itoa(offset.first) + "]";
					codestr += "\nmov [ebp - " + itoa((i + 6) * 4) + "], eax";
				}
				else{
					codestr += "\nmov eax, ebp";
					codestr += "\nadd eax ," + itoa(offset.first);
					if(!flag){
						codestr += "\nadd eax, esi";
						codestr += "\nadd eax, " + itoa(off);
					}
					codestr += "\nmov [ebp - " + itoa((i + 6) * 4) + "], eax";
				}
			}
			else{
				cout << codestr << endl;
				int ret = param[i] -> gencode();
				codestr = "";
				codestr += "\nmov [ebp - " + itoa((i + 6) * 4) + "], " + reg::single() -> finde(ret);
				reg::single() -> setfree(ret);
			}	
		}
		cout << codestr << endl;
		reg::single() -> push();
		cout << "add esi, " << off << endl;
		cout << "sub esp, " + itoa(call_length) << endl;
		cout << "call " + fun -> name << endl;
		cout << "add esp, " + itoa(call_length - 4) << endl;
		cout << "mov ebp, esp\nmov eax, [ebp]" << endl;
		cout << "add esp, 4" << endl;
	}
	reg::single() -> pop();
}
void arr_assign::gencode(){
	base_stmt::gencode();
	auto _reg = reg::single();
	_reg -> clear();
	int ind  = index -> gencode();
	_reg -> setflag(ind);
	string str;
	if(value -> isconststr(str)){
		auto in = enviroment::single() -> top() -> v_r -> search(id,3);
		if(in.second == -1){	
			auto in2 = enviroment::single() -> search(id,3);
			if(in2.second == -1){
				cout << id << " not found" << endl;
				return;
			}
			else{
				codestr = "";
				cout << "mov ebp, esp" << endl;
				cout << "push esi" << endl;
				cout << "mov edx, " << _reg -> finde(ind) << endl;
				cout << "imul edx, " << base_type::size(in2.second) << endl;
				cout << "add esi, edx" << endl;
				codestr += "\nmov eax, " + str;
				codestr += "\nmov [ebp + esi + " + itoa(in2.first) + "], eax";
				cout  << codestr << endl;
				cout << "pop esi";
				return;
			}
		}
		else{
			codestr = "";
			cout << "mov ebp, esp" << endl;
			cout << "push esi" << endl;
			cout << "xor esi ,esi" << endl;
			cout << "mov edx, " << _reg -> finde(ind) << endl;
			cout << "imul edx, " << base_type::size(in.second) << endl;
			cout << "add esi, edx" << endl;	
			codestr += "\nmov eax, " + str;
			codestr += "\nmov [ebp + esi" + itoa(in.first) + "], eax";
			cout  << codestr << endl;
			return;
		}
	}
	bool flag = enviroment::single() -> search(id, 3).second == REAL_TYPE;
	int k  = value -> gencode(flag);
	auto in = enviroment::single() -> top() -> v_r -> search(id,3);
	if(in.second == -1){	
		auto in2 = enviroment::single() -> search(id,3);
		if(in2.second == -1){
			cout << id << " not found" << endl;
			return;
		}
		else{
			codestr = "";
			cout << "mov ebp, esp" << endl;
			cout << "push esi" << endl;
			cout << "mov edx, " << _reg -> finde(ind) << endl;
			cout << "imul edx, " << base_type::size(in2.second) << endl;
			cout << "add esi, edx" << endl;
			if(flag)
				codestr += "\nfstp dword [ebp + esi + " + itoa(in2.first) + "]";
			else{
				codestr += "\nmov [ebp + esi + " + itoa(in2.first) + "], " + _reg -> finde(k);
			_reg -> setfree(k);
			}
			cout  << codestr << endl;
			cout << "pop esi" << endl;
		}	
		
	}
	else{
		codestr = "";
		cout << "mov ebp, esp" << endl;
		cout << "push esi" << endl;
		cout << "xor esi, esi" << endl;
		cout << "mov edx, " << _reg -> finde(ind) << endl;
		cout << "imul edx, " << base_type::size(in.second) << endl;
		cout << "add esi, edx" << endl;
		if(flag)
			codestr += "\nfstp dword [ebp + esi + " + itoa(in.first) + "]";
		else{
			codestr += "\nmov [ebp + esi + " + itoa(in.first) + "], " + _reg -> finde(k);
			_reg -> setfree(k);
		}	
		cout << codestr << endl;
		cout << "pop esi" << endl;
	}
}
void if_stmt::gencode(){
	base_stmt::gencode();
	reg::single() -> clear();
	int k = judge -> gencode();
	string a1 = labelmanager::genlabel();
	string a2 = labelmanager::genlabel();
	cout << "cmp " + reg::single() -> finde(k) + ", 0" << endl;
	reg::single() -> setfree(k);	
	cout << "jz " + a1 << endl;
	for(int i = 0; i < lchild -> vt.size(); ++i)
	lchild -> vt[i] -> gencode();
	cout << "jmp " + a2 << endl;
	cout << a1 + ":" << endl;
	if(rchild == nullptr){
	}
	else{
		for(int i = 0; i < rchild -> vt.size(); ++i)
		rchild -> vt[i] -> gencode();
	}
	cout << a2 << ":" << endl;
}
void for_stmt::gencode(){
	base_stmt::gencode();
	auto off = enviroment::single() -> search(id);
	auto _reg = reg::single();
	_reg -> clear();
	int k = start -> gencode();
	cout << "mov ebp, esp" << endl;
	cout << "mov [ebp + " << off.first << "], " + _reg -> finde(k) << endl;
	string loop = labelmanager::genlabel();
	string break_place = labelmanager::genlabel();
	_reg -> setfree(k);
	cout << loop + ":" << endl;
	for(int i = 0; i < stmt -> vt.size(); ++i){
		stmt -> vt[i] -> gencode();
	}
	int w = end -> gencode();
	cout << "mov eax, [ebp + " << off.first  << "]" << endl;
	cout << "cmp eax, " + _reg -> finde(w) << endl;
	cout << "jz " + break_place << endl;
	if(dic){
		cout << "mov eax, [ebp + " << off.first << "]" << endl; 
		cout << "sub eax, 1" << endl;
		cout << "mov [ebp + " << off.first << "], eax" << endl; 
	}
	else{
		cout << "mov eax, [ebp + " << off.first << "]" << endl; 
		cout << "add eax, 1" << endl;
		cout << "mov [ebp + " << off.first << "], eax" << endl;
	}
	cout << "jmp " + loop << endl;
	cout << break_place + ":" << endl;
}
void while_stmt::gencode(){
	base_stmt::gencode();
	auto _reg = reg::single();
	_reg -> clear();
	string loop =  labelmanager::genlabel();
	string break_place = labelmanager::genlabel();
	cout << loop + ":" << endl; 
	int ju = judge -> gencode();
	cout << "cmp " + _reg -> finde(ju) + ", 0" << endl;  
	_reg -> setfree(ju);
	cout << "jz " + break_place << endl;
	for(int i = 0; i < stmt -> vt.size(); ++i){
		stmt -> vt[i] -> gencode();
	}
	cout << "jmp " + loop << endl;
	cout << break_place + ":;" << endl;
}

void case_stmt::gencode(){
	base_stmt::gencode();
	auto _reg = reg::single();
	_reg -> clear();
	vector <string> label_vt;
	int lcmp = expr -> gencode();
	for(int i = 0; i < case_list -> case_vt.size(); ++i){
		label_vt.push_back(labelmanager::genlabel());
	}
	string over_label = labelmanager::genlabel();
	for(int i = 0; i < case_list -> case_vt.size(); ++i){
		cout << "cmp " + _reg -> finde(lcmp) + ", " + value_set_to_str(case_list -> case_vt[i] -> value -> first -> gettype(),
			case_list -> case_vt[i] -> value -> second) << endl;
		cout << "jz " + label_vt[i] << endl;
	}
	cout << codestr << endl;
	cout << "jmp " << over_label << endl;
	for(int i = 0; i < case_list -> case_vt.size(); ++i){
		cout << label_vt[i] << ":\n";
		case_list -> case_vt[i] -> stmt -> gencode();
		cout << "jmp " << over_label << endl; 
	}
	cout << over_label << ":;" << endl;
}

void repeat_stmt::gencode(){
	base_stmt::gencode();
	auto _reg = reg::single();
	_reg -> clear();
	string loop =  labelmanager::genlabel();
	cout << loop + ":";
	for(int i = 0; i < stmt_vt -> vt.size(); ++i){
		stmt_vt -> vt[i] -> gencode();
	}
	int ju = judge -> gencode();
	cout << "cmp " + _reg -> finde(ju) + ", 0" << endl; 
	_reg -> setfree(ju);
	cout << "jz " + loop << endl;
}

void goto_stmt::gencode(){
	base_stmt::gencode();
	cout << "jmp " << labelmanager::searchlabel(addr) << endl;
}
void sys_write_stmt::gencode(){
	int k = expr -> gencode();
	cout << "push dword " + reg::single() -> finde(k) << endl;
	reg::single() -> setfree(k);
	if(expr -> isstr()){
		cout << "call prints" << endl;
	}
	else
		if(expr -> isdouble()){
		cout << "call printr" << endl;
		}
		else{
		cout << "call print" << endl;
	}
	cout << "pop " + reg::single() -> finde(k) << endl;
}
int func_node_value::gencode(bool _double){
	func_stmt -> gencode();
	int k = reg::single() -> findfree();
	if(_double){
		cout << "push eax" << endl;
		cout << "fld dword [esp]" << endl; 
		cout << "pop eax " << endl;
	}
	else{
		reg::single() -> setflag(k);
		cout << "mov " +reg::single() -> finde(k) + ", eax" << endl;
	}
	return k;
}