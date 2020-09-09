#ifndef INCLUDE_BOT_WEAPON_COMPONENT_TEMPLATE
#define INCLUDE_BOT_WEAPON_COMPONENT_TEMPLATE

#include <vector>
#include "gametemplate/bot_component_template.h"

namespace bot {

class MissileTemplate;

struct FirePoint {
    std::vector<float> m_firePos;
    std::vector<float> m_fireDirection;
};

class WeaponComponentTemplate: public ComponentTemplate {
public:
    WeaponComponentTemplate()
        : ComponentTemplate(COMPONENT_WEAPON)
        , m_fireDuration(0.0f)
        , m_firePower(0.0f)
    {}

    virtual ~WeaponComponentTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem);

    float getFireDuration() const
    {
        return m_fireDuration;
    }

    float getFirePower() const
    {
        return m_firePower;
    }

    const std::vector<const MissileTemplate*>& getMissileTemplate() const
    {
        return m_missiles;
    }

    int numFirePoints() const
    {
        return static_cast<int>(m_firePoints.size());
    }

    const FirePoint& getFirePoint(int idx) const
    {
        return m_firePoints[idx];
    }

private:
    bool initMissiles(const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem);

    bool initFirePoints(const rapidjson::Value& elem);

protected:
    float m_fireDuration;
    float m_firePower;
    std::vector<const MissileTemplate*> m_missiles;
    std::vector<FirePoint> m_firePoints;
};

} // end of namespace bot

#endif

