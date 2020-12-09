#ifndef INCLUDE_BOT_TEXT_SIZE
#define INCLUDE_BOT_TEXT_SIZE

#include <string>

namespace bot {

enum TextSize {
    TEXT_SIZE_BIG = 0,
    TEXT_SIZE_MEDIUM,
    TEXT_SIZE_SMALL,
    TEXT_SIZE_TINY,
    TEXT_SIZE_COUNT,
    TEXT_SIZE_INVALID
};

inline bool isValidTextSize(TextSize sz)
{
    return sz >= TEXT_SIZE_BIG && sz < TEXT_SIZE_COUNT;
}

TextSize strToTextSize(const std::string& s);

} // end of namespace bot

#endif
