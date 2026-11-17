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
    std::vector<Wonin::Token> tokens = scanner.ScanTokens();

    if (scanner.HasError())
        return -1;
    
    Wonin::Parser parser(scanner, tokens);
    std::vector<Wonin::StmtPtr> stmts = parser.Parse();

    if (parser.HasError())
        return -2;
    
    Wonin::Interpreter in;
    in.Interpret(stmts);

    return 0;
}
