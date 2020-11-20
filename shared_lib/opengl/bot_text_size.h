#ifndef INCLUDE_BOT_TEXT_SIZE
#define INCLUDE_BOT_TEXT_SIZE

namespace bot {

enum TextSize {
    TEXT_SIZE_BIG = 0,
    TEXT_SIZE_MEDIUM,
    TEXT_SIZE_SMALL,
    TEXT_SIZE_TINY,
    TEXT_SIZE_COUNT
};

inline bool isValidTextSize(TextSize sz)
{
    return sz >= TEXT_SIZE_BIG && sz < TEXT_SIZE_COUNT;
}

} // end of namespace bot

#endif
