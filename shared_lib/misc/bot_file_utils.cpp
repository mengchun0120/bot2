#include <fstream>
#include <sstream>
#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"

namespace bot {

std::string getFileSeparator()
{
#ifdef _WIN32
    return "\\";
#elif __linux__
    return "/";
#endif
}

std::string constructPath(std::initializer_list<std::string> s)
{
    if (s.size() == 0) {
        return "";
    }

    std::ostringstream oss;
    std::string separator = getFileSeparator();
    auto it = s.begin();

    oss << *it;
    for (++it; it != s.end(); ++it) {
        oss << separator << *it;
    }

    return oss.str();
}

bool readText(std::string& text, const std::string& fileName)
{
    std::ifstream in;

    in.open(fileName);
    if (in.fail()) {
        LOG_ERROR("Failed to open file %s", fileName.c_str());
        return false;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    text = ss.str();

    if (in.bad()) {
        LOG_ERROR("Error happened while reading file %s", fileName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
