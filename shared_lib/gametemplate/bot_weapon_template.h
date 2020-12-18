#ifndef INCLUDE_BOT_WEAPON_TEMPLATE
#define INCLUDE_BOT_WEAPON_TEMPLATE

#include <string>
#include <vector>
#include "gameobj/bot_fire_point.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class MissileTemplate;

class WeaponTemplate: public SingleUnitTemplate {
public:
    WeaponTemplate();

    virtual ~WeaponTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    bool init(const MissileTemplate* missileTemplate,
              const rapidjson::Value& elem);

    float getFireDuration(int level) const;

    bool setFireDuration(float duration);

    bool setFireDurReductionPerLevel(float reductionPerLevel);

    const MissileTemplate* getMissileTemplate() const
    {
        return m_missileTemplate;
    }

    void setMissileTemplate(const MissileTemplate* missileTemplate)
    {
        m_missileTemplate = missileTemplate;
    }

    int numFirePoints() const
    {
        return static_cast<int>(m_firePoints.size());
    }

    const FirePoint& getFirePoint(int idx) const
    {
        return m_firePoints[idx];
    }

    bool setNumFirePoints(int count);

    FirePoint& getFirePoint(int idx)
    {
        return m_firePoints[idx];
    }

protected:
    float m_fireDuration;
    float m_fireDurReductionPerLevel;
    const MissileTemplate* m_missileTemplate;
    std::vector<FirePoint> m_firePoints;
};

} // end of namespace bot

#endif

