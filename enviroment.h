#pragma once
#include "routine.h"
#include "type_value.h"
class enviroment{
public:
	deque < routine_head * > q;
	static shared_ptr <enviroment> single();
	void insert(routine_head * tmp);
	void insert(shared_ptr <routine_head> tmp);
	routine_head * top();
	void pop();
	pair<int, int> search(const string & id);
	pair<int, int> search(const string & id, int index);
	pair<int, int> search(const string & id, const string & member);
	pair <shared_ptr <routine>, int> searchfunc(const string & id);
	key_value_tuple searchconst(const string & id);
	shared_ptr <base_type> searchtype(const string & id){
		int l = q.size();
		for(int i = l - 1; i >= 0; --i){
			auto res  = q[i] -> t_r -> search(id);
			if(res != nullptr)
				return res;		
		}
		return nullptr;
	}
	void clear();
};