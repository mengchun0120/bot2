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

bool Shell::init(const ShellTemplate *t, Side side, float x, float y,
                 float directionX, float directionY, float damage, float speed)
{
    return Missile::init(t, side, x, y, directionX, directionY, damage, speed);
}

void Shell::present()
{
    const ShellTemplate *t = getTemplate();
    t->getRect()->draw(m_pos, m_direction, nullptr, nullptr,
                       *(t->getTexture()), nullptr);
}

void Shell::update(float delta, GameScreen &screen)
{
    float deltaX = getSpeedX() * delta;
    float deltaY = getSpeedY() * delta;

    shiftPos(deltaX, deltaY);
    checkCollision(screen);
}

bool Shell::onEntry(GameScreen &screen)
{
    if (checkCollision(screen))
    {
        screen.getMap().addObject(this);
        return true;
    }

    return false;
}

void Shell::onHit(GameScreen &screen, GameObject &obj)
{
    if (obj.getType() != GAME_OBJ_TYPE_ROBOT)
    {
        return;
    }

    const ShellTemplate *t = getTemplate();
    explode(screen, t->getExplodeBreath(), t->getImpactEffectTemplate());
}

void Shell::onDeath(GameScreen &screen)
{
    GameObjectManager &gameObjMgr = screen.getGameObjManager();
    gameObjMgr.sendToDeathQueue(this);
}

bool Shell::checkCollision(GameScreen &screen)
{
    GameMap &map = screen.getMap();

    ReturnCode rc = map.checkCollision(this, nullptr);

    if (RET_CODE_OUT_OF_SIGHT == rc)
    {
        onDeath(screen);
        return false;
    }

    if (RET_CODE_COLLIDE != rc)
    {
        return true;
    }

    const ShellTemplate *t = getTemplate();

    explode(screen, t->getExplodeBreath(), t->getImpactEffectTemplate());

    return false;
}

} // end of namespace bot

