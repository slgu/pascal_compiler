#pragma once
#include "common.h"
#define RECORD_TYPE 142857
#define ARR_TYPE 428571
#define INT_TYPE 285714
#define STR_TYPE 857142
#define CHAR_TYPE 571428
#define REAL_TYPE 714285
#define DISCRETE_TYPE 31413
#define CONTINUE_TYPE 14133
#define POINT_TYPE 978232
class base_type{
protected:
	int type_id;
public:
	base_type(int i = 0){
		type_id = i;
	}
	void set_type(const int i){
		type_id = i;
	}
	int gettype(){
		return type_id;
	}
	virtual int getsize(){
		switch(type_id){
		case INT_TYPE:
			return 4;
		case REAL_TYPE:
			return 4;
		case CHAR_TYPE:
			return 1;
		case STR_TYPE:
			return 4;
		}
	}
	static int size(int  _type_id){
		switch(_type_id){
		case INT_TYPE:
			return 4;
		case REAL_TYPE:
			return 4;
		case CHAR_TYPE:
			return 1;
		case STR_TYPE:
			return 4;
		}
	}
};
class point_type: public base_type{
public:
	int offset;
	point_type(){
		type_id = POINT_TYPE;
	}
	int getsize(){
		return 4;
	}
	shared_ptr <base_type> nxt;
};
typedef shared_ptr <base_type> type_ptr;
union value_set{
	int _int;
	double _double;
	char _char;
	char * _str;
};//char * will free according the type of the value

typedef pair <type_ptr, value_set> key_value_tuple;
class record_type: public base_type{
public:
	vector <pair <string, type_ptr> > vt;
	record_type(){
		type_id = RECORD_TYPE;
	}
	int getsize(){
		int _size = 0;
		for(int i = 0; i < vt.size(); ++i){
			_size += vt[i].second -> getsize();
		}
		return _size;
	}
	pair <int, int> search(const string & id){
		int l = vt.size();
		int off = 0;
		for(int i = l - 1; i >= 0; --i){
			if(vt[i].first == id){
				int type_id = vt[i].second -> gettype();
				return make_pair(off, type_id);
			}
			else{
				off += vt[i].second -> getsize();
			}
		}
		return make_pair(off, -1);
	}
};
class arr_type: public base_type{
public:
	arr_type(){
		type_id = ARR_TYPE;
	}
	type_ptr index;
	type_ptr nxt;
	int getsize(){
		return nxt -> getsize() * index -> getsize();	
	}
private:
};
class continue_type:public base_type{
public:
	continue_type(){
		type_id = CONTINUE_TYPE;
	}
	key_value_tuple left;
	key_value_tuple right;
	int getsize(){
		if(left.first -> gettype() == INT_TYPE){
			return right.second._int - left.second._int + 1;
		}
		else
		return right.second._double - left.second._double + 1;
	}
private:
};

class discrete_type:public base_type{
public:
	discrete_type(){
		type_id = DISCRETE_TYPE;
	}
	vector <string> index;
	int getsize(){
		return index.size();
	}
private:

};


inline string value_set_to_str(int type_id, value_set value){
	switch(type_id){
		case INT_TYPE:
			return string(itoa(value._int));
		case STR_TYPE:
			return string(value._str);
		case REAL_TYPE:
			return ftoa(value._double);
		case CHAR_TYPE:{
			string res = "";
			res += value._char;
			return res;
		}
	}
}
