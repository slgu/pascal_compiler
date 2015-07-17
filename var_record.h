#pragma once
#include "type_value.h"
class var_record{
private:
	map <string, shared_ptr <base_type> > mp;
	deque < pair <string, shared_ptr <base_type> > > vt;
	string concat(const vector <string> & vt){
		string res = "";
		for(const string & s: vt){
			res += s + ":";
		}
		return res;
	}
public:
	bool insert(const string type_id, shared_ptr <base_type> type);
	bool insert_front(const string type_id, shared_ptr <base_type> type);
	pair <int,int> search(const string & id);
	pair <int, int> search(const string & id, int index);
	pair <int, int> search(const string & id, const string & member);
	void gencode();
	int getsize(){
		int _s = 0;
		for(auto i = vt.begin(); i != vt.end(); ++i){
			_s += i -> second -> getsize();
		}
		return _s;
	}
	int getstacksize(){
		int _s = 0;
		int l = vt.size();
		int off = 0;
		for(int i = l - 1; i >= 0; --i){
			if(vt[i].first[0] == '~')
				break;
			off += vt[i].second -> getsize();
		}
		return off;
	}
};