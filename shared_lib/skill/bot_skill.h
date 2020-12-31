#ifndef INCLUDE_BOT_SKILL
#define INCLUDE_BOT_SKILL

#include "misc/bot_time_utils.h"
#include "skill/bot_skill_template.h"

namespace bot {

class GameScreen;
class Robot;

class Skill {
public:
    static Skill* create(const SkillTemplate* t, Robot* robot,
                         unsigned int level);

    Skill();

    virtual ~Skill()
    {}

    const SkillTemplate* getTemplate() const
    {
        return m_template;
    }

    SkillType getSkillType() const
    {
        return m_template->getType();
    }

    bool isOffensive() const
    {
        return m_template->isOffensive();
    }

    bool isRange() const
    {
        return m_template->isRange();
    }

    bool init(const SkillTemplate* t, Robot* robot, unsigned int level);

    virtual void apply(GameScreen& screen, const TimePoint& t);

    virtual bool available(const TimePoint& t) const;

    bool setCooldownMultiplier(float multiplier);

protected:
    const SkillTemplate* m_template;
    Robot* m_robot;
    TimePoint m_lastApplyTime;
    unsigned int m_level;
    float m_cooldownMultiplier;
};

} // end of namespace bot

#endif

