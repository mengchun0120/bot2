#ifndef INCLUDE_BOT_BASE_COMPONENT_TEMPLATE
#define INCLUDE_BOT_BASE_COMPONENT_TEMPLATE

#include <vector>
#include "gametemplate/bot_component_template.h"

namespace bot {

class BaseComponentTemplate: public ComponentTemplate {
public:
    BaseComponentTemplate()
        : ComponentTemplate(COMPONENT_BASE)
        , m_hp(0.0f)
        , m_hpRestoreRate(0.0f)
        , m_armor(0.0f)
        , m_armorRepairRate(0.0f)
        , m_power(0.0f)
        , m_powerRestoreRate(0.0f)
        , m_missileCapacity(0)
    {}

    virtual ~BaseComponentTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    float getHP() const
    {
        return m_hp;
    }

    float getHPRestoreRate() const
    {
        return m_hpRestoreRate;
    }

    float getArmor() const
    {
        return m_armor;
    }

    float getArmorRepairRate() const
    {
        return m_armorRepairRate;
    }

    float getPower() const
    {
        return m_power;
    }

    float getPowerRestoreRate() const
    {
        return m_powerRestoreRate;
    }

    int getMissileCapacity() const
    {
        return m_missileCapacity;
    }

protected:
    float m_hp;
    float m_hpRestoreRate;
    float m_armor;
    float m_armorRepairRate;
    float m_power;
    float m_powerRestoreRate;
    int m_missileCapacity;
};

} // end of namespace bot

#endif

