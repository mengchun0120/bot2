#include "misc/bot_log.h"
#include "misc/bot_time_utils.h"
#include "misc/bot_math_utils.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_ai_robot.h"
#include "gameobj/bot_player.h"
#include "skill/bot_skill.h"
#include "screen/bot_game_screen.h"
#include "ai/bot_chase_shoot_ai.h"

namespace bot {

ChaseShootAI::ChaseShootAI()
    : m_chaseDurationMs(0.0f)
    , m_directionChangeIntervalMs(0.0f)
    , m_shootDurationMs(0.0f)
    , m_chaseProb(0.0f)
    , m_stopChaseDist(0.0f)
    , m_generator(std::random_device()())
    , m_distribution(0.0, 1.0)
    , m_directions{ {1.0f, 0.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f}, {0.0f, -1.0f} }
    , m_numDirections(4)
{

}

bool ChaseShootAI::init(const rapidjson::Value &elem)
{
    std::vector<JsonParamPtr> params =
    {
        jsonParam(m_chaseDurationMs, "chaseDuration",
                  gt(m_chaseDurationMs, 0.0f)),
        jsonParam(m_directionChangeIntervalMs, "directionChangeInterval",
                  gt(m_directionChangeIntervalMs, 0.0f)),
        jsonParam(m_shootDurationMs, "shootDuration",
                  gt(m_shootDurationMs, 0.0f)),
        jsonParam(m_chaseProb, "chaseProb",
                  ge(m_chaseProb, 0.0f) && le(m_chaseProb, 1.0f)),
        jsonParam(m_stopChaseDist, "stopChaseDist",
                  gt(m_stopChaseDist, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

void ChaseShootAI::apply(AIRobot &robot, float delta, GameScreen &screen)
{
    GameMap &map = screen.getMap();
    Player *player = map.getPlayer();
    int DEAD_FLAG = GAME_OBJ_FLAG_DISSOLVE | GAME_OBJ_FLAG_DEAD;

    bool dontApply = robot.testFlag(DEAD_FLAG) ||
                     player == nullptr ||
                     player->testFlag(DEAD_FLAG);

    if (dontApply)
    {
        return;
    }

    if (map.isOutsideViewport(&robot))
    {
        resetAction(robot, ACTION_NONE);
        return;
    }

    if (tryChangeAction(robot, screen))
    {
        return;
    }

    switch (robot.getCurAction())
    {
        case ACTION_CHASE:
            applyChaseAction(robot, delta, screen);
            break;
        case ACTION_SHOOT:
            applyShootAction(robot, delta, screen);
            break;
        default:
            LOG_ERROR("Wrong action: %d",
                      static_cast<int>(robot.getCurAction()));
            break;
    }
}

bool ChaseShootAI::tryChangeAction(AIRobot &robot, GameScreen &screen)
{
    bool rollDice = false;
    const Player *player = screen.getMap().getPlayer();
    float d = dist(robot.getPosX(), robot.getPosY(),
                   player->getPosX(), player->getPosY());

    switch (robot.getCurAction())
    {
        case ACTION_NONE:
        {
            if (d > m_stopChaseDist)
            {
                rollDice = true;
            }
            else
            {
                resetAction(robot, ACTION_SHOOT);
                return true;
            }
            break;
        }
        case ACTION_CHASE:
        {
            if (d <= m_stopChaseDist)
            {
                resetAction(robot, ACTION_SHOOT);
                return true;
            }
            else
            {
                float elapsedTime =
                        elapsedTimeMs(robot.getLastChangeActionTime());
                if (elapsedTime >= m_chaseDurationMs)
                {
                    rollDice = true;
                }
            }
            break;
        }
        case ACTION_SHOOT:
        {
            float elapsedTime = elapsedTimeMs(robot.getLastChangeActionTime());
            rollDice = d > m_stopChaseDist && elapsedTime >= m_shootDurationMs;
            break;
        }
        default:
            LOG_WARN("Unexpected action %d",
                     static_cast<int>(robot.getCurAction()));
            break;
    }

    if (!rollDice)
    {
        return false;
    }

    float dice = m_distribution(m_generator);

    if (dice <= m_chaseProb)
    {
        resetAction(robot, ACTION_CHASE);
    }
    else
    {
        resetAction(robot, ACTION_SHOOT);
    }

    return true;
}

void ChaseShootAI::resetAction(AIRobot &robot, Action action)
{
    switch (action)
    {
        case ACTION_NONE:
            robot.setCurAction(action);
            robot.setMovingEnabled(false);
            return;
        case ACTION_SHOOT:
            robot.setCurAction(action);
            robot.setMovingEnabled(false);
            return;
        case ACTION_CHASE:
            robot.setCurAction(action);
            robot.setMovingEnabled(true);
            return;
        default:
            LOG_ERROR("Invalid action: %d", static_cast<int>(action));
    }
}

void ChaseShootAI::applyChaseAction(AIRobot &robot, float delta,
                                    GameScreen &screen)
{
    bool collide = robot.updateMover(delta, screen);
    const int DEAD_FLAG = GAME_OBJ_FLAG_DEAD | GAME_OBJ_FLAG_DISSOLVE;

    if (robot.testFlag(DEAD_FLAG))
    {
        return;
    }

    TimePoint tp = Clock::now();
    float timeToNow = timeDistMs(robot.getLastChangeDirectionTime(), tp);
    bool changeDirection = timeToNow >= m_directionChangeIntervalMs;

    if (!collide && !changeDirection)
    {
        return;
    }

    if (!resetChaseDirection(robot, delta, screen))
    {
        resetAction(robot, ACTION_SHOOT);
    }
}

void ChaseShootAI::applyShootAction(AIRobot &robot, float delta,
                                    GameScreen &screen)
{
    Player *player = screen.getMap().getPlayer();
    float directionX, directionY;

    calculateDirection(directionX, directionY, robot.getPosX(), robot.getPosY(),
                       player->getPosX(), player->getPosY());
    robot.setDirection(directionX, directionY);

    unsigned int numSkills = robot.numSkills();
    Skill *skill = nullptr;
    TimePoint now = Clock::now();

    for (unsigned int i = 0; i < numSkills; ++i)
    {
        Skill *s = robot.getSkill(i);
        if (s->isOffensive() && s->available(now))
        {
            skill= s;
            break;
        }
    }

    if (skill)
    {
        skill->apply(screen, now);
    }
}

bool ChaseShootAI::resetChaseDirection(AIRobot &robot, float delta,
                                       GameScreen &screen)
{
    float directionX, directionY;

    if (tryFirstDirection(robot, directionX, directionY, delta, screen))
    {
        return true;
    }

    int index = findNewDirection(robot, delta, directionX, directionY,
                                 screen);
    if (index < 0)
    {
        LOG_WARN("Failed to find new direction");
        return false;
    }

    robot.setDirection(m_directions[index].first, m_directions[index].second);
    return true;
}

bool ChaseShootAI::tryFirstDirection(AIRobot &robot, float &directionX,
                                     float &directionY, float delta,
                                     GameScreen &screen)
{
    GameMap &map = screen.getMap();
    Player *player = map.getPlayer();
    float speed = robot.getSpeed();
    float speedX, speedY;
    float newDelta;

    calculateDirection(directionX, directionY, robot.getPosX(), robot.getPosY(),
                       player->getPosX(), player->getPosY());
    speedX = speed * directionX;
    speedY = speed * directionY;

    if (map.checkCollision(newDelta, nullptr, &robot, speedX, speedY, delta))
    {
        return false;
    }

    robot.setDirection(directionX, directionY);
    return true;
}

int ChaseShootAI::findNewDirection(AIRobot &robot, float delta,
                                   float refDirectionX, float refDirectionY,
                                   GameScreen &screen)
{
    int tryDirections[4];
    int numTries = sortDirections(tryDirections, refDirectionX, refDirectionY);

    GameMap &map = screen.getMap();
    float speed = robot.getSpeed();
    float speedX, speedY;
    float newDelta;

    for (int i = 0; i < numTries; ++i)
    {
        int j = tryDirections[i];
        speedX = m_directions[j].first * speed;
        speedY = m_directions[j].second * speed;
        bool collide = map.checkCollision(newDelta, nullptr, &robot,
                                          speedX, speedY, delta);
        if (!collide)
        {
            return j;
        }
    }

    return -1;
}

int ChaseShootAI::sortDirections(int *result, float refDirectionX,
                                 float refDirectionY)
{
    bool matchFound = false;
    int i, j;

    for (i = 0; i < m_numDirections; ++i)
    {
        auto &d1 = m_directions[i];

        if (refDirectionX == d1.first && refDirectionY == d1.second)
        {
            matchFound = true;
            break;
        }

        j = (i + 1) % m_numDirections;
        auto &d2 = m_directions[j];

        bool isBetween =
            between(refDirectionX, d1.first, d2.first) &&
            between(refDirectionY, d1.second, d2.second);

        if (isBetween)
        {
            break;
        }
    }

    int numTries;

    if (!matchFound)
    {
        randomChoose(result, i, j);
        i = (i - 1) % m_numDirections;
        j = (j + 1) % m_numDirections;
        randomChoose(result + 2, i, j);
        numTries = m_numDirections;
    }
    else
    {
        i = (i - 1) % m_numDirections;
        j = (j + 1) % m_numDirections;
        randomChoose(result, i, j);
        result[2] = (j + 1) % m_numDirections;
        numTries = 3;
    }

    return numTries;
}

void ChaseShootAI::randomChoose(int *result, int i1, int i2)
{
    float dice = m_distribution(m_generator);

    if (dice < 0.5f)
    {
        *result = i1;
        *(result + 1) = i2;
    }
    else
    {
        *result = i2;
        *(result + 1) = i1;
    }
}

} // end of namespace bot
