#include "../include/wonin/wonin.hpp"
#include "log.hpp"
#include "utils.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "interpreter.hpp"

#include <iostream>

using namespace wonin;

namespace
{

    int execute(Environment& environment, std::string const& source)
    {
        Scanner scanner(source);

        if (!scanner.scan())
            return -1;
        
        Parser parser(scanner);

        if (!parser.parse())
            return -2;
        
        Interpreter(parser).interpret(environment);

        return 0;
    }

}

int wonin::interpret(std::string const& source)
{
    Environment environment{};
    return execute(environment, source);
}

int wonin::run(int argc, char *argv[])
{
    Environment environment{};

    if (argc == 1)
    {
        for (;;)
        {
            std::string line;
            std::cout << "> ";
            if (!std::getline(std::cin, line))
                break;
            
            execute(environment, line);
        }
        return 0;
    }

    if (argc == 2)
    {
        return execute(environment, utils::loadText(argv[1]));
    }

    log::info("Usage: wonin [script]");
    return -1;
}
