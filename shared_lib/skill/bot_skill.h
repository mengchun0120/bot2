#ifndef INCLUDE_BOT_SKILL
#define INCLUDE_BOT_SKILL

#include "misc/bot_time_utils.h"

namespace bot {

class SkillTemplate;
class GameScreen;

class Skill {
public:
    Skill();

    virtual ~Skill();

    bool init(const SkillTemplate* t, Robot* robot, unsigned int level);

    virtual void apply(GameScreen& screen, const TimePoint& t);

    virtual bool available(const TimePoint& t) const;

protected:
    const SkillTemplate* m_template;
    Robot* m_robot;
    TimePoint m_lastApplyTime;
    unsigned int m_level;
};

} // end of namespace bot

#endif

