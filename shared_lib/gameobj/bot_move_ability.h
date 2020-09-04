#ifndef INCLUDE_BOT_MOVE_ABILITY
#define INCLUDE_BOT_MOVE_ABILITY

#include "gametemplate/bot_move_ability_template.h"
#include "gameobj/bot_ability.h"

namespace bot {

class MoveAbility : public Ability {
public:
    MoveAbility(const MoveAbilityTemplate* t)
        : Ability(t)
        , m_moving(false)
        , m_speedMultiplier(1.0f)
    {
    }

    virtual ~MoveAbility()
    {}

    float getSpeed() const
    {
        return static_cast<const MoveAbilityTemplate*>(m_template)->getSpeed() * m_speedMultiplier;
    }

    bool isMoving() const
    {
        return m_moving;
    }

    void setMoving(bool moving)
    {
        m_moving = moving;
    }

    float getSpeedMultiplier() const
    {
        return m_speedMultiplier;
    }

    void setSpeedMultiplier(float multiplier)
    {
        m_speedMultiplier = multiplier;
    }

protected:
    bool m_moving;
    float m_speedMultiplier;
};

} // end of namespace bot

#endif
