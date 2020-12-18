#ifndef INCLUDE_BOT_SKILL_DATA
#define INCLUDE_BOT_SKILL_DATA

#include "misc/bot_time_utils.h"

namespace bot {

class Skill;

struct SkillData {
    SkillData();

    Skill* m_skill;
    TimePoint m_lastApplyTime;
};

} // end of namespace bot

#endif

