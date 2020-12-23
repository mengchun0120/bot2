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
                 float directionX, float directionY, float damage)
{
    return Missile::init(t, side, x, y, directionX, directionY,
                         damage, t->getSpeed());
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

    LinkedList<GameObjectItem> collideObjs;
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    GameMap& map = screen.getMap();

    ReturnCode rc = map.checkCollision(this, &collideObjs);

    if (RET_CODE_OUT_OF_SIGHT == rc)
    {
        onDeath(screen);
        return;
    }

    if (collideObjs.isEmpty())
    {
        return;
    }

    explode(screen);
    gameObjMgr.freeGameObjectItems(collideObjs);

    onDeath(screen);
}

void Shell::onDeath(GameScreen& screen)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    gameObjMgr.sendToDeathQueue(this);
}

void Shell::explode(GameScreen& screen)
{

}

} // end of namespace bot

