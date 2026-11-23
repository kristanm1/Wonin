#include "utils.hpp"
#include <fstream>

using namespace wonin;

std::string utils::loadText(std::string const& path)
{
    std::fstream fs(path, std::ifstream::in | std::ifstream::binary);
    if (!fs.is_open())
    {
        return {};
    }

    std::streampos pos = fs.seekg(0, std::ios::end).tellg();
    if (std::streampos(-1) == pos)
    {
        return {};
    }

    fs.seekg(0, std::ios::beg);
    size_t size = static_cast<size_t>(pos);

    std::string content(size, '\0');
    fs.read(&content[0], size);

    return content;
}
