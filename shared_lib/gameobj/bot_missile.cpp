#include "misc/bot_log.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_collide.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_missile.h"
#include "screen/bot_game_screen.h"
#include "screen/bot_screen_manager.h"

namespace bot {

Missile::Missile()
    : m_side(SIDE_UNKNOWN)
    , m_damage(0.0f)
    , m_ability(MISSILE_ABILITY_NONE)
{
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
}

Missile::~Missile()
{
    onDealloc();
}


bool Missile::init(const MissileTemplate* t, Side side, float damage,
                   float x, float y, float directionX, float directionY,
                   MissileAbility ability)
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

    if (!isValidMissileAbility(ability))
    {
        LOG_ERROR("Invalid ability %d", static_cast<int>(ability));
        return false;
    }

    setDirection(directionX, directionY);
    m_damage = damage;
    m_side = side;
    m_ability = ability;

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

    switch(m_ability)
    {
        case MISSILE_ABILITY_NONE:
        {
            ReturnCode rc = map.checkCollision(this, nullptr);

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

            break;
        }
        case MISSILE_ABILITY_PENETRATE:
        {
            LinkedList<GameObjectItem> collideObjs;

            ReturnCode rc = map.checkCollision(this, &collideObjs);

            if (RET_CODE_OUT_OF_SIGHT == rc)
            {
                gameObjManager.sendToDeathQueue(this);
                return;
            }

            if (!colldeObjs.isEmpty())
            {
                bool stop = processPenetrate(collideObjs, screen)

                if (!collideObjs.isEmpty())
                {
                    gameObjManager.freeGameObjItems(collideObjs);
                }

                if (stop)
                {
                    gameObjManager.sendToDeathQueue(this);
                    return;
                }
            }

            break;
        }
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

void onDealloc()
{
    if (m_penetrateObjs.isEmpty())
    {
        return;
    }

    GameScreen* screen = static_cast<GameScreen*>(
                                screenManager::getInstance().getCurScreen());
    screen->getGameObjManager().freeGameObjItems(m_penetrateObjs);
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

bool Missile::processPenetrateObjs(LinkedList<GameObjectItem>& collideObjs,
                                   GameScreen& screen)
{
    GameObjectItem* prev = nullptr, * next, * cur;
    GameObjectManager& gameObjMgr = screen.getGameObjManager();

    for (cur = m_penetrateObjs.getFirst(); cur; cur = next)
    {
        next = cur->getNext();

        // If cur is not in collideObjs, remove it from penetrateObjs
        if (!collideObjs.find(GameObjetItem::match, cur))
        {
            m_penetrateObjs.unlink(prev, cur);
            gameObjMgr.freeGameObjItem(cur);
        }
        else
        {
            prev = cur;
        }
    }

    bool stop = false;

    prev = nullptr;
    for (cur = collideObjs.getFirst(); cur; cur = next)
    {
        next = cur->getNext();

        // If cur has been penetrated before, don't penetrate it again
        if (m_penetrateObjs.find(GameObject::match, cur))
        {
            prev = cur;
            continue;
        }

        GameObject* o = cur->getObj();
        bool alive = true;

        if (o->testFlag(GAME_FLAG_INDESTRUCTABLE))
        {
            stop = true;
        }
        else if (o->getType() == GAME_OBJ_TYPE_ROBOT)
        {
            Robot* robot = static_cast<Robot*>(o);
            alive = robot->addHP(-m_damage);
        }
        else if (o->getType() == GAME_OBJ_TYPE_TILE)
        {
            Tile* tile = static_cast<Tile*>(o);
            alive = tile->addHP(-m_damage);
        }

        if (!alive)
        {
            o->onDeath();
            prev = cur;
        }
        else
        {
            m_collideObjs.unlink(prev, cur);
            m_penetrateObjs.add(cur);
        }
    }

    return stop;
}

} // end of namespace bot

