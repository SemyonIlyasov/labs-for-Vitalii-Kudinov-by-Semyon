#include "readfile.h"
#include <fstream>
using namespace std;
readfile::readfile(vector <string> v, std::vector <std::string>* b) {
	this->arguments = v;
	this->b = b;
	// vector of arguments
}

vector <string> readfile:: do_work() {
	string filename = this->arguments[0];
	ifstream in(filename);
	string buffer;
	vector <string> result;
	while (!in.eof()) {
		getline(in, buffer);
		result.push_back(buffer);
	}
	*b = result;
	return result;
}