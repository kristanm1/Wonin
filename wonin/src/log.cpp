#include "log.hpp"

#include <iostream>

void Wonin::Log::InfoImpl(std::string const& s)
{
    std::cout << s << std::endl;
}

void Wonin::Log::ErrorImpl(std::string const& s)
{
    std::cerr << s << std::endl;
}
