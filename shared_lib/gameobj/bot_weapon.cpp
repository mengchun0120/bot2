#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "opengl/bot_texture.h"
#include "gameobj/bot_weapon_template.h"
#include "gameobj/bot_missile_template.h"
#include "gameobj/bot_robot.h"
#include "screen/bot_game_screen.h"

namespace bot {

Weapon::Weapon()
    : m_weaponTemplate(nullptr)
    , m_robot(nullptr)
    , m_damageMultiplier(1.0f)
{
}

bool Weapon::init(const WeaponTemplate *weaponTemplate, Robot *robot,
                  int weaponLevel)
{
    if (!weaponTemplate)
    {
        LOG_ERROR("Weapon template is null");
        return false;
    }

    if (!robot)
    {
        LOG_ERROR("robot is null");
        return false;
    }

    if (weaponLevel < 0)
    {
        LOG_ERROR("Invalid weapon-level %d", weaponLevel);
        return false;
    }

    m_weaponTemplate = weaponTemplate;
    m_robot = robot;

    m_firePoints.resize(m_weaponTemplate->numFirePoints());
    resetFirePoints();

    m_damageMultiplier = 1.0f;

    return true;
}

bool Weapon::update(GameScreen &screen)
{
    return true;
}

void Weapon::present()
{
    const Base &base = m_robot->getBase();
    m_weaponTemplate->getRect()->draw(
                                base.getWeaponPos(), m_robot->getDirection(),
                                nullptr, nullptr,
                                m_weaponTemplate->getTexture()->textureId(),
                                &(m_robot->getMask()));
}

void Weapon::shiftFirePoints(float deltaX, float deltaY)
{
    for (auto &fp: m_firePoints)
    {
        fp.m_firePos[0] += deltaX;
        fp.m_firePos[1] += deltaY;
    }
}

void Weapon::resetFirePoints()
{
    int count = static_cast<int>(m_firePoints.size());
    const Base &base = m_robot->getBase();
    float weaponX = base.getWeaponPosX();
    float weaponY = base.getWeaponPosY();
    float directionX = m_robot->getDirectionX();
    float directionY = m_robot->getDirectionY();

    for (int i = 0; i < count; ++i)
    {
        FirePoint &p1 = m_firePoints[i];
        const FirePoint &p2 = m_weaponTemplate->getFirePoint(i);
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

bool Weapon::setDamageMultiplier(float multiplier)
{
    if (multiplier < 0.0f)
    {
        LOG_ERROR("Invalid multiplier %f", multiplier);
        return false;
    }

    m_damageMultiplier = multiplier;

    return true;
}

bool Weapon::fireMissile(GameScreen &screen,
                         const MissileTemplate *missileTemplate,
                         float damageMultiplier, float speedMultiplier)
{
    GameObjectManager &gameObjMgr = screen.getGameObjManager();
    float damage = m_weaponTemplate->getDamage() * damageMultiplier *
                   m_damageMultiplier;
    float speed = missileTemplate->getSpeed() * speedMultiplier;

    for(auto &fp: m_firePoints)
    {
        Missile *missile = gameObjMgr.createMissile(
                                 missileTemplate, m_robot->getSide(),
                                 fp.m_firePos[0], fp.m_firePos[1],
                                 fp.m_fireDirection[0], fp.m_fireDirection[1],
                                 damage, speed);

        if (!missile)
        {
            LOG_ERROR("Failed to create missile");
            return false;
        }

        missile->onEntry(screen);
    }

    return true;
}

} // end of namespace bot

