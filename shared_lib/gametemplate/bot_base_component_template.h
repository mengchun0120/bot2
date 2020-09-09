#ifndef INCLUDE_BOT_BASE_COMPONENT_TEMPLATE
#define INCLUDE_BOT_BASE_COMPONENT_TEMPLATE

#include <vector>
#include "gametemplate/bot_component_template.h"

namespace bot {

class BaseComponentTemplate: public ComponentTemplate {
public:
    BaseComponentTemplate()
        : ComponentTemplate(COMPONENT_BASE)
        , m_hp(0)
        , m_armor(0)
        , m_power(0)
    {}

    virtual ~BaseComponentTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    int getHP() const
    {
        return m_hp;
    }

    int getArmor() const
    {
        return m_armor;
    }

    int getPower() const
    {
        return m_power;
    }

    const float* getWeaponPos() const
    {
        return m_weaponPos.data();
    }

    const float* getMoverPos() const
    {
        return m_moverPos.data();
    }

protected:
    int m_hp;
    int m_armor;
    int m_power;
    std::vector<float> m_weaponPos;
    std::vector<float> m_moverPos;
};

} // end of namespace bot

#endif

