#ifndef INCLUDE_MISSILE_TYPE
#define INCLUDE_MISSILE_TYPE

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

} // end of namespace bot

#endif

