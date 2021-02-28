#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "gameobj/bot_tile_template.h"
#include "gameutil/bot_game_lib.h"
#include "gameobj/bot_game_object_flag.h"
#include "gameobj/bot_tile.h"
#include "screen/bot_game_screen.h"

namespace bot {

Tile::Tile(const TileTemplate *tileTemplate, int level,
           float x, float y) noexcept(false)
    : GameObject(tileTemplate, x, y)
    , m_maxHP(tileTemplate->getHP(level))
    , m_hp(m_maxHP)
{
    m_flags = tileTemplate->getFlags();
    m_mask.init(255, 255, 255, 255);
}

void Tile::present()
{
    const TileTemplate *t = static_cast<const TileTemplate*>(m_template);

    t->getRect()->draw(m_pos, nullptr, nullptr, nullptr,
                       t->getTexture()->textureId(), &m_mask);
}

void Tile::update(float delta, GameScreen &screen)
{
    if (!testFlag(GAME_OBJ_FLAG_DISSOLVE))
    {
        return;
    }

    const GameConfig &cfg = GameLib::getInstance().getGameConfig();
    float alpha = 1.0f - elapsedTimeMs(m_deathTime) / cfg.getDissolveTimeMS();

    if (alpha > 0.0f)
    {
        m_mask.setAlpha(alpha);
    }
    else
    {
        GameObjectManager &gameObjMgr = screen.getGameObjManager();
        gameObjMgr.sendToDeathQueue(this);
    }
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

void Tile::onDeath(GameScreen &screen)
{
    GameObjectManager &gameObjMgr = screen.getGameObjManager();

    gameObjMgr.sendToDissolveQueue(this);
    m_deathTime = Clock::now();
}

} // end of namespace bot

