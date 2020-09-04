#include "gametemplate/bot_shoot_ability_template.h"

namespace bot {

ShootAbilityTemplate::ShootAbilityTemplate(float shootInterval, float shootPosX, float shootPosY,
                                           const MissileTemplate* missileTemplate)
    : AbilityTemplate(ABILITY_SHOOT)
    , m_shootInterval(shootInterval)
    , m_missileTemplate(missileTemplate)
{
    m_shootPos[0] = shootPosX;
    m_shootPos[1] = shootPosY;
}

ShootAbilityTemplate::~ShootAbilityTemplate()
{
}

} // end of namespace bot
