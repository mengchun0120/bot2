#include <vector>
#include "skill/bot_skill_type.h"

namespace bot {

static std::vector<std::string> k_skillTypeStr = {
    "Shoot",
    "Penetrate Shoot",
    "Invalid Skill"
};

SkillType strToSkillType(const std::string& s)
{
    for (SkillType t = SKILL_SHOOT; t < SKILL_COUNT; ++i)
    {
        if (k_skillTypeStr[t] == s)
        {
            return t;
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

