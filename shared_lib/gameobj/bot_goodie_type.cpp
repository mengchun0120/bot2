#include "gameobj/bot_goodie_type.h"

namespace bot {

const char* getGoodieTypeStr(GoodieType t)
{
    static const char* TYPE_STR[] = {
        "Gold",
        "Health",
        "Indestructable",
        "Double Experience",
        "Quick Shooter",
        "Quick Mover",
        "Double Damage"
    };

    if (t < GOODIE_GOLD || t > GOODIE_DOUBLE_DAMAGE)
    {
        return "";
    }

    return TYPE_STR[t];
}

} // end of namespace bot

