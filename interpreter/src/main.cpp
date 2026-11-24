#include <wonin/wonin.hpp>

int main(int argc, char *argv[])
{

    char* args[2] = {"", "../../interpreter/res/statements.txt"};
    return wonin::run(2, args);
}
