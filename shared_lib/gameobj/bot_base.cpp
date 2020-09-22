#include "misc/bot_math_utils.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_base_template.h"
#include "gameobj/bot_base.h"

namespace bot {

Base::Base()
    : m_baseTemplate(nullptr)
    , m_level(1)
    , m_hp(0.0f)
    , m_maxHP(0.0f)
    , m_hpRestoreRate(0.0f)
    , m_hpRatio(0.0f)
    , m_armor(0.0f)
    , m_maxArmor(0.0f)
    , m_armorRepairRate(0.0f)
    , m_power(0.0f)
    , m_maxPower(0.0f)
    , m_powerRestoreRate(0.0f)
{
}

bool Base::init(const BaseTemplate* t, int level, float x, float y, float directionX, float directionY)
{
    if (!t)
    {
        LOG_ERROR("BaseTemplate is null");
        return false;
    }

    if (level <= 0)
    {
        LOG_ERROR("Invalid level %d", level);
        return false;
    }

    m_baseTemplate = t;
    m_level = level;

    m_maxHP = t->getHP(level);
    m_hp = m_maxHP;
    m_hpRestoreRate = t->getHPRestoreRate(level);
    m_hpRatio = 1.0f;

    m_maxArmor = t->getArmor(level);
    m_armor = m_maxArmor;
    m_armorRepairRate = t->getArmorRepairRate(level);

    m_maxPower = t->getPower(level);
    m_power = m_maxPower;
    m_powerRestoreRate = t->getPowerRestoreRate(level);

    updateWeaponMoverPos(x, y, directionX, directionY);

    m_lastUpdateTime = Clock::now();

    return true;
}

void Base::update(GameScreen& screen)
{
    const BaseTemplate* t = getTemplate();
    TimePoint now = Clock::now();
    float timeDist = timeDistS(m_lastUpdateTime, now);

    float newHP = m_hp + timeDist * m_hpRestoreRate;
    setHP(newHP);

    float newArmor = m_armor + timeDist * m_armorRepairRate;
    setArmor(newArmor);

    float newPower = m_power + timeDist * m_powerRestoreRate;
    setPower(newPower);

    m_lastUpdateTime = now;
}

void Base::present(Graphics& g, const float* pos, const float* direction)
{
    t->getRect()->draw(g, pos, direction, nullptr, nullptr, t->getTexture()->textureId(), nullptr);
}

void Base::updateWeaponMoverPos(float x, float y, float directionX, float directionY)
{
    float dx = m_baseTemplate->getWeaponPosX();
    float dy = m_baseTemplate->getWeaponPosY();

    rotate(dx, dy, directionX, directionY);
    m_weaponPos[0] = x + dx;
    m_weaponPos[1] = y + dy;

    dx = m_baseTemplate->getMoverPosX();
    dy = m_baseTemplate->getMoverPosY();
    rotate(dx, dy, directionX, directionY);
    m_moverPos[0] = x + dx;
    m_moverPos[1] = y + dy;
}

} // end of namespace bot

