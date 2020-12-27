#include "misc/bot_log.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameobj/bot_shell.h"
#include "screen/bot_game_screen.h"

namespace bot {

Shell::Shell()
    : Missile()
{
}

Shell::~Shell()
{
}

bool Shell::init(const ShellTemplate* t, Side side, float x, float y,
                 float directionX, float directionY, float damage, float speed)
{
    return Missile::init(t, side, x, y, directionX, directionY, damage, speed);
}

void Shell::present()
{
    const BulletTemplate* t = getTemplate();
    t->getRect()->draw(m_pos, m_direction, nullptr, nullptr,
                       t->getTexture(), nullptr);
}

void Shell::update(float delta, GameScreen& screen)
{
    float deltaX = getSpeedX() * delta;
    float deltaY = getSpeedY() * delta;

    shiftPos(deltaX, deltaY);

    GameMap& map = screen.getMap();

    ReturnCode rc = map.checkCollision(this, nullptr);

    if (RET_CODE_OUT_OF_SIGHT == rc)
    {
        onDeath(screen);
        return;
    }

    if (RET_COLLIDE == rc)
    {
        return;
    }

    const ShellTemplate* t = getTemplate();

    explode(screen, t->getExplodeBreath(), t->getExplodeEffectTemplate());
}

void Shell::onDeath(GameScreen& screen)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    gameObjMgr.sendToDeathQueue(this);
}

} // end of namespace bot

