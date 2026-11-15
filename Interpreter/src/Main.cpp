#include "Log.hpp"
#include "Utils.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"

void Scan(std::string path)
{
    std::string source = Wonin::Utils::LoadText(path);

    Wonin::Scanner scanner(source);
    std::vector<Wonin::Token> tokens = scanner.ScanTokens();

    //scanner.PrintTokens(tokens);

    if (scanner.HasError())
        return;
    
    Wonin::Parser parser(scanner, tokens);
    Wonin::ExprPtr expr = parser.Parse();
        
    if (parser.HasError())
        return;
    
    Wonin::Interpreter in;
    Wonin::Value value = expr->Accept(in);
    Wonin::Log::Info("Result ", value.GetNumber());
}

int main(int argc, char *argv[])
{
    // https://craftinginterpreters.com/representing-code.html

    //Scan("../../Interpreter/src/besedilo.txt");
    Scan("../../Interpreter/src/expression.txt");

    return 0;
}
