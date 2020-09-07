#include "misc/bot_math_utils.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "gameobj/bot_tile.h"

namespace bot {

Tile::Tile(const TileTemplate* tileTemplate)
    : GameObject(tileTemplate)
    , m_hp(tileTemplate->getHP())
{
    m_flags = tileTemplate->getFlags();
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
}

Tile::~Tile()
{}

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

void Tile::setPos(float x, float y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

} // end of namespace bot
