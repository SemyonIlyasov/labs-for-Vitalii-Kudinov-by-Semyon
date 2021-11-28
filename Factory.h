#pragma once
#include <string>
#include "Worker.h"
#include <vector>
using namespace std;

class Factory
{
public:
	Factory();
	virtual Worker* Create(string op, std::vector <std::string> v, vector <string> * buffer);
};

