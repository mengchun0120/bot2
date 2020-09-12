#ifndef INCLUDE_BOT_WEAPON_COMPONENT
#define INCLUDE_BOT_WEAPON_COMPONENT

#include "misc/bot_time_utils.h"
#include "gametemplate/bot_weapon_component_template.h"
#include "gameobj/bot_component.h"

namespace bot {

class WeaponComponent: public Component {
public:
    WeaponComponent(const WeaponComponentTemplate* weaponTemplate, const MissileTemplate* missileTemplate);

    virtual ~WeaponComponent()
    {}

    const WeaponComponentTemplate* getTemplate() const
    {
        return static_cast<const WeaponComponentTemplate*>(m_template);
    }

    virtual void update(GameScreen& screen);

    bool isFiring() const
    {
        return m_firing;
    }

    void setFiring(bool enabled)
    {
        m_firing = enabled;
    }

    void shiftFirePoints(float deltaX, float deltaY);

    void setFirePoints(float weaponX, float weaponY, float directionX, float directionY);

    float getFireDuration() const
    {
        return getTemplate()->getFireDuration() * m_fireDurationMultiplier;
    }

    void setFireDurationMultiplier(float multiplier)
    {
        m_fireDurationMultiplier = multiplier;
    }

    void setDamageMultiplier(float multiplier)
    {
        m_damageMultipler = multiplier;
    }

protected:
    const MissileTemplate* m_missileTemplate;
    bool m_firing;
    std::vector<FirePoint> m_firePoints;
    TimePoint m_lastFireTime;
    float m_fireDurationMultiplier;
    float m_damageMultipler;
};

} // end of namespace bot

#endif

