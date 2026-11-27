#include "../include/wonin/wonin.hpp"
#include "log.hpp"
#include "utils.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "interpreter.hpp"

#include <iostream>

using namespace wonin;

namespace
{

    int execute(
        Lexer& lexer,
        Parser& parser,
        Interpreter& interpreter,
        std::string const& source
    )
    {
        if (!lexer.scan(source))
            return -1;

        if (!parser.parse())
            return -2;

        if (!interpreter.interpret(parser))
            return -3;

        return 0;
    }

}

int wonin::run(int argc, char *argv[])
{
    Lexer lexer{};
    Parser parser(lexer);
    Interpreter interpreter{};

    if (argc == 1)
    {
        for (;;)
        {
            std::string line;
            std::cout << "> ";
            if (!std::getline(std::cin, line))
                break;
            
            execute(lexer, parser, interpreter, line);
        }
        return 0;
    }

    if (argc == 2)
        return execute(lexer, parser, interpreter, utils::loadText(argv[1]));

    log::info("Usage: wonin [script]");
    return -1;
}
