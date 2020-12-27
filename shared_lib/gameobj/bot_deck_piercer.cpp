#include "misc/bot_log.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_deck_piercer.h"
#include "screen/bot_game_screen.h"

namespace bot {

DeckPiercer::DeckPiercer()
    : Missile()
{
}

DeckPiercer::~DeckPiercer()
{
}

bool DeckPiercer::init(const DeckPiercerTemplate* t, Side side,
                       float x, float y, float directionX, float directionY,
                       float damage, float speed)
{
    return Missile::init(t, side, x, y, directionX, directionY, damage, speed);
}

void DeckPiercer::present()
{
    const DeckPiercerTemplate* t = getTemplate();
    const Color* color = m_penetrateObjs.isEmpty() ?
                                nullptr : &(t->getPassThroughMask());

    t->getRect()->draw(m_pos, m_direction, nullptr, nullptr,
                       t->getTexture(), color);
}

void DeckPiercer::update(float delta, GameScreen& screen)
{
    LinkedList<GameObjectItem> collideObjs;
    GameMap& map = screen.getMap();
    GameObjectManager& gameObjMgr = screen.getGameObjManager();

    ReturnCode rc = map.checkCollision(this, &collideObjs);

    if (RET_CODE_OUT_OF_SIGHT == rc)
    {
        onDeath(screen);
        return;
    }

    if (!collideObjs.isEmpty())
    {
        bool stop = processPenetrateObjs(collideObjs, screen);

        if (!collideObjs.isEmpty())
        {
            gameObjMgr.freeGameObjItems(collideObjs);
        }

        if (stop)
        {
            const DeckPiercerTemplate* t = getTemplate();

            if (t->getExplodeOnDeath())
            {
                explode(screen, t->getExplodeBreath(),
                        t->getExplodeEffectTemplate());
            }
            else
            {
                onDeath(screen);
            }
        }
    }
}

void DeckPiercer::onDeath(GameScreen& screen)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();

    gameObjMgr.sendToDeathQueue(this);

    if (!m_penetrateObjs.isEmpty())
    {
        gameObjMgr.freeGameObjectItems(m_penetrateObjs);
    }
}

void DeckPiercer::processPenetrateObjs(LinkedList<GameObjectItem>& collideObjs,
                          GameScreen& screen)
{
    GameObjectItem* prev = nullptr, * next, * cur;
    GameObjectManager& gameObjMgr = screen.getGameObjManager();

    for (cur = m_penetrateObjs.getFirst(); cur; cur = next)
    {
        next = cur->getNext();

        // If cur is not in collideObjs, remove it from penetrateObjs
        if (!collideObjs.find(GameObjectItem::match, cur))
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
        if (m_penetrateObjs.find(GameObjectItem::match, cur))
        {
            prev = cur;
            continue;
        }

        GameObject* o = cur->getObj();
        bool alive = true;

        if (o->testFlag(GAME_OBJ_FLAG_INDESTRUCTABLE))
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
            o->onDeath(screen);
            prev = cur;
        }
        else
        {
            collideObjs.unlink(prev, cur);
            m_penetrateObjs.add(cur);
        }
    }

    return stop;
}

} // end of namespace bot

