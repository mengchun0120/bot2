#include "gameobj/bot_missile_type.h"

namespace bot {

static const std::string k_missileTypeStr[] = {
    "bullet",
    "shell",
    "deck-piercer",
    "invalid"
};

const std::string &missileTypeToStr(MissileType type)
{
    if (!isValidMissileType(type))
    {
        return k_missileTypeStr[MISSILE_INVALID];
    }

    return k_missileTypeStr[type];
}

MissileType strToMissileType(const std::string &typeStr)
{
    for (int i = 0; i < MISSILE_COUNT; ++i)
    {
        if (k_missileTypeStr[i] == typeStr)
        {
            return static_cast<MissileType>(i);
        }
    }

    return MISSILE_INVALID;
}

} // end of namespace bot

