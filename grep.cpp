#include "grep.h"
#include <vector>
#include <string>
using namespace std;
grep::grep(vector <string> v, vector <string>* b) {
	this->arguments = v;
	this->b = b;
}

vector <string> grep::do_work() {
	vector <string> res;
	string word = arguments[0];
	vector <string> ::iterator i = (*b).begin();
	for (; i != (*b).end(); i++)
		if ((*i).find(word) != string::npos)
			res.push_back(*i);
	*b = res;
	return res;
}