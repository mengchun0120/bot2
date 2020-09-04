#ifndef INCLUDE_BOT_FILE_UTILS
#define INCLUDE_BOT_FILE_UTILS

#include <string>
#include <rapidjson/document.h>

namespace bot {

std::string getFileSeparator();

std::string constructPath(std::initializer_list<std::string> s);

bool readText(std::string& text, const std::string& fileName);

} // end of namespace bot

#endif
