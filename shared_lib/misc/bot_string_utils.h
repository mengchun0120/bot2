#ifndef INCLUDE_BOT_STRING_UTILS
#define INCLUDE_BOT_STRING_UTILS

#include <string>
#include <vector>

namespace bot {

std::string trim(const std::string& s);

bool isBlank(const std::string& s);

void tokenize(std::vector<std::string>& tokens, const std::string& s,
              char delimiter, bool trimSpace = true);

} // end of namespace bot

#endif
