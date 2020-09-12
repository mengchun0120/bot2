#include "misc/bot_math_utils.h"
#include "gametemplate/bot_base_component_template.h"
#include "gameobj/bot_base_component.h"

namespace bot {

BaseComponent::BaseComponent(const BaseComponentTemplate* t)
    : Component(t)
    , m_hp(t->getHP())
    , m_hpRatio(1.0f)
    , m_armor(t->getArmor())
    , m_power(t->getPower())
{
    m_lastUpdateTime = Clock::now();
}

void BaseComponent::update(GameScreen& screen)
{
    const BaseComponentTemplate* t = getTemplate();
    TimePoint now = Clock::now();
    float timeDist = timeDistS(m_lastUpdateTime, now);

    float hpDelta = timeDist * t->getHPRestoreRate();
    addHP(hpDelta);

    float armorDelta = timeDist * t->getArmorRepairRate();
    addArmor(armorDelta);

    float powerDelta = timeDist * t->getPowerRestoreRate();
    addPower(powerDelta);

    m_lastUpdateTime = now;
}

void BaseComponent::addHP(float delta)
{
    m_hp = clamp(m_hp + delta, 0.0f, getTemplate()->getHP());
    m_hpRatio = m_hp / getTemplate()->getHP();
}

void BaseComponent::addArmor(float delta)
{
    m_armor = clamp(m_armor + delta, 0.0f, getTemplate()->getArmor());
}

void BaseComponent::addPower(float delta)
{
    m_power = clamp(m_power + delta, 0.0f, getTemplate()->getPower());
}

} // end of namespace bot

