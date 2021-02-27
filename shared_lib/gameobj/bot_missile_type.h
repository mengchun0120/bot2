#ifndef INCLUDE_MISSILE_TYPE
#define INCLUDE_MISSILE_TYPE

#include <string>

namespace bot {

enum MissileType {
    MISSILE_BULLET,
    MISSILE_SHELL,
    MISSILE_DECK_PIERCER,
    MISSILE_COUNT,
    MISSILE_INVALID = MISSILE_COUNT
};

inline bool isValidMissileType(MissileType t)
{
    return t >= MISSILE_BULLET && t < MISSILE_COUNT;
}

const std::string &missileTypeToStr(MissileType type);

MissileType strToMissileType(const std::string &typeStr);

} // end of namespace bot

#endif

