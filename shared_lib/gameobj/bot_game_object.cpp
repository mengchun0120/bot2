#include "gameobj/bot_game_object.h"

namespace bot {

GameObject::GameObject(const GameObjectTemplate* t)
    : DoubleLinkedItem()
    , m_template(t)
    , m_coverStartRow(0)
    , m_coverEndRow(0)
    , m_coverStartCol(0)
    , m_coverEndCol(0)
    , m_flags(0)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
}

} // end of namespace bot
