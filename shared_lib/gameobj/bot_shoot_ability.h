#ifndef INCLUDE_BOT_SHOOT_ABILITY
#define INCLUDE_BOT_SHOOT_ABILITY

#include "misc/bot_time_utils.h"
#include "gametemplate/bot_shoot_ability_template.h"
#include "gameobj/bot_ability.h"

namespace bot {

class MissileTemplate;

class ShootAbility : public Ability {
public:
    ShootAbility(const ShootAbilityTemplate* t);

    virtual ~ShootAbility()
    {}

    const float* getShootPos() const
    {
        return static_cast<const float*>(m_shootPos);
    }

    float getShootPosX() const
    {
        return m_shootPos[0];
    }

    float getShootPosY() const
    {
        return m_shootPos[1];
    }

    bool isShootingEnabled() const
    {
        return m_shootingEnabled;
    }

    void enableShooting(bool enabled)
    {
        m_shootingEnabled = enabled;
    }

    void setShootTime();

    const ShootAbilityTemplate* getTemplate() const
    {
        return static_cast<const ShootAbilityTemplate*>(m_template);
    }

    const MissileTemplate* getMissileTemplate() const
    {
        return getTemplate()->getMissileTemplate();
    }

    bool canShoot() const;

    void shiftShootPos(float deltaX, float deltaY);

    void setShootPosDirection(float referenceX, float referenceY, float directionX, float directionY);

    float getShootInterval() const
    {
        return getTemplate()->getShootInterval() * m_shootSpeedMultiplier;
    }

    float getShootSpeedMultiplier() const
    {
        return m_shootSpeedMultiplier;
    }

    void setShootSpeedMultiplier(float multiplier)
    {
        m_shootSpeedMultiplier = multiplier;
    }

    float getDamageMultiplier() const
    {
        return m_damageMultiplier;
    }

    void setDamageMultiplier(float multiplier)
    {
        m_damageMultiplier = multiplier;
    }

protected:
    TimePoint m_lastShootTime;
    float m_shootingEnabled;
    float m_shootDirection[Constants::NUM_FLOATS_PER_POSITION];
    float m_shootPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_shootSpeedMultiplier;
    float m_damageMultiplier;
};

} // end of namespace bot

#endif
