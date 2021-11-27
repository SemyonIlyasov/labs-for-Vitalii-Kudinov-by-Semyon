#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;


typedef struct block
{
	string operation;
	vector <string> arguments;
}block;


class WorkflowParser {
private:
	map <int, block> methods;
	map <int, block> ::iterator it;
	vector <int> order;
	vector <string> existing_methods;
	char** argv;
	int argc;
	bool find_desc_or_csed(string s);
	bool read_methods(ifstream& in);
	bool read_order(string str);
	bool fill_existing_methods();
	bool validation();
public:
	void show_methods();
	void show_order();
	void set_argc(int a);
	void set_argv(char** a);
	WorkflowParser();
	WorkflowParser(WorkflowParser& p);
	WorkflowParser(WorkflowParser&& p);
	map <int, block> get_methods();
	bool check_file_cond(string s);
	bool check_input();
	bool parse_file();



};