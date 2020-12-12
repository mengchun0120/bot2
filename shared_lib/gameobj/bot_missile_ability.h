#ifndef INCLUDE_BOT_MISSILE_ABILITY
#define INCLUDE_BOT_MISSILE_ABILITY

namespace bot {

enum MissileAbility {
    MISSILE_ABILITY_NONE,
    MISSILE_ABILITY_PENETRATE
};

inline bool isValidMissileAbility(MissileAbility a)
{
    return a >= MISSILE_ABILITY_NONE && a <= MISSILE_ABILITY_PENETRATE;
}

} // end of namespace bot

#endif

