#include <wonin/wonin.hpp>

int main(int argc, char *argv[])
{


    std::string source = 
    //"var b = true;"
    //"print b;"
    //"var s = \"tralala\";"
    //"print s;"
    //"var n = ((1 + 2) - (3 * 2)) / -3;"
    //"print n;"
    "print z;";

    Wonin::Interpret(source);

    return 0;
}
