#include "gametemplate/bot_game_object_template.h"

namespace bot {

GameObjectTemplate::GameObjectTemplate(GameObjectType type)
    : m_type(type)
    , m_coverBreathX(0.0f)
    , m_coverBreathY(0.0f)
    , m_collideBreathX(0.0f)
    , m_collideBreathY(0.0f)
    , m_flags(0)
{}

} // end of namespace bot