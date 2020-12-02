#ifndef INCLUDE_BOT_GOODIE_TYPE
#define INCLUDE_BOT_GOODIE_TYPE

namespace bot {

enum GoodieType {
    GOODIE_GOLD,
    GOODIE_HEALTH,
    GOODIE_INDESTRUCTABLE,
    GOODIE_DOUBLE_EXPERIENCE,
    GOODIE_QUICK_SHOOTER,
    GOODIE_QUICK_MOVER,
    GOODIE_DOUBLE_DAMAGE,
    GOODIE_UNKNOWN,
    GOODIE_COUNT = GOODIE_UNKNOWN - GOODIE_GOLD,
    GOODIE_EFFECT_COUNT = GOODIE_UNKNOWN - GOODIE_INDESTRUCTABLE
};

inline bool isInstantaneousGoodie(GoodieType type)
{
    return type == GOODIE_GOLD || type == GOODIE_HEALTH;
}

const char* getGoodieTypeStr(GoodieType t);

} // end of namespace bot

#endif
