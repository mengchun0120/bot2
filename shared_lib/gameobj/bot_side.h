#ifndef INCLUDE_BOT_SIDE
#define INCLUDE_BOT_SIDE

namespace bot {

enum Side {
    SIDE_AI,
    SIDE_PLAYER,
    SIDE_UNKNOWN
};

inline bool isValid(Side side)
{
    return side >= SIDE_AI && side <= SIDE_PLAYER;
}

} // end of namespace bot

#endif
