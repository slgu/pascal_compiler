#pragma once
#include "common.h"
class labelmanager{
public:
	static map <int, string> mp;
	static string searchlabel(int i){
		if(mp.find(i) == mp.end()){
			cout << "label not found" << endl;
		}
		else
			return mp[i];
	}
	static void store(int i){
		if(mp.find(i) != mp.end())
			cout << "label repeat" << endl;
		else
			mp[i] = genlabel();
	}
	static string genlabel(){
		static int x = 26 * 26 * 26;
		++x;
		int t = x;
		string res = "";
		while(t){
			res += t % 26 + 'a';
			t /= 26;
		}
		return res;
	}
};
