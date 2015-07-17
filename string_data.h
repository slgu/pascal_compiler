#pragma once
#include "common.h"
#include "labelmanager.h"
class string_data{
public:
	map <string, int> mp;
	vector < pair <string, string> >vt;
	string insert(const string & str){
		if(mp.find(str) != mp.end()){
			for(int i = 0; i < vt.size(); ++i){
				if(vt[i].second == str)
					return vt[i].first;
			}
		}
		mp[str] = 0;
		string label = labelmanager::genlabel();
		vt.push_back(make_pair(label, str));
		return label;
	}
	void gencode(){
		for(int i = 0; i < vt.size(); ++i){
			cout << vt[i].first << " db \"" << vt[i].second << "\", 0" << endl; 
		}
	}
	static shared_ptr <string_data> single(){
		static bool flag = false;
		static shared_ptr <string_data> res;
		if(flag){
			return res;
		}
		flag = true;
		res.reset(new string_data());
		return res;
	}
};