#ifndef INCLUDE_BOT_SKILL_TYPE
#define INCLUDE_BOT_SKILL_TYPE

#include <string>

namespace bot {

enum SkillType {
    SKILL_SHOOT,
    SKILL_PENETRATE_SHOOT,
    SKILL_COUNT,
    SKILL_INVALID = SKILL_COUNT
};

inline bool isValidSkillType(SkillType t)
{
    return t >= SKILL_SHOOT && t < SKILL_COUNT;
}

SkillType strToSkillType(const std::string& s);

const std::string& skillTypeToStr(SkillType t);

} // end of namespace bot

#endif

