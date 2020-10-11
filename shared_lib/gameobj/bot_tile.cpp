#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_graphics.h"
#include "gametemplate/bot_tile_template.h"
#include "gameobj/bot_game_object_flag.h"
#include "gameobj/bot_tile.h"

namespace bot {

Tile::Tile()
    : m_hp(0.0f)
    , m_maxHP(0.0f)
{
}

bool Tile::init(const TileTemplate* tileTemplate, int level, float x, float y)
{
    if (!GameObject::init(tileTemplate, x, y))
    {
        return false;
    }

    if (level < 0)
    {
        LOG_ERROR("Invalid tile level %d", level);
        return false;
    }

    m_maxHP = tileTemplate->getHP(level);
    m_hp = m_maxHP;
    m_flags = tileTemplate->getFlags();

    return true;
}

void Tile::present(Graphics& g)
{
    const TileTemplate* t = static_cast<const TileTemplate*>(m_template);

    t->getRect()->draw(g, m_pos, nullptr, nullptr, nullptr,
                       t->getTexture()->textureId(), nullptr);
}

void Tile::update(float delta, GameScreen& screen)
{
}

bool Tile::addHP(float deltaHP)
{
    if (testFlag(GAME_OBJ_FLAG_INDESTRUCTABLE))
    {
        return true;
    }

    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return false;
    }

    m_hp = clamp(m_hp + deltaHP, 0.0f, m_maxHP);

    return m_hp > 0;
}

} // end of namespace bot

