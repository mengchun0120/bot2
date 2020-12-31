#ifndef INCLUDE_BOT_SKILL_TYPE
#define INCLUDE_BOT_SKILL_TYPE

#include <string>

namespace bot {

enum SkillType {
    SKILL_SHOOT_BULLET,
    SKILL_SHOOT_SHELL,
    SKILL_SHOOT_DECK_PIERCER,
    SKILL_COUNT,
    SKILL_INVALID = SKILL_COUNT
};

inline bool isValidSkillType(SkillType t)
{
    return t >= SKILL_SHOOT_BULLET && t < SKILL_COUNT;
}

SkillType strToSkillType(const std::string& s);

const std::string& skillTypeToStr(SkillType t);

inline bool isShootMissileSkill(SkillType t)
{
    return t >= SKILL_SHOOT_BULLET && t <= SKILL_SHOOT_DECK_PIERCER;
}

} // end of namespace bot

#endif

