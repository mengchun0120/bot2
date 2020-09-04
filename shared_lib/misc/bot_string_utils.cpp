#include <list>
#include "misc/bot_string_utils.h"

namespace bot {

std::string trim(const std::string& s)
{
    int len = static_cast<int>(s.length());
    int start;

    for (start = 0; start < len; ++start) {
        if (!isspace(s[start])) {
            break;
        }
    }

    if (start >= len) {
        return "";
    }

    int end;

    for (end = len - 1; end >= 0; --end) {
        if (!isspace(s[end])) {
            break;
        }
    }

    return s.substr(start, end - start + 1);
}


bool isBlank(const std::string& s)
{
    int sz = static_cast<int>(s.size());
    for (int i = 0; i < sz; ++i) {
        if (!isspace(s[i])) {
            return false;
        }
    }

    return true;
}

void tokenize(std::vector<std::string>& tokens, const std::string& s,
    char delimiter, bool trimSpace)
{
    std::list<std::string> tokenList;
    int sz = static_cast<int>(s.size());
    int lastDelim = -1;

    while (true) {
        int j = lastDelim + 1;
        while (j < sz && s[j] != delimiter) {
            ++j;
        }

        std::string token = std::move(s.substr(lastDelim + 1, j - lastDelim - 1));
        if (trimSpace) {
            token = std::move(trim(token));
        }

        tokenList.push_back(std::move(token));

        if (j >= sz) {
            break;
        }

        lastDelim = j;
    }

    if (tokenList.size() == 0) {
        return;
    }

    int count = static_cast<int>(tokenList.size());
    tokens.resize(count);
    auto it = tokenList.begin();

    for (int i = 0; i < count; ++i, ++it) {
        tokens[i] = std::move(*it);
    }
}

} // end of namespace bot
