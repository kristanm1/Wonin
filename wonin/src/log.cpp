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

void log::lexerError(std::string message, usize lineNumber)
{
    error("Lexer error: ", message, " at line: ", lineNumber);
}
