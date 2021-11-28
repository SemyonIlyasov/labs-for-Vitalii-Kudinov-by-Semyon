#include "replace.h"
#include <vector>
#include <string>
using namespace std;
replace::replace(vector <string> v, vector <string>* b) {
	this->arguments = v;
	this->b = b;
}

vector <string> replace::do_work() {
	vector <string> res;
	string word1 = arguments[0];
	string word2 = arguments[1];
	string localstring = "";
	vector <string> ::iterator i = (*b).begin();
	for (; i != (*b).end(); i++) {
		localstring = (*i);
		size_t pos = 0;
		while ((pos = localstring.find(word1)) != string::npos) {
			localstring.replace(pos, word1.size(), word2);
			pos += word2.size();
		}
		res.push_back(localstring);
	}
	*b = res;
	return *b;
}