#include "Factory.h"
#include "readfile.h"
#include "writefile.h"
#include "grep.h"
#include "replace.h"
#include "sort.h"
#include "dump.h"
Factory::Factory() {

}

Worker* Factory::Create(string op, vector <string> v, vector <string>* buffer) {
	if (op == "writefile")	return new writefile(v, buffer);
	if (op == "readfile")	return new readfile(v, buffer);
	if (op == "sort")		return new sort(v, buffer);
	if (op == "grep")		return new grep(v, buffer);
	if (op == "replace")	return new replace(v, buffer);
	if (op == "dump")		return new dump(v, buffer);
	return new Worker();
}
