#include "writefile.h"
#include <vector>
#include <string>
using namespace std;
writefile::writefile(vector <string> v, vector <string>* b) {
	this->arguments = v;
	this->b = b;
}

vector <string> writefile::do_work() {
	string filename = this->arguments[0];
	ofstream out(filename);
	vector <string> ::iterator i = (*b).begin();
	for (; i != (*b).end(); i++)
		out << *i << "\n";
	vector <string> empty;
	*b = empty;
	return *b;
}