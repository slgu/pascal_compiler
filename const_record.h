#pragma once
#include "common.h"
#include "type_value.h"
class const_record{
private:
	map <string , key_value_tuple >mp;
public:
	bool insert(const string & id, 
		type_ptr type, 
		value_set value){
		if(mp.find(id) != mp.end())
			return false;
		mp[id] = make_pair(type,value);
		return true;
	}

	void debug(){
		for(auto i = mp.begin(); i != mp.end(); ++i){
			cout << i -> first << " " << i -> second.first -> gettype() 
			<< " " << value_set_to_str(i -> second.first -> gettype(), i -> second.second) << endl;
		}
	}
	key_value_tuple const_replace(const string & id){
		if(mp.find(id) == mp.end()){
			value_set tmp;
			return make_pair(nullptr, tmp);
		}	
		else
			return mp[id];
	}
};

