#include "misc/bot_log.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_collide.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_missile.h"
#include "screen/bot_game_screen.h"

namespace bot {

Missile::Missile()
    : m_side(SIDE_UNKNOWN)
    , m_damage(0.0f)
{
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
}

bool Missile::init(const MissileTemplate* t, Side side, float damage,
                   float x, float y, float directionX, float directionY)
{
    if (!GameObject::init(t, x, y))
    {
        return false;
    }

    if (!isValid(side))
    {
        LOG_ERROR("Invalid side %d", static_cast<int>(side));
        return false;
    }

    if (damage <= 0.0f)
    {
        LOG_ERROR("Invalid damage %f", damage);
        return false;
    }

    setDirection(directionX, directionY);
    m_damage = damage;
    m_side = side;

    return true;
}

void Missile::present()
{
    const MissileTemplate* t = static_cast<const MissileTemplate*>(m_template);
    t->getRect()->draw(m_pos, m_direction, nullptr, nullptr,
                       t->getTexture()->textureId(), t->getColor());
}

void Missile::update(float delta, GameScreen& screen)
{
    const MissileTemplate* t = getTemplate();
    GameMap& map = screen.getMap();
    GameObjectManager& gameObjManager = screen.getGameObjManager();

    float speedX = t->getSpeed() * m_direction[0];
    float speedY = t->getSpeed() * m_direction[1];
    float deltaX = speedX * delta;
    float deltaY = speedY * delta;

    shiftPos(deltaX, deltaY);

    ReturnCode rc = map.checkCollision(this);

    if (RET_CODE_OUT_OF_SIGHT == rc)
    {
        gameObjManager.sendToDeathQueue(this);
        return;
    }

    if (RET_CODE_COLLIDE == rc)
    {
        explode(screen);
        return;
    }

    map.repositionObject(this);
}

void Missile::shiftPos(float deltaX, float deltaY)
{
    m_pos[0] += deltaX;
    m_pos[1] += deltaY;
}

void Missile::setPos(float x, float y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void Missile::setDirection(float directionX, float directionY)
{
    m_direction[0] = directionX;
    m_direction[1] = directionY;
}

void Missile::explode(GameScreen& gameScreen)
{
    const MissileTemplate* t = getTemplate();
    GameMap& map = gameScreen.getMap();
    GameObjectManager& gameObjMgr = gameScreen.getGameObjManager();

    ParticleEffect* explosion = gameObjMgr.createParticleEffect(
                                                t->getExplosionTemplate(),
                                                getPosX(), getPosY());
    if (!map.addObject(explosion))
    {
        gameObjMgr.sendToDeathQueue(explosion);
    }

    int startRow, endRow, startCol, endCol;
    float left = m_pos[0] - t->getExplosionBreath();
    float right = m_pos[0] + t->getExplosionBreath();
    float bottom = m_pos[1] - t->getExplosionBreath();
    float top = m_pos[1] + t->getExplosionBreath();

    bool inBound = map.getRectCoords(startRow, endRow, startCol, endCol,
                                     left, bottom, right, top);
    if (!inBound)
    {
        gameObjMgr.sendToDeathQueue(this);
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
            while (item)
            {
                GameObject* obj = item->getObj();

                if (!checkExplosion(obj, left, bottom, right, top))
                {
                    obj->onDeath(gameScreen);
                }

                obj->setFlag(GAME_OBJ_FLAG_EXPLODE_CHECKED);

                item = static_cast<GameObjectItem*>(item->getNext());
            }
        }
    }

    gameObjMgr.sendToDeathQueue(this);
}

bool Missile::checkExplosion(GameObject* obj, float left, float bottom,
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

