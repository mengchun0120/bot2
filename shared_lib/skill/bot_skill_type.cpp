#include <vector>
#include "skill/bot_skill_type.h"

namespace bot {

static std::vector<std::string> k_skillTypeStr = {
    "shoot-bullet",
    "shoot-shell",
    "shoot-deck-piercer",
    "invalid"
};

SkillType strToSkillType(const std::string& s)
{
    for (int t = SKILL_SHOOT_BULLET; t < SKILL_COUNT; ++t)
    {
        if (k_skillTypeStr[t] == s)
        {
            return static_cast<SkillType>(t);
        }
    }

    return SKILL_INVALID;
}

const std::string& skillTypeToStr(SkillType t)
{
    return isValidSkillType(t) ? k_skillTypeStr[t] :
                                 k_skillTypeStr[SKILL_INVALID];
}

} // end of namespace bot

