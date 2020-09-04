#ifndef INLCUDE_BOT_MOVE_ABILITY_TEMPLATE
#define INLCUDE_BOT_MOVE_ABILITY_TEMPLATE

#include "gametemplate/bot_ability_template.h"

namespace bot {

class MoveAbilityTemplate : public AbilityTemplate {
public:
    MoveAbilityTemplate(float speed);

    virtual ~MoveAbilityTemplate();

    float getSpeed() const
    {
        return m_speed;
    }

protected:
    float m_speed;
};

} // end of namespace bot

#endif
