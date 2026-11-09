#include "Log.h"
#include <iostream>

void Wonin::LogImpl(std::string const& s)
{
    std::cout << s << std::endl;
}