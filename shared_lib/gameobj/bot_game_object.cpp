#include <stdexcept>
#include "misc/bot_log.h"
#include "gameobj/bot_game_object.h"

namespace bot {

GameObject::GameObject(const GameObjectTemplate *t,
                       float x, float y) noexcept(false)
    : m_template(t)
    , m_coverStartRow(0)
    , m_coverEndRow(0)
    , m_coverStartCol(0)
    , m_coverEndCol(0)
    , m_flags(0)
{
    if (!t)
    {
        LOG_ERROR("GameObjectTemplate is null");
        throw std::runtime_error("GameObjectTemplate is null");
    }

    m_pos[0] = x;
    m_pos[1] = y;
}

bool GameObject::init(const GameObjectTemplate *t, float x, float y)
{
    if (!t)
    {
        LOG_ERROR("GameObjectTemplate is null");
        return false;
    }

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
