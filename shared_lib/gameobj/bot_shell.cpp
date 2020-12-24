#include "gameobj/bot_shell.h"
#include "gameutil/bot_collide.h"
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
}

void Shell::onDeath(GameScreen& screen)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    gameObjMgr.sendToDeathQueue(this);
}

void Shell::explode(GameScreen& screen)
{
    const ShellTemplate* t = getTemplate();
    GameMap& map = gameScreen.getMap();
    GameObjectManager& gameObjMgr = gameScreen.getGameObjManager();

    ParticleEffect* explosion = gameObjMgr.createParticleEffect(
                                     t->getExplosionEffectTemplate(),
                                     getPosX(), getPosY());
    if (!map.addObject(explosion))
    {
        explosion->onDeath(screen);
    }

    int startRow, endRow, startCol, endCol;
    float left = getPosX() - t->getExplodeBreath();
    float right = getPosX() + t->getExplodeBreath();
    float bottom = getPosY() - t->getExplodeBreath();
    float top = getPosY() + t->getExplodeBreath();

    bool inBound = map.getRectCoords(startRow, endRow, startCol, endCol,
                                     left, bottom, right, top);
    if (!inBound)
    {
        onDeath(screen);
        return;
    }

    map.clearFlagsInRect(startRow, endRow, startCol, endCol,
                         GAME_OBJ_FLAG_EXPLODE_CHECKED);

    for (int r = startRow; r <= endRow; ++r)
    {
        for (int c = startCol; c <= endCol; ++c)
        {
            LinkedList<GameObjectItem>& cell = map.getMapCell(r, c);
            GameObjectItem* item = cell.getFirst();
            for (; item; item = item->getNext())
            {
                GameObject* obj = item->getObj();

                if (!checkExplosion(obj, left, bottom, right, top))
                {
                    obj->onDeath(gameScreen);
                }

                obj->setFlag(GAME_OBJ_FLAG_EXPLODE_CHECKED);
            }
        }
    }

    onDeath(screen);
}

bool Shell::checkExplosion(GameObject* obj, float left, float bottom,
                             float right, float top)
{
    static const int UNAFFECTED_FLAGS = GAME_OBJ_FLAG_EXPLODE_CHECKED |
                                        GAME_OBJ_FLAG_INDESTRUCTABLE |
                                        GAME_OBJ_FLAG_DISSOLVE |
                                        GAME_OBJ_FLAG_DEAD;

    bool dontCheck = obj == static_cast<GameObject*>(this) ||
                     obj->testFlag(UNAFFECTED_FLAGS) ||
                     (obj->getType() != GAME_OBJ_TYPE_ROBOT &&
                      obj->getType() != GAME_OBJ_TYPE_TILE);

    if (dontCheck)
    {
        return true;
    }

    bool touch = checkRectOverlapp(left, bottom, right, top,
                                   obj->getCollideLeft(),
                                   obj->getCollideBottom(),
                                   obj->getCollideRight(),
                                   obj->getCollideTop());
    if (!touch)
    {
        return true;
    }

    bool active = true;

    if (obj->getType() == GAME_OBJ_TYPE_ROBOT)
    {
        Robot* robot = static_cast<Robot*>(obj);
        if (robot->getSide() == m_side)
        {
            return true;
        }

        active = robot->addHP(-m_damage);
    }
    else
    {
        Tile* tile = static_cast<Tile*>(obj);
        active = tile->addHP(-m_damage);
    }

    return active;
}

} // end of namespace bot

