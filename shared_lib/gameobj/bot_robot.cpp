#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "gameutil/bot_collide.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_player.h"
#include "screen/bot_game_screen.h"

namespace bot {

Robot::Robot(const RobotTemplate* t, const BaseComponentTemplate* baseTemplate,
             const WeaponComponentTemplate* weaponTemplate, const MoverComponentTemplate* moverTemplate,
             const MissileTemplate* missileTemplate, float x, float y, float directionX, float directionY,
             Side side)
    : GameObject(t)
    , m_base(baseTemplate)
    , m_weapon(weaponTemplate, missileTemplate)
    , m_mover(moverTemplate)
    , m_side(side)
    , m_lastChangeActionTime(Clock::now())
    , m_lastChangeDirectionTime(Clock::now())
    , m_curAction(ACTION_NONE)
{
    m_pos[0] = x;
    m_pos[1] = y;
    m_direction[0] = directionX;
    m_direction[1] = directionY;

    resetWeaponPos();
    resetMoverPos();

    resetHPStr();
}

Robot::~Robot()
{
}

void Robot::present(Graphics& g)
{
    m_base.present(g, m_pos, m_direction);
    m_weapon.present(g, m_weaponPos, m_direction);
    m_mover.present(g, m_moverPos, m_direction);
}

void Robot::shiftPos(float deltaX, float deltaY)
{
    m_pos[0] += deltaX;
    m_pos[1] += deltaY;

    m_weaponPos[0] += deltaX;
    m_weaponPos[1] += deltaY;
    m_weapon.shiftFirePoints(deltaX, deltaY);

    m_moverPos[0] += deltaX;
    m_moverPos[1] += deltaY;
}

void Robot::setPos(float x, float y)
{
    shiftPos(x - m_pos[0], y - m_pos[1]);
}

void Robot::setDirection(float directionX, float directionY)
{
    m_direction[0] = directionX;
    m_direction[1] = directionY;
    resetWeaponPos();
    resetMoverPos();
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

    m_base.addHP(delta);
    resetHPStr();

    return m_base.getHP() > 0.0f;
}

void Robot::refillHP()
{
    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return;
    }

    m_base.restoreHP();
    resetHPStr();
}

void Robot::setCurAction(Action action)
{
    m_curAction = action;
    m_lastChangeActionTime = Clock::now();
}

bool Robot::updateMoveAbility(float delta, GameScreen& gameScreen)
{
    return true;
}

void Robot::updateShootAbility(GameScreen& gameScreen)
{
}

void Robot::resetWeaponPos()
{
    const RobotTemplate* t = getTemplate();
    float dx = t->getWeaponPosX();
    float dy = t->getWeaponPosY();

    rotate(dx, dy, m_direction[0], m_direction[1]);
    m_weaponPos[0] = m_pos[0] + dx;
    m_weaponPos[1] = m_pos[1] + dy;
    m_weapon.setFirePoints(m_weaponPos[0], m_weaponPos[1], m_direction[0], m_direction[1]);
}

void Robot::resetMoverPos()
{
    const RobotTemplate* t = getTemplate();
    float dx = t->getMoverPosX();
    float dy = t->getMoverPosY();

    rotate(dx, dy, m_direction[0], m_direction[1]);
    m_moverPos[0] = m_pos[0] + dx;
    m_moverPos[1] = m_pos[1] + dy;
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

void Robot::resetHPStr()
{
    snprintf(m_hpStr, sizeof(m_hpStr), "%d", static_cast<int>(getHP()));
}

} // end of namespace bot
