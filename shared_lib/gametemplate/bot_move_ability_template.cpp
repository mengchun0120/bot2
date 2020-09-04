#include "gametemplate/bot_move_ability_template.h"

namespace bot {

MoveAbilityTemplate::MoveAbilityTemplate(float speed)
    : AbilityTemplate(ABILITY_MOVE)
    , m_speed(speed)
{}

MoveAbilityTemplate::~MoveAbilityTemplate()
{}

} // end of namespace bot
