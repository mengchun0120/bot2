#include "misc/bot_log.h"
#include "gameobj/bot_game_object.h"

namespace bot {

GameObject::GameObject()
    : m_template(nullptr)
    , m_coverStartRow(0)
    , m_coverEndRow(0)
    , m_coverStartCol(0)
    , m_coverEndCol(0)
    , m_flags(0)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
}

bool GameObject::init(const GameObjectTemplate *t, float x, float y)
{
    if (!t)
    {
        LOG_ERROR("GameObjectTemplate is null");
        return false;
    }

    m_template = t;
    m_pos[0] = x;
    m_pos[1] = y;
    m_coverStartRow = 0;
    m_coverEndRow = 0;
    m_coverStartCol = 0;
    m_coverEndCol = 0;
    m_flags = 0;

    return true;
}

} // end of namespace bot
