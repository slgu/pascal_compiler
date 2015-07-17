#include <cstdio>
#include <memory>
#include <deque>
using namespace std;
int * p;
int main(){
	shared_ptr <int> c;
	c.reset(p);
	deque <int> q;
	q.push_back(1);
	q.pop_back();
	return 0;
}