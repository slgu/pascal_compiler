#include "expr.h"
#include "enviroment.h"
#include "reg.h"
#include "common.h"
int unary_expr::gencode(bool _double){
	int k = child -> gencode(_double);
	switch(op){
	case MINUS_TYPE:
		codestr = "xor " + reg::single() -> finde(k) + ", -1";
		codestr += "\nadd " + reg::single() -> finde(k) + ", 1";
		break;
	case NOT_TYPE:
		codestr = "xor " + reg::single() -> finde(k) + ", -1";
		break;
	}
	cout << codestr << endl;
	return k;
}

int binary_expr::gencode(bool _double){
	auto _reg = reg::single();
	int l = lchild -> gencode(_double);
	int r = rchild -> gencode(_double);
	switch(op){
	case MUL_TYPE:
		if(_double)
			codestr = "fmul";
		else
			codestr = "imul " + _reg -> finde(l) +", " + _reg -> finde(r);
		break;
	case PLUS_TYPE:
		if(_double)
			codestr = "fadd";
		else
			codestr = "add " + _reg -> finde(l) +", " + _reg -> finde(r);
		break;
	case MINUS_TYPE:
		if(_double)
			codestr = "fsub";
		else
			codestr = "sub " + _reg -> finde(l) +", " + _reg -> finde(r);
		break;
	case DIV_TYPE:
		if(_double)
			codestr = "fdiv";
		else{
		codestr  = "mov eax, " + _reg -> finde(l);
		codestr += "\nxor edx, edx";
		codestr += "\nidiv " + _reg -> finde(r);
		codestr += "\nmov " + _reg -> finde(l) + ", eax";
		}
		break;
	case GE_TYPE:
		codestr = "cmp " + _reg -> finde(l) + ", " + _reg -> finde(r);
		codestr += "\npushf";
		codestr += "\npop eax";
		codestr += "\nshr eax, 7";
		codestr += "\nand eax, 1";
		codestr += "\nxor eax, 1";
		codestr += "\nmov " + _reg -> finde(l) + ", eax";
		break;
	case GT_TYPE:
		codestr = "cmp " + _reg -> finde(r) + ", " + _reg -> finde(l);
		codestr += "\npushf";
		codestr += "\npop eax";
		codestr += "\nshr eax, 7";
		codestr += "\nand eax, 1";
		codestr += "\nmov " + _reg -> finde(l) + ", eax";
		break;
	case LE_TYPE:
		codestr = "cmp " + _reg -> finde(r) + ", " + _reg -> finde(l);
		codestr += "\npushf";
		codestr += "\npop eax";
		codestr += "\nshr eax, 7";
		codestr += "\nand eax, 1";
		codestr += "\nxor eax, 1";
		codestr += "\nmov " + _reg -> finde(l) + ", eax";
		break;
	case LT_TYPE:
		codestr = "cmp " + _reg -> finde(l) + ", " + _reg -> finde(r);
		codestr += "\npushf";
		codestr += "\npop eax";
		codestr += "\nshr eax, 7";
		codestr += "\nand eax, 1";
		codestr += "\nmov " + _reg -> finde(l) + ", eax";
		break;
	case EQUAL_TYPE:
		codestr = "cmp " + _reg -> finde(l) + ", " + _reg -> finde(r);
		codestr += "\npushf";
		codestr += "\npop eax";
		codestr += "\nshr eax, 6";
		codestr += "\nand eax, 1";
		codestr += "\nmov " + _reg -> finde(l) + ", eax";
		break;
	case MOD_TYPE:
		codestr  = "mov eax, " + _reg -> finde(l);
		codestr += "\nxor edx, edx";
		codestr += "\nidiv " + _reg -> finde(r);
		codestr += "\nmov " + _reg -> finde(l) + ", edx";
		break;
	case UNEQUAL_TYPE:
		codestr = "cmp " + _reg -> finde(l) + ", " + _reg -> finde(r);
		codestr += "\npushf";
		codestr += "\npop eax";
		codestr += "\nshr eax, 6";
		codestr += "\nand eax, 1";
		codestr += "\nxor eax, 1";
		codestr += "\nmov " + _reg -> finde(l) + ", eax";
		break;
	case AND_TYPE:
		codestr = "and " + _reg -> finde(l) +", " + _reg -> finde(r);
		break;
	}
	_reg -> setfree(r);
	cout << codestr << endl;
	return l;
}

int leaf_node_value::gencode(bool _double){
	auto _reg = reg::single();
	string str = value_set_to_str(type_id, value);
	int k;
	if(_double){
		cout << "mov eax, __float32__(";
		if(type_id == INT_TYPE)
			cout << str << ".0";
		else
			cout << str;
		cout << ")" << endl;
		cout << "sub esp, 4" << endl;
		cout << "mov ebp, esp" << endl;
		cout << "mov [ebp], eax" << endl;
		cout << "fld dword [ebp]" << endl;
		cout << "add esp, 4" << endl;
	}
	else{
		k = _reg -> findfree();
		codestr = "xor " + _reg -> finde(k) + "," + _reg -> finde(k);
		codestr += "\nmov " + _reg -> finde(k) + ", " + str;
		_reg -> setflag(k);
		cout << codestr << endl;
	}
	return k;
}

int record_node_value::gencode(bool _double){
	auto _reg = reg::single();
	auto off = enviroment::single() -> top() -> v_r -> search(id, member);
	int k;
	if(off.second == -1){
		auto off2 = enviroment::single() -> search(id, member);
		if(off2.second == -1){
			cout << "id:" + id + " not found" << endl;
		}
		else{
			cout << "mov ebp, esp" << endl;
			if(_double){
				cout << "fld dword [ebp + esi +" + itoa(off2.first) + "]" << endl;
			}
			else{
			k = _reg -> findfree();
			cout << "mov " + _reg -> finde(k) + ", [ebp + esi + " + itoa(off2.first) + "]" << endl;
			_reg -> setflag(k);
			}
		}
	}
	else{
		cout << "mov ebp, esp" << endl;
		if(_double){
				cout << "fld dword [ebp + " + itoa(off.first) + "]" << endl;
		}
		else{
			k = _reg -> findfree();
			cout << "mov " + _reg -> finde(k) + ", [ebp + " + itoa(off.first) + "]" << endl;
			_reg -> setflag(k);
		}
	}
	return k;
}
bool record_node_value::isstr(){
	auto off = enviroment::single() -> search(id, member);
	if(off.second == STR_TYPE)
		return true;
	else
	return false;
}
bool id_node_value::isstr(){
	auto off = enviroment::single() -> search(id);
	if(off.second == STR_TYPE)
		return true;
	else
	return false;
}
bool arr_node_value::isstr(){
	return false;
}
int id_node_value::gencode(bool _double){
	auto _reg = reg::single();
	auto off = enviroment::single() -> top() -> v_r -> search(id);
	int k;
	if(off.second == POINT_TYPE){
		cout << "mov ebp, esp" << endl;
	 	cout << "mov ebp, [ebp + " << off.first << "]" << endl;	
		k = _reg -> findfree();
		cout << "mov " + _reg -> finde(k) + ", [ebp]" << endl;
		cout << "mov ebp, esp" << endl;
	 	_reg -> setflag(k);
		return k;
	} 
	if(off.second == -1){
		auto off2 = enviroment::single() -> search(id);
		if(off2.second == -1){
			cout << "id:" + id + " not found" << endl;
		}
		else{	
			cout << "mov ebp, esp" << endl;
			if(_double){
				cout << "fld dword [ebp + esi" + itoa(off2.first) + "]" << endl;
			}
			else{
				k = _reg -> findfree();
				cout << "mov " + _reg -> finde(k) + ", [ebp + esi + " + itoa(off2.first) + "]" << endl;
				_reg -> setflag(k);
			}
		}
	}
	else{
		cout << "mov ebp, esp" << endl;
		if(_double){
				cout << "fld dword [ebp + " + itoa(off.first) + "]" << endl;
		}
		else{
			k = _reg -> findfree();
			cout << "mov " + _reg -> finde(k) + ", [ebp + " + itoa(off.first) + "]" << endl;
			_reg -> setflag(k);
		}
	}
	return k;
}

int arr_node_value::gencode(bool _double){
	auto _reg = reg::single();
	auto off = enviroment::single() -> top() -> v_r -> search(id,3);
	int ind = index -> gencode();
	_reg -> setflag(ind);
	if(off.second == -1){
		auto off2 = enviroment::single() -> search(id, 3);
		if(off2.second == -1){
			cout << "id:" + id + " not found" << endl;
		}
		else{	
			cout << "mov ebp, esp" << endl;
			cout << "push esi" << endl;
			cout << "mov eax, " << _reg -> finde(ind) << endl;
			cout << "imul eax, " << base_type::size(off2.second) << endl;
			cout << "add esi, eax" << endl;
			if(_double){
				cout << "fld dword [ebp + esi" + itoa(off2.first) + "]" << endl;
			}
			else{
				cout << "mov " + _reg -> finde(ind) + ", [ebp + esi + " + itoa(off2.first) + "]" << endl;
			}
			cout << "pop esi" << endl;
		}
	}
	else{
		cout << "mov ebp, esp" << endl;
		cout << "push esi" << endl;
		cout << "xor esi, esi" << endl;
		cout << "mov eax, " << _reg -> finde(ind) << endl;
		cout << "imul eax, " << base_type::size(off.second) << endl;
		cout << "add esi, eax" << endl;
		if(_double){
			cout << "fld dword [ebp + esi + " + itoa(off.first) + "]" << endl;
		}
		else{
			cout << "mov " + _reg -> finde(ind) + ", [ebp + esi + " + itoa(off.first) + "]" << endl;
		}
		cout << "pop esi" << endl;
	}
	return ind;
}
bool arr_node_value::isdouble(){
	auto off = enviroment::single() -> search(id, 3);
	return off.second == REAL_TYPE;
}
bool id_node_value::isdouble(){
	auto off = enviroment::single() -> search(id);
	return off.second == REAL_TYPE;
}

bool record_node_value::isdouble(){
	auto off = enviroment::single() -> search(id, member);
	return off.second == REAL_TYPE;
}
bool record_node_value::checktype(bool _double){
	auto off = enviroment::single() -> search(id, member);
	if(_double){
		return off.second == REAL_TYPE;
	}
	else
		return off.second == INT_TYPE;
}
bool id_node_value::checktype(bool _double){
	auto off = enviroment::single() -> search(id);
	if(_double){
		return off.second == REAL_TYPE;
	}
	else
		return off.second == INT_TYPE;
}
bool arr_node_value::checktype(bool _double){
	auto off = enviroment::single() -> search(id, 3);
	if(_double){
		return off.second == REAL_TYPE;
	}
	else
		return off.second == INT_TYPE;
}

