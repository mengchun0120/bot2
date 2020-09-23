#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "opengl/bot_graphics.h"
#include "gametempalte/bot_weapon_template.h"
#include "gametemplate/bot_missile_template.h"
#include "gameobj/bot_base.h"
#include "gameobj/bot_weapon.h"
#include "screen/bot_game_screen.h"

namespace bot {

Weapon::Weapon()
    : m_weaponTemplate(nullptr)
    , m_weaponLevel(1)
    , m_missileTemplate(nullptr)
    , m_missileLevel(1)
    , m_firing(false)
    , m_fireDuration(0.0f)
    , m_fireDurationMultiplier(1.0f)
    , m_damageMultiplier(1.0f)
{
}

bool Weapon::init(const WeaponTemplate* weaponTemplate, int weaponLevel,
                  const MissileTemplate* missileTemplate, int missileLevel,
                  float weaponX, float weaponY, float directionX, float directionY)
{
    m_weaponTemplate = weaponTemplate;
    if (weaponLevel < 1)
    {
        LOG_ERROR("Invalid weapon-level %d", weaponLevel);
        return false;
    }
    m_weaponLevel = weaponLevel;

    m_missileTemplate = missileTemplate;
    if (missileLevel < 1)
    {
        LOG_ERROR("Invalid missile-level %d", missileLevel);
        return false;
    }
    m_missileLevel = missileLevel;

    m_firing = false;
    m_fireDurationMultiplier = 1.0f;
    m_damageMultiplier = 1.0f;

    resetFireDuration();

    m_firePoints.resize(m_weaponTemplate->numFirePoints());
    setFirePoints(weaponX, weaponY, directionX, directionY);

    m_lastFireTime = Clock::now();

    return true;
}

void Weapon::update(GameScreen& screen, Base& base)
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

    fireMissile(screen, base);
    m_lastFireTime = now;
}

void Weapon::present(Graphics& g, const float* pos, const float* direction)
{
    m_weaponTemplate->getRect()->draw(g, pos, direction, nullptr, nullptr,
                                      m_weaponTemplate->getTexture()->textureId, nullptr);
}

void Weapon::shiftFirePoints(float deltaX, float deltaY)
{
    for (auto& fp: m_firePoints)
    {
        fp.m_firePos[0] += deltaX;
        fp.m_firePos[1] += deltaY;
    }
}

void Weapon::setFirePoints(float weaponX, float weaponY, float directionX, float directionY)
{
    int count = static_cast<int>(m_firePoints.size());

    for (int i = 0; i < count; ++i)
    {
        FirePoint& p1 = m_firePoints[i];
        const FirePoint& p2 = m_weaponTemplate->getFirePoint(i);
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

bool Weapon::setFireDurationMultiplier(float multiplier)
{
    if (multiplier <= 0.0f)
    {
        LOG_ERROR("Invalid fire-duration multiplier %f", multiplier);
        return false;
    }

    m_fireDurationMultiplier = multiplier;
    resetFireDuration();

    return true;
}

bool Weapon::setDamageMultiplier(float multiplier)
{
    if (multiplier <= 0.0f)
    {
        LOG_ERROR("Invalid damage multiplier %f", multiplier);
        return false;
    }

    m_damageMultiplier = multiplier;

    return true;
}

void Weapon::resetFireDuration()
{
    m_fireDuration = m_weaponTemplate->getFireDuration(m_weaponLevel) * m_fireDurationMultiplier;
}

void Weapon::fireMissile(GameScreen& screen, Base& base)
{
    //TODO
}

} // end of namespace bot

