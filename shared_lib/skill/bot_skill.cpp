#include "misc/bot_log.h"
#include "gameobj/bot_robot.h"
#include "skill/bot_shoot_missile_skill_template.h"
#include "skill/bot_shoot_missile_skill.h"

namespace bot {

Skill *Skill::create(const SkillTemplate *t, Robot *robot, unsigned int level)
{
    switch (t->getType())
    {
        case SKILL_SHOOT_MISSILE:
        {
            const ShootMissileSkillTemplate *t1 =
                            static_cast<const ShootMissileSkillTemplate*>(t);
            ShootMissileSkill *s = new ShootMissileSkill();
            if (!s->init(t1, robot, level))
            {
                LOG_ERROR("Failed to initialize ShootMissileSkill");
                delete s;
                return nullptr;
            }
            return s;
        }
        default:
        {
            LOG_ERROR("Invalid SkillType: %d", static_cast<int>(t->getType()));
        }
    }

    return nullptr;
}

Skill::Skill()
    : m_template(nullptr)
    , m_robot(nullptr)
    , m_level(0)
    , m_cooldownMultiplier(1.0f)
{
}

bool Skill::init(const SkillTemplate *t, Robot *robot, unsigned int level)
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
    m_cooldownMultiplier = 1.0f;

    return true;
}

void Skill::apply(GameScreen &screen, const TimePoint &t)
{
    Base &base = m_robot->getBase();
    float power = base.getPower() - m_template->getPowerCost();

    if (power < 0.0f)
    {
        power = 0.0f;
    }
    base.setPower(power);

    m_lastApplyTime = t;
}

bool Skill::available(const TimePoint &t) const
{
    float cooldown = m_template->getCooldownMS() * m_cooldownMultiplier;
    return m_robot->getBase().getPower() >= m_template->getPowerCost() &&
           timeDistMs(m_lastApplyTime, t) >= cooldown;
}

bool Skill::setCooldownMultiplier(float multiplier)
{
    if (multiplier < 0)
    {
        LOG_ERROR("Invalid multiplier %f", multiplier);
        return false;
    }

    m_cooldownMultiplier = multiplier;

    return true;
}

} // end of namespace bot

