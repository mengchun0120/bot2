#include "misc/bot_log.h"
#include "gameobj/bot_game_object_flag.h"
#include "gameobj/bot_ai_robot.h"
#include "ai/bot_ai.h"
#include "screen/bot_game_screen.h"

namespace bot {

AIRobot::AIRobot()
    : m_curAction(ACTION_NONE)
{
}

bool AIRobot::init(const AIRobotTemplate *t, Side side,
                   int hpLevel, int hpRestoreLevel,
                   int armorLevel, int armorRepairLevel,
                   int powerLevel, int powerRestoreLevel,
                   int weaponLevel, int moverLevel,
                   const std::vector<int> &skillLevels,
                   float x, float y,
                   float directionX, float directionY)
{
    bool ret = Robot::init(t, side, hpLevel, hpRestoreLevel, armorLevel,
                           armorRepairLevel, powerLevel, powerRestoreLevel,
                           weaponLevel, moverLevel, skillLevels,
                           x, y, directionX, directionY);

    if (!ret)
    {
        return false;
    }

    m_lastChangeActionTime = Clock::now();
    m_lastChangeDirectionTime = Clock::now();

    return true;
}

bool AIRobot::init(const AIRobotTemplate *t, Side side,
                   int hpLevel, int hpRestoreLevel,
                   int armorLevel, int armorRepairLevel,
                   int powerLevel, int powerRestoreLevel,
                   int weaponLevel, int moverLevel, int skillLevel,
                   float x, float y,
                   float directionX, float directionY)
{
    bool ret = Robot::init(t, side, hpLevel, hpRestoreLevel, armorLevel,
                           armorRepairLevel, powerLevel, powerRestoreLevel,
                           weaponLevel, moverLevel, skillLevel,
                           x, y, directionX, directionY);

    if (!ret)
    {
        return false;
    }

    m_lastChangeActionTime = Clock::now();
    m_lastChangeDirectionTime = Clock::now();

    return true;
}

void AIRobot::present()
{
    Robot::present();
}

void AIRobot::update(float delta, GameScreen &screen)
{
    if (!testFlag(GAME_OBJ_FLAG_DISSOLVE))
    {
        const GameObjectManager &gameObjMgr = screen.getGameObjManager();
        if (gameObjMgr.isPlayerAlive())
        {
            const AIRobotTemplate *t =
                        static_cast<const AIRobotTemplate*>(m_template);
            t->getAI()->apply(*this, delta, screen);
        }
    }
    else
    {
        if (!updateMask())
        {
            GameObjectManager &gameObjMgr = screen.getGameObjManager();
            gameObjMgr.sendToDeathQueue(this);
        }
    }
}

void AIRobot::setDirection(float directionX, float directionY)
{
    Robot::setDirection(directionX, directionY);
    m_lastChangeDirectionTime = Clock::now();
}

bool AIRobot::setCurAction(Action action)
{
    if (!isValidAction(action))
    {
        return false;
    }

    if (m_curAction == action)
    {
        return true;
    }

    m_lastChangeActionTime = Clock::now();
    m_curAction = action;

    return true;
}

void AIRobot::onDeath(GameScreen &screen)
{
    GameObjectManager &gameObjMgr = screen.getGameObjManager();
    GameMap &map = screen.getMap();

    Goodie *goodie = gameObjMgr.createGoodie(getGoodieSpawnProb(),
                                             getPosX(), getPosY());
    if (goodie)
    {
        map.addObject(goodie);
    }

    gameObjMgr.sendToDissolveQueue(this);
    m_deathTime = Clock::now();
}

} // end of namespace bot

