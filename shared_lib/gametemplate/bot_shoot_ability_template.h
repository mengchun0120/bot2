#ifndef INCLUDE_BOT_SHOOT_ABILITY_TEMPLATE
#define INCLUDE_BOT_SHOOT_ABILITY_TEMPLATE

#include "misc/bot_constants.h"
#include "gametemplate/bot_ability_template.h"

namespace bot {

class MissileTemplate;

class ShootAbilityTemplate : public AbilityTemplate {
public:
    ShootAbilityTemplate(float shootInterval, float shootPosX, float shootPosY,
                         const MissileTemplate* missileTemplate);

    virtual ~ShootAbilityTemplate();

    float getShootInterval() const
    {
        return m_shootInterval;
    }

    float getShootPosX() const
    {
        return m_shootPos[0];
    }

    const float* getShootPos() const
    {
        return m_shootPos;
    }

    float getShootPosY() const
    {
        return m_shootPos[1];
    }

    const MissileTemplate* getMissileTemplate() const
    {
        return m_missileTemplate;
    }

protected:
    float m_shootInterval;
    float m_shootPos[Constants::NUM_FLOATS_PER_POSITION];
    const MissileTemplate* m_missileTemplate;
};

} // end of namespace bot

#endif
