#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "opengl/bot_graphics.h"
#include "gametemplate/bot_weapon_template.h"
#include "gametemplate/bot_missile_template.h"
#include "gameobj/bot_robot.h"
#include "screen/bot_game_screen.h"

namespace bot {

Weapon::Weapon()
    : m_weaponTemplate(nullptr)
    , m_missileLevel(1)
    , m_firing(false)
    , m_normalFireDuration(0.0f)
    , m_fireDuration(0.0f)
    , m_fireDurationMultiplier(1.0f)
    , m_damage(0.0f)
    , m_damageMultiplier(1.0f)
{
}

bool Weapon::init(const WeaponTemplate* weaponTemplate, int weaponLevel,
                  int missileLevel, float weaponX, float weaponY,
                  float directionX, float directionY)
{
    if (!weaponTemplate)
    {
        LOG_ERROR("Weapon template is null");
        return false;
    }

    if (weaponLevel < 0)
    {
        LOG_ERROR("Invalid weapon-level %d", weaponLevel);
        return false;
    }

    if (missileLevel < 0)
    {
        LOG_ERROR("Invalid missile-level %d", missileLevel);
        return false;
    }

    m_weaponTemplate = weaponTemplate;
    m_missileLevel = missileLevel;

    m_firing = false;
    m_normalFireDuration = weaponTemplate->getFireDuration(weaponLevel);
    m_fireDurationMultiplier = 1.0f;
    resetFireDuration();

    m_damageMultiplier = 1.0f;
    resetDamage();

    m_firePoints.resize(m_weaponTemplate->numFirePoints());
    setFirePoints(weaponX, weaponY, directionX, directionY);

    m_lastFireTime = Clock::now();

    return true;
}

bool Weapon::update(GameScreen& screen, Robot& robot)
{
    if (!m_firing)
    {
        return true;
    }

    TimePoint now = Clock::now();
    float dur = timeDistMs(m_lastFireTime, now);
    if (dur < m_fireDuration)
    {
        return true;
    }

    if (!fireMissile(screen, robot))
    {
        return false;
    }

    m_lastFireTime = now;

    return true;
}

void Weapon::present(Graphics& g, const float* pos, const float* direction)
{
    m_weaponTemplate->getRect()->draw(
                                g, pos, direction, nullptr, nullptr,
                                m_weaponTemplate->getTexture()->textureId(),
                                nullptr);
}

void Weapon::shiftFirePoints(float deltaX, float deltaY)
{
    for (auto& fp: m_firePoints)
    {
        fp.m_firePos[0] += deltaX;
        fp.m_firePos[1] += deltaY;
    }
}

void Weapon::setFirePoints(float weaponX, float weaponY,
                           float directionX, float directionY)
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
    resetDamage();

    return true;
}

void Weapon::resetFireDuration()
{
    const float MIN_FIRE_DURATION = 100.0f;
    m_fireDuration = m_normalFireDuration * m_fireDurationMultiplier;
    if (m_fireDuration < MIN_FIRE_DURATION)
    {
        m_fireDuration = MIN_FIRE_DURATION;
    }
}

void Weapon::resetDamage()
{
    const MissileTemplate* t = m_weaponTemplate->getMissileTemplate();
    m_damage = t->getDamage(m_missileLevel) * m_damageMultiplier;
}

bool Weapon::fireMissile(GameScreen& screen, Robot& robot)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    GameMap& map = screen.getMap();

    for(auto& fp: m_firePoints)
    {
        Missile* missile = gameObjMgr.createMissile(
                                 m_weaponTemplate->getMissileTemplate(),
                                 &robot, m_damage,
                                 fp.m_firePos[0], fp.m_firePos[1],
                                 fp.m_fireDirection[0], fp.m_fireDirection[1]);
        if (!missile)
        {
            LOG_ERROR("Failed to create missile");
            return false;
        }

        ReturnCode rc = map.checkCollision(missile);

        if (rc == RET_CODE_OUT_OF_SIGHT)
        {
            gameObjMgr.sendToDeathQueue(missile);
            continue;
        }
        else if (rc == RET_CODE_COLLIDE)
        {
            missile->explode(screen);
            continue;
        }

        map.addObject(missile);
    }

    return true;
}

} // end of namespace bot

