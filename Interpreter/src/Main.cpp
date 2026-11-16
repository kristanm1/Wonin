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
    std::vector<Wonin::StmtPtr> stmts = parser.Parse();
        
    if (parser.HasError())
        return;
    
    for (auto const& stmt : stmts)
    {
        Wonin::Interpreter in;
        stmt->Accept(in);
    }
}

int main(int argc, char *argv[])
{
    // https://craftinginterpreters.com/representing-code.html

    //Scan("../../Interpreter/res/besedilo.txt");
    //Scan("../../Interpreter/res/expression.txt");
    Scan("../../Interpreter/res/statements.txt");

    return 0;
}
