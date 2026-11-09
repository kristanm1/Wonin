#include "Log.h"
#include "Expression.h"
#include "Visitor.h"
#include "Scanner.h"
#include "Parser.h"

#include <memory>

void Scan(std::string path)
{
    Wonin::Scanner scanner{};
    if (scanner.LoadText(path))
    {
        if (scanner.ScanTokens())
        {
            //scanner.Display();

            Wonin::Parser parser(scanner);
            Wonin::Expression *expr = parser.Parse();

            Wonin::Interpreter in;
            expr->Accept(in);
            Wonin::Info("Result ", in.result);
        }
    }
}

void Expr()
{
    auto expr = std::make_unique<Wonin::Binary>(
        new Wonin::Unary(
            Wonin::Token::Type::Minus,
            new Wonin::Number(3.2)
        ),
        Wonin::Token::Type::Plus,
        new Wonin::Binary(
            new Wonin::Number(4.0),
            Wonin::Token::Type::Star,
            new Wonin::Number(5.0)
        )
    );

    Wonin::Interpreter in;
    expr->Accept(in);
    Wonin::Info("Result ", in.result);
}

int main(int argc, char *argv[])
{
    // https://craftinginterpreters.com/parsing-expressions.html

    //Scan("../../Interpreter/src/besedilo.txt");
    Scan("../../Interpreter/src/expression.txt");
    //Expr();

    return 0;
}
