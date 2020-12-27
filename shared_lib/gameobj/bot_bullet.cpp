#include "gameobj/bot_bullet.h"
#include "screen/bot_game_screen.h"

namespace bot {

Bullet::Bullet()
    : Missile()
{
}

Bullet::~Bullet()
{
}

bool Bullet::init(const BulletTemplate* t, Side side, float x, float y,
                  float directionX, float directionY, float damage, float speed)
{
    return Missile::init(t, side, x, y, directionX, directionY, damage, speed);
}

void Bullet::present()
{
    const BulletTemplate* t = getTemplate();
    t->getRect()->draw(m_pos, m_direction, nullptr, nullptr,
                       t->getTexture(), nullptr);
}

void Bullet::update(float delta, GameScreen& screen)
{
    float deltaX = getSpeedX() * delta;
    float deltaY = getSpeedY() * delta;

    shiftPos(deltaX, deltaY);

    LinkedList<GameObjectItem> collideObjs;
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

    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    const BulletTemplate* t = getTemplate();

    processCollideObjs(screen, collideObjs);
    gameObjMgr.freeGameObjectItems(collideObjs);

    const ParticleEffectTemplate* e = t->getImpactEffectTemplate();
    if (e)
    {
        ParticleEffect* effect = gameObjMgr.createParticleEffect(
                                                e, getPosX(), getPosY())

        if (!map.addObject(effect))
        {
            gameObjMgr.sendToDeathQueue(effect);
        }
    }

    onDeath(screen);
}

void Bullet::processCollideObjs(GameScreen& screen,
                                LinkedList<GameObjectItem>& collideObjs)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    GameObjectItem* item;

    for (item = collideObjs.getFirst() item; item = item->getNext())
    {
        GameObject* obj = item->getObject();
        switch(obj->getType())
        {
            case GAME_OBJ_TYPE_TILE:
            {
                Tile* tile = static_cast<Tile*>(obj);

                if (!tile->addHP(-m_damage))
                {
                    tile->onDeath(screen);
                }

                break;
            }
            case GAME_OBJ_TYPE_ROBOT:
            {
                Robot* robot = static_cast<Robot*>(obj);

                if (robot->getSide() != m_side)
                {
                    if (!robot->addHP(-m_damage))
                    {
                        robot->onDeath(screen);
                    }
                }

                break;
            }
            default:
            {
                LOG_ERROR("Unexpected GameObjectType %d",
                          static_cast<int>(obj->getType()));
            }
        }
    }
}

void Bullet::onDeath(GameScreen& screen)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    gameObjMgr.sendToDeathQueue(this);
}

} // end of namespace bot

