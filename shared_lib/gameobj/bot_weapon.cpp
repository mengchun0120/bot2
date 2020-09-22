#include "misc/bot_math_utils.h"
#include "gameobj/bot_weapon_component.h"

namespace bot {

WeaponComponent::WeaponComponent(const WeaponComponentTemplate* weaponTemplate, const MissileTemplate* missileTemplate)
    : Component(weaponTemplate)
    , m_missileTemplate(missileTemplate)
    , m_firing(false)
    , m_lastFireTime(Clock::now())
{
}

void WeaponComponent::update(GameScreen& screen)
{
    if (!m_firing)
    {
        return;
    }

    TimePoint now = Clock::now();

    if (timeDistMs(now, m_lastFireTime) < getFireDuration())
    {
        return;
    }

    m_lastFireTime = now;
}

void WeaponComponent::shiftFirePoints(float deltaX, float deltaY)
{
    for (auto& fp: m_firePoints)
    {
        fp.m_firePos[0] += deltaX;
        fp.m_firePos[1] += deltaY;
    }
}

void WeaponComponent::setFirePoints(float weaponX, float weaponY, float directionX, float directionY)
{
    const WeaponComponentTemplate* t = getTemplate();
    int count = static_cast<int>(m_firePoints.size());

    for (int i = 0; i < count; ++i)
    {
        FirePoint& p1 = m_firePoints[i];
        const FirePoint& p2 = t->getFirePoint(i);
        float dx = p2.m_firePos[0];
        float dy = p2.m_firePos[1];

        rotate(dx, dy, directionX, directionY);
        p1.m_firePos[0] = weaponX + dx;
        p1.m_firePos[1] = weaponY + dy;

        dx = p2.m_fireDirection[0];
        dy = p2.m_fireDirection[1];
        rotate(dx, dy, directionX, directionY);
        p1.m_fireDirection[0] = dx;
        p1.m_fireDirection[1] = dy;
    }
}

} // end of namespace bot

