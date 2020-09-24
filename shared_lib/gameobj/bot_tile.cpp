#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_graphics.h"
#include "gametemplate/bot_tile_template.h"
#include "gameobj/bot_tile.h"

namespace bot {

Tile::Tile()
    : m_hp(0.0f)
    , m_level(1)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
}

bool Tile::init(const TileTemplate* tileTemplate, int level, float x, float y)
{
    if (!GameObject::init(tileTemplate, x, y))
    {
        return false;
    }

    if (level < 1)
    {
        LOG_ERROR("Invalid tile level %d", level);
        return false;
    }

    m_level = level;
    m_hp = tileTemplate->getHP(level);
    m_flags = tileTemplate->getFlags();

    return true;
}

void Tile::present(Graphics& g)
{
    const TileTemplate* t = static_cast<const TileTemplate*>(m_template);
    const Rectangle* rect = t->getRect();
    const Texture* texture = t->getTexture();

    rect->draw(g, m_pos, nullptr, nullptr, nullptr, texture->textureId(), nullptr);
}

void Tile::update(float delta, GameScreen& screen)
{
}

bool Tile::addHP(int deltaHP)
{
    if (testFlag(GAME_OBJ_FLAG_INDESTRUCTABLE))
    {
        return true;
    }

    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return false;
    }

    m_hp = clamp(m_hp + deltaHP, 0, getTemplate()->getHP());

    return m_hp > 0;
}

} // end of namespace bot

