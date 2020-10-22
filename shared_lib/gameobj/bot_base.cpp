#include <cstdio>
#include <cmath>
#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "opengl/bot_graphics.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_base_template.h"
#include "gameobj/bot_base.h"
#include "screen/bot_game_screen.h"

namespace bot {

Base::Base()
    : m_baseTemplate(nullptr)
    , m_hp(0.0f)
    , m_maxHP(0.0f)
    , m_hpRestoreRate(0.0f)
    , m_hpPercent(0.0f)
    , m_armor(0.0f)
    , m_maxArmor(0.0f)
    , m_armorRepairRate(0.0f)
    , m_power(0.0f)
    , m_maxPower(0.0f)
    , m_powerRestoreRate(0.0f)
{
}

bool Base::init(const BaseTemplate* t, int hpLevel, int hpRestoreLevel,
                int armorLevel, int armorRepairLevel,
                int powerLevel, int powerRestoreLevel,
                float x, float y, float directionX, float directionY)
{
    if (!t)
    {
        LOG_ERROR("BaseTemplate is null");
        return false;
    }

    if (hpLevel < 0)
    {
        LOG_ERROR("Invalid hp-level %d", hpLevel);
        return false;
    }

    if (hpRestoreLevel < 0)
    {
        LOG_ERROR("Invalid hp-retore-level %d", hpRestoreLevel);
        return false;
    }

    if (armorLevel < 0)
    {
        LOG_ERROR("Invalid armor-level %d", armorLevel);
        return false;
    }

    if (armorRepairLevel < 0)
    {
        LOG_ERROR("Invalid armor-repair-level %d", armorRepairLevel);
        return false;
    }

    if (powerLevel < 0)
    {
        LOG_ERROR("Invalid power-level %d", powerLevel);
        return false;
    }

    if (powerRestoreLevel < 0)
    {
        LOG_ERROR("Invalid power-restore-level %d", powerRestoreLevel);
        return false;
    }

    m_baseTemplate = t;

    m_maxHP = t->getHP(hpLevel);
    setHP(m_maxHP);
    m_hpRestoreRate = t->getHPRestoreRate(hpRestoreLevel);

    m_maxArmor = t->getArmor(armorLevel);
    m_armor = m_maxArmor;
    m_armorRepairRate = t->getArmorRepairRate(armorRepairLevel);

    m_maxPower = t->getPower(powerLevel);
    m_power = m_maxPower;
    m_powerRestoreRate = t->getPowerRestoreRate(powerRestoreLevel);

    setWeaponMoverPos(x, y, directionX, directionY);

    m_lastUpdateTime = Clock::now();

    return true;
}

void Base::update(GameScreen& screen)
{
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
    m_baseTemplate->getRect()->draw(g, pos, direction, nullptr, nullptr,
                                    m_baseTemplate->getTexture()->textureId(),
                                    nullptr);

    TextSystem& textSys = g.getTextSystem();
    float w, h;
    float p[Constants::NUM_FLOATS_PER_POSITION];

    textSys.getStringSize(w, h, TEXT_SIZE_TINY, m_hpPercentStr);
    p[0] = pos[0] - w / 2.0f;
    p[1] = pos[1] - h / 2.0f;

    SimpleShaderProgram& shader = g.getSimpleShader();

    shader.setUseDirection(false);
    textSys.drawString(shader, m_hpPercentStr, TEXT_SIZE_TINY,
                       p, m_baseTemplate->getHPColor()->getColor());
}

void Base::setHP(float hp)
{
    m_hp = clamp(hp, 0.0f, m_maxHP);
    m_hpPercent = static_cast<int>(ceil(m_hp / m_maxHP * 100.0f));
    resetHPPercentStr();
}

void Base::refillHP()
{
    m_hp = m_maxHP;
    m_hpPercent = 100.0f;
    resetHPPercentStr();
}

void Base::shiftWeaponMoverPos(float deltaX, float deltaY)
{
    m_weaponPos[0] += deltaX;
    m_weaponPos[1] += deltaY;
    m_moverPos[0] += deltaX;
    m_moverPos[1] += deltaY;
}

void Base::setWeaponMoverPos(float x, float y,
                             float directionX, float directionY)
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

void Base::resetHPPercentStr()
{
    snprintf(m_hpPercentStr, sizeof(m_hpPercentStr), "%d%%", m_hpPercent);
}

} // end of namespace bot

