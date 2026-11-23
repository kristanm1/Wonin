#ifndef _WONIN_
#define _WONIN_

#include <string>

namespace wonin
{

    int run(int argc, char *argv[]);
    
    int interpret(std::string const& source);

}

#endif
