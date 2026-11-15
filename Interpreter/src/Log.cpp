#include "Log.hpp"
#include <iostream>

void Wonin::Log::LogImpl(std::string const& s)
{
    std::cout << s << std::endl;
}