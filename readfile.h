#pragma once
#include "Worker.h"
#include <vector>
#include <string>

class readfile :
    public Worker
{
private:
    std::vector <std::string> arguments;
    std::vector <std::string>* b;
public:
    readfile(std::vector <std::string> v, std::vector <std::string>* buffer);

    std::vector <std::string> do_work() override;
};

