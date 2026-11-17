#ifndef _WONIN_
#define _WONIN_

#include <string>

namespace Wonin
{
    int InterpretFromFile(std::string const& path);
    int Interpret(std::string const& source);
}

#endif
