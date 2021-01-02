#ifndef INCLUDE_BOT_SKILL_TYPE
#define INCLUDE_BOT_SKILL_TYPE

#include <string>

namespace bot {

enum SkillType {
    SKILL_SHOOT_MISSILE,
    SKILL_COUNT,
    SKILL_INVALID = SKILL_COUNT
};

inline bool isValidSkillType(SkillType t)
{
    return t >= SKILL_SHOOT_MISSILE && t < SKILL_COUNT;
}

SkillType strToSkillType(const std::string& s);

const std::string& skillTypeToStr(SkillType t);

inline bool isShootMissileSkill(SkillType t)
{
    return t == SKILL_SHOOT_MISSILE;
}

} // end of namespace bot

#endif

