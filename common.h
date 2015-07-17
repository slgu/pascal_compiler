#pragma once
#include <cstdio>
#include <cstring>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <deque>
#include <stack>
using namespace std;

#define MUL_TYPE 3265
#define DIV_TYPE 3266
#define PLUS_TYPE 3267
#define MINUS_TYPE 3268
#define GE_TYPE 3270
#define GT_TYPE 3271
#define LE_TYPE 3272
#define LT_TYPE 3273
#define EQUAL_TYPE 3274
#define MOD_TYPE 3292
#define UNEQUAL_TYPE 3293
#define NOT_TYPE 3295
#define AND_TYPE 3296
extern int lineno;
extern bool flag;
inline double atof(const char * str){
	int l = strlen(str);
	double total = 0;
	int shift = 0;
	while((*str) != '\0'){	
		if(*str == '.')
			shift = -shift;
		else{
			total = total * 10 + *str - '0';
		}

		if(shift >= 0)	
			++shift;
		++str; 
	}
	shift = l + shift - 1;
	while(shift--){
		total /= 10;
	}
	return total;
}
inline string itoa(int x){
	char _int[20];
	sprintf(_int, "%d", x);
	return string(_int);
}
inline string ftoa(double x){
	char real[20];
	sprintf(real, "%.2f", x);
	return string(real);
}
