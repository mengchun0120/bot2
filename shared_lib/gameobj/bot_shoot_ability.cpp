#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "gameobj/bot_shoot_ability.h"

namespace bot {

ShootAbility::ShootAbility(const ShootAbilityTemplate* t)
    : Ability(t)
    , m_shootingEnabled(false)
    , m_shootSpeedMultiplier(1.0f)
    , m_damageMultiplier(1.0f)
{
    m_lastShootTime = Clock::now();
    m_shootDirection[0] = 1.0f;
    m_shootDirection[1] = 0.0f;
    m_shootPos[0] = 0.0f;
    m_shootPos[1] = 0.0f;
}

void ShootAbility::setShootTime()
{
    m_lastShootTime = Clock::now();
}

bool ShootAbility::canShoot() const
{
    if (!m_shootingEnabled)
    {
        return false;
    }

    if (elapsedTimeMs(m_lastShootTime) >= getShootInterval())
    {
        return true;
    }

    return false;
}

void ShootAbility::shiftShootPos(float deltaX, float deltaY)
{
    m_shootPos[0] += deltaX;
    m_shootPos[1] += deltaY;
}

void ShootAbility::setShootPosDirection(float referenceX, float referenceY, float directionX, float directionY)
{
    float x = getTemplate()->getShootPosX();
    float y = getTemplate()->getShootPosY();
    rotate(x, y, directionX, directionY);
    m_shootPos[0] = referenceX + x;
    m_shootPos[1] = referenceY + y;
    m_shootDirection[0] = directionX;
    m_shootDirection[1] = directionY;
}

} // end of namespace bot
