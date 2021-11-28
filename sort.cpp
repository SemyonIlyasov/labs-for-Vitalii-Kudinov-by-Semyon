#include "sort.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
sort::sort(vector <string> v, vector <string>* b) {
	this->arguments = v;
	this->b = b;
}

vector <string> sort::do_work() {
	std::sort((*b).begin(), (*b).end());
	return *b;
}