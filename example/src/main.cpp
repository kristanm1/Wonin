#include <wonin/wonin.hpp>

int main(int argc, char *argv[])
{

    std::string source = std::string("print 1 - (2 * 3);");
    Wonin::Interpret(source);

    return 0;
}
