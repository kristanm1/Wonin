#include <wonin/wonin.hpp>

int main(int argc, char *argv[])
{
    std::string source = std::string("print true; print 1 + (2 * 3); print \"tralala\";");
    Wonin::Interpret(source);

    return 0;
}
