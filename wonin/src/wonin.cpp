#include "../include/wonin/wonin.hpp"

#include "log.hpp"
#include "utils.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

int Wonin::InterpretFromFile(std::string const& path)
{
    return Interpret(Utils::LoadText(path));
}

int Wonin::Interpret(std::string const& source)
{
    Wonin::Scanner scanner(source);
    if (!scanner.ScanTokens())
        return -1;
    
    Wonin::Parser parser(scanner);

    if (!parser.Parse())
        return -2;
    
    Wonin::Interpreter(parser).Interpret();

    return 0;
}
