#pragma once
#include "Worker.h"
class replace :
    public Worker
{
private:
	std::vector <std::string> arguments;
	std::vector <std::string>* b;
public:
	replace(std::vector <std::string> v, std::vector <std::string>* b);
	std::vector <std::string> do_work();
};

