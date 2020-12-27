#include "misc/bot_log.h"
#include "gameobj/bot_robot.h"
#include "skill/bot_skill.h"

namespace bot {

Skill::Skill()
    : m_template(nullptr)
    , m_robot(nullptr)
{
}

bool Skill::init(const SkillTemplate* t, Robot* robot, unsigned int level)
{
    if (!t)
    {
        LOG_ERROR("SkillTemplate is null");
        return false;
    }

    if (!robot)
    {
        LOG_ERROR("robot is null");
        return false;
    }

    m_template = t;
    m_robot = robot;
    m_lastApplyTime = Clock::now();
    m_level = level;

    return true;
}

void Skill::apply(GameScreen& screen, const TimePoint& t)
{
    Base& base = m_robot->getBase();
    float power = base.getPower() - m_template->getPowerCost();
    base.setPower(power);

    m_lastApplyTime = t;

    return true;
}

bool Skill::available(const TimePoint& t) const
{
    return m_robot->getBase().getPower() >= m_template->getPowerCost() &&
           timeDistMs(m_lastApplyTime, t) >= m_template->getCooldownMS();
}

} // end of namespace bot

