#ifndef INCLUDE_BOT_WEAPON_COMPONENT_TEMPLATE
#define INCLUDE_BOT_WEAPON_COMPONENT_TEMPLATE

#include <vector>
#include "gametemplate/bot_component_template.h"

namespace bot {

class MissileTemplate;

class WeaponComponentTemplate: public ComponentTemplate {
public:
    WeaponComponentTemplate()
        : ComponentTemplate(COMPONENT_WEAPON)
        , m_damage(0)
        , m_missileTemplate(nullptr)
    {}

    virtual ~WeaponComponentTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem);

    int getDamage() const
    {
        return m_damage;
    }

    const MissileTemplate* getMissileTemplate() const
    {
        return m_missileTemplate;
    }

    const float* getFirePos() const
    {
        return m_firePos.data();
    }

protected:
    int m_damage;
    const MissileTemplate* m_missileTemplate;
    std::vector<float> m_firePos;
};

} // end of namespace bot

#endif

