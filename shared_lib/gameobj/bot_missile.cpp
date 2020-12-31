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
    : GameObject()
    , m_side(SIDE_UNKNOWN)
    , m_damage(0.0f)
    , m_speed(0.0f)
{
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
}

bool Missile::init(const MissileTemplate* t, Side side, float x, float y,
                   float directionX, float directionY,
                   float damage, float speed)
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

    if (speed <= 0.0f)
    {
        LOG_ERROR("Invalid speed %f", speed);
        return false;
    }

    setDirection(directionX, directionY);
    m_side = side;
    m_damage = damage;
    m_speed = speed;

    return true;
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

void Missile::explode(GameScreen& screen, float explodeBreath,
                      const ParticleEffectTemplate* explosionTemplate)
{
    GameMap& map = screen.getMap();
    GameObjectManager& gameObjMgr = screen.getGameObjManager();

    if (explosionTemplate)
    {
        ParticleEffect* explosion = gameObjMgr.createParticleEffect(
                                                    explosionTemplate,
                                                    getPosX(), getPosY());
        if (!map.addObject(explosion))
        {
            explosion->onDeath(screen);
        }
    }

    int startRow, endRow, startCol, endCol;
    float left = getPosX() - explodeBreath;
    float right = getPosX() + explodeBreath;
    float bottom = getPosY() - explodeBreath;
    float top = getPosY() + explodeBreath;

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
            GameObjectItem* item;

            for (item = cell.getFirst(); item; item = item->getNext())
            {
                GameObject* obj = item->getObj();

                if (!checkExplosion(obj, left, bottom, right, top))
                {
                    obj->onDeath(screen);
                }

                obj->setFlag(GAME_OBJ_FLAG_EXPLODE_CHECKED);
            }
        }
    }

    onDeath(screen);
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

