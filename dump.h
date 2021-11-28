#pragma once
#include "Worker.h"
class dump :
    public Worker
{
private:
	std::vector <std::string> arguments;
	std::vector <std::string>* b;
public:
	dump(std::vector <std::string> v, std::vector <std::string>* b);
	std::vector <std::string> do_work();
};

