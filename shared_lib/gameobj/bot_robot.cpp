#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "structure/bot_linked_list.h"
#include "gameutil/bot_game_object_item.h"
#include "gameutil/bot_collide.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_goodie.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_player.h"
#include "screen/bot_game_screen.h"

namespace bot {

Robot::Robot()
    : m_side(SIDE_UNKNOWN)
{
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
}

bool Robot::init(const RobotTemplate* t, Side side, int hpLevel, int hpRestoreLevel,
                 int armorLevel, int armorRepairLevel, int powerLevel, int powerRestoreLevel,
                 int weaponLevel, int missileLevel, int moverLevel, float x, float y,
                 float directionX, float directionY)
{
    bool ret = GameObject::init(t, x, y);
    if (!ret)
    {
        return false;
    }

    ret = m_base.init(t->getBaseTemplate(), hpLevel, hpRestoreLevel, armorLevel,
                      armorRepairLevel, powerLevel, powerRestoreLevel, x, y,
                      directionX, directionY);
    if (!ret)
    {
        return false;
    }

    ret = m_weapon.init(t->getWeaponTemplate(), weaponLevel, t->getMissileTemplate(), missileLevel,
                        m_base.getWeaponX(), m_base.getWeaponY(), directionX, directionY);
    if (!ret)
    {
        return false;
    }

    ret = m_mover.init(t->getMoverTemplate(), moverLevel);
    if (!ret)
    {
        return false;
    }

    if (side != SIDE_AI && side != SIDE_PLAYER)
    {
        LOG_ERROR("Invalid side %d", static_cast<int>(side));
        return false;
    }

    m_side = side;

    setDirection(directionX, directionY);

    return true;
}

void Robot::present(Graphics& g)
{
    m_weapon.present(g, m_base.getWeaponPos(), m_direction);
    m_base.present(g, m_pos, m_direction);
    m_mover.present(g, m_base.getMoverPos(), m_direction);
}

void Robot::shiftPos(float deltaX, float deltaY)
{
    m_pos[0] += deltaX;
    m_pos[1] += deltaY;
    m_base.shiftWeaponMoverPos(deltaX, deltaY);
    m_weapon.shiftFirePoints(deltaX, deltaY);
}

void Robot::setPos(float x, float y)
{
    shiftPos(x - m_pos[0], y - m_pos[1]);
}

void Robot::setDirection(float directionX, float directionY)
{
    m_direction[0] = directionX;
    m_direction[1] = directionY;
    m_base.setWeaponMoverPos(m_pos[0], m_pos[1], directionX, directionY);
    m_weapon.setFirePoints(m_base.getWeaponPosX(), m_base.getWeaponPosY(),
                           directionX, directionY);
}

bool Robot::addHP(float delta)
{
    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return false;
    }

    if (testFlag(GAME_OBJ_FLAG_INDESTRUCTABLE) && delta < 0.0f)
    {
        return true;
    }

    m_base.setHP(m_base.getHP() + delta);

    return m_base.getHP() > 0.0f;
}

void Robot::refillHP()
{
    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return;
    }

    m_base.refillHP();
}

void Robot::processCollisions(LinkedList<GameObjectItem>& collideObjs, GameScreen& gameScreen)
{
    for (GameObjectItem* item = collideObjs.getFirst(); item; item = static_cast<GameObjectItem*>(item->getNext()))
    {
        switch (item->getObj()->getType())
        {
            case GAME_OBJ_TYPE_MISSILE:
            {
                Missile* missile = static_cast<Missile*>(item->getObj());
                missile->explode(gameScreen);
                break;
            }
            case GAME_OBJ_TYPE_GOODIE:
            {
                if (m_side == SIDE_PLAYER)
                {
                    Player* player = static_cast<Player*>(this);
                    Goodie* goodie = static_cast<Goodie*>(item->getObj());
                    player->consumeGoodie(goodie, gameScreen);
                }
                break;
            }
            default:
            {
                LOG_WARN("Unexcepted game object type %d for collision", static_cast<int>(item->getObj()->getType()));
            }
        }
    }
}

} // end of namespace bot

