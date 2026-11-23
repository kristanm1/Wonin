#include "log.hpp"

#include <iostream>

using namespace wonin;

void log::infoImpl(std::string const& s)
{
    std::cout << s << std::endl;
}

void log::errorImpl(std::string const& s)
{
    std::cerr << s << std::endl;
}

void log::scannerError(std::string message, usize lineNumber)
{
    error("Scanner error: ", message, " at line: ", lineNumber);
}
