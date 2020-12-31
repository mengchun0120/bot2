#ifndef INCLUDE_BOT_WEAPON_TEMPLATE
#define INCLUDE_BOT_WEAPON_TEMPLATE

#include <vector>
#include "gameobj/bot_fire_point.h"
#include "gameobj/bot_single_unit_template.h"

namespace bot {

class WeaponTemplate: public SingleUnitTemplate {
public:
    WeaponTemplate();

    virtual ~WeaponTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    float getDamage() const
    {
        return m_damage;
    }

    int numFirePoints() const
    {
        return static_cast<int>(m_firePoints.size());
    }

    const FirePoint& getFirePoint(int idx) const
    {
        return m_firePoints[idx];
    }

protected:
    float m_damage;
    std::vector<FirePoint> m_firePoints;
};

} // end of namespace bot

#endif

