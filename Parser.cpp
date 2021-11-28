#pragma once
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <list>
using namespace std;		// not allowed

	bool WorkflowParser::find_desc_or_csed(string s) {
		int i = 0;
		if (s == "") return false;
		for (; i < s.length() && s[i] == ' '; i++);
		if ((i < s.length() - 4) &&
			s[i] == 'd' &&
			s[i + 1] == 'e' &&
			s[i + 2] == 's' &&
			s[i + 3] == 'c'
			||
			s[i] == 'c' &&
			s[i + 1] == 's' &&
			s[i + 2] == 'e' &&
			s[i + 3] == 'd') {
			i += 4;
			for (; i < s.length() && s[i] == ' '; i++);
			if (i >= s.length() || s[i] == '\n')
				return true;
		}
		return false;
	}

	bool WorkflowParser::fill_existing_methods() {
		this->existing_methods.push_back("writefile");
		this->existing_methods.push_back("readfile");
		this->existing_methods.push_back("sort");
		this->existing_methods.push_back("grep");
		this->existing_methods.push_back("dump");
		this->existing_methods.push_back("replace");
		return true;
	}

	bool WorkflowParser::validation() {
		for (map <int, block> ::iterator map_it = methods.begin(); map_it != methods.end(); map_it++)
		{
			bool method_exists = false;
			for (vector <string> ::iterator methods_names = existing_methods.begin(); methods_names != existing_methods.end() && !method_exists; methods_names++) {
				if (*methods_names == map_it->second.operation) {
					method_exists = true;
					break;
				}
			}
			if (method_exists == false) return false;
		}
		// при желании можно добавить проверку на уникальность id методов, проверку на правильные id в последовательности для выполнения и т.д.
		return true;
	}

	void WorkflowParser::set_argc(int a) {
		argc = a;
	}

	void WorkflowParser::set_argv(char** a) {
		argv = a;
	}

	WorkflowParser::WorkflowParser() {
		argv = NULL;
		argc = 0;
		fill_existing_methods();
	}

	WorkflowParser::WorkflowParser(WorkflowParser& p) {
		argc = p.argc;
		argv = p.argv;
		fill_existing_methods();
	}

	WorkflowParser::WorkflowParser(WorkflowParser&& p) {
		argc = p.argc;
		argv = p.argv;
		fill_existing_methods();

		for (int i = 0; i < p.argc; i++)
			free(argv[i]);
		free(argv);
		
	}

	bool  WorkflowParser::read_order(string str) {
		stringstream ss(str);
		vector <int> vec;
		string numstr = "";
		int numint = 0;
		bool num_is_read = false;
		for (; ss >> numstr; ) {
			if (!num_is_read && (numint = strtol(numstr.c_str(), 0, 0))) {
				vec.push_back(numint);
				num_is_read = true;
			}
			else if (num_is_read && numstr == "->")
				num_is_read = false;
			else
				return false;
		}
		this->order = vec;
		return true;
	}

	void WorkflowParser::show_order() {
		for (vector<int>::iterator vec_it = order.begin(); vec_it != order.end(); ++vec_it) {
			cout << *vec_it << " ";
		}
		cout << "\n";
	}

	vector <int> WorkflowParser::get_order() {
		return order;
	}

	map <int, block> WorkflowParser::get_methods() {
		return methods;
	}

	bool WorkflowParser::check_file_cond(string s) { //File not found exception
		ifstream input(s);
		if (!input.is_open())
		{
			cout << "File not found" << '\n';
			input.close();
			return false;
		}
		else if (input.peek() == EOF)
		{
			cout << "File is empty" << '\n';
			input.close();
			return false;
		}
		input.close();
		return true;
	}

	void WorkflowParser::show_methods() {
		map <int, block> ::iterator it = this->methods.begin();
		for (; it != this->methods.end(); it++)
		{
			cout << it->first << ": " << it->second.operation << " ";
			for (vector<string>::iterator vec_it = it->second.arguments.begin(); vec_it != it->second.arguments.end(); ++vec_it) {
				cout << *vec_it << " ";
			}
			cout << "\n";
		}
	}

	bool WorkflowParser::check_input() {					// Exception: "Usage: <programm name> <Conf file name>"
		if (argc < 2) {
			cout << "Usage: <programm name> <Conf file name>" << '\n';
			return false;
		}
		return true;
	}

	bool WorkflowParser::read_methods(ifstream& in) {
		string str = "", word = ""; 
		getline(in, str);
		while (!find_desc_or_csed(str))
		{
			stringstream ss(str);
			int operid = 0;
			string func = "";
			vector <string> params;
			bool idRead = false;
			bool eqRead = false;
			bool functionRead = false;
			for (; ss >> word; ) {
				if (!idRead && (operid = strtol(word.c_str(), 0, 0)))
					idRead = true;
				else if (!eqRead && word == "=")
					eqRead = true;
				else if (!functionRead) {
					func = word;
					functionRead = true;
				}
				else if (eqRead && functionRead && idRead)
					params.push_back(word);
				else
					return false;
			}
			block *b = new block;
			b->operation = func;
			b->arguments = params;
			this->methods[operid] = *b;
			delete b;
			getline(in, str);
		}

		return find_desc_or_csed(str);

	};

	bool WorkflowParser::parse_file() {
		cout << "start reading config...\n";
		cout << "check command line...\n";
		if (!check_input()) return false;			//Command line input exception
		cout << "correct!\n";
		string filename = argv[1];
		cout << "check file is not empty...\n";
		if (!check_file_cond(filename))  return false;	//Input file exception
		cout << "correct!\n";
		ifstream input(filename);
		string str = "";
		getline(input, str);
		cout << "check config word(desc or csed)...\n";
		if (!find_desc_or_csed(str)) return false; //File structure exception
		cout << "correct!\n";
		cout << "read methods of the factory...\n";
		if (!read_methods(input)) return false;
		cout << "correct!\n";
		cout << "read order...\n";
		getline(input, str);
		if (!read_order(str)) return false;
		cout << "correct!\n";
		cout << "validation...\n";
		if (!validation()) return false;
		cout << "correct!\n";
		cout << "Reading finished successfully!\n";

		cout << "list of methods: \n";
		this->show_methods();
		cout << "list of order:\n";
		this->show_order();

		return true;
	}



