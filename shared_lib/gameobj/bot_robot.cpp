#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "structure/bot_linked_list.h"
#include "gameutil/bot_game_object_item.h"
#include "gameutil/bot_collide.h"
#include "gameutil/bot_game_config.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_goodie.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_player.h"
#include "screen/bot_game_screen.h"

namespace bot {

Robot::Robot()
    : m_side(SIDE_UNKNOWN)
    , m_mask()
{
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
    m_mask.init(255, 255, 255, 255);
}

bool Robot::init(const RobotTemplate* t, Side side,
                 int hpLevel, int hpRestoreLevel,
                 int armorLevel, int armorRepairLevel,
                 int powerLevel, int powerRestoreLevel,
                 int weaponLevel, int missileLevel,
                 int moverLevel, float x, float y,
                 float directionX, float directionY)
{
    bool ret = GameObject::init(t, x, y);
    if (!ret)
    {
        return false;
    }

    m_direction[0] = directionX;
    m_direction[1] = directionY;

    ret = m_base.init(t->getBaseTemplate(), this, hpLevel, hpRestoreLevel,
                      armorLevel, armorRepairLevel, powerLevel,
                      powerRestoreLevel);
    if (!ret)
    {
        return false;
    }

    ret = m_weapon.init(t->getWeaponTemplate(), this, weaponLevel, missileLevel);
    if (!ret)
    {
        return false;
    }

    ret = m_mover.init(t->getMoverTemplate(), this, moverLevel);
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

    return true;
}

void Robot::present()
{
    m_weapon.present();
    m_base.present();
    m_mover.present();
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
    m_base.resetWeaponMoverPos();
    m_weapon.resetFirePoints();
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

void Robot::processCollisions(LinkedList<GameObjectItem>& collideObjs,
                              GameScreen& gameScreen)
{
    GameObjectItem* item = collideObjs.getFirst();
    while (item)
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
                LOG_WARN("Unexcepted game object type %d for collision",
                         static_cast<int>(item->getObj()->getType()));
            }
        }

        item = static_cast<GameObjectItem*>(item->getNext());
    }
}

bool Robot::updateMover(float delta, GameScreen& gameScreen)
{
    return m_mover.update(gameScreen, delta);
}

bool Robot::updateWeapon(GameScreen& gameScreen)
{
    return m_weapon.update(gameScreen);
}

bool Robot::updateMask()
{
    const GameConfig& cfg = GameConfig::getInstance();
    float alpha = 1.0f - elapsedTimeMs(m_deathTime) / cfg.getDissovleTimeMS();
    bool maskVisible = alpha > 0.0f;

    if (maskVisible)
    {
        m_mask.setAlpha(alpha);
    }
    else
    {
        m_mask.setAlpha(0.0f);
    }

    m_base.setMask(m_mask);
    m_weapon.setMask(m_mask);
    m_mover.setMask(m_mask);

    return maskVisible;
}

} // end of namespace bot

