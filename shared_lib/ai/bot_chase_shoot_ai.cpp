#include <utility>
#include "misc/bot_log.h"
#include "misc/bot_time_utils.h"
#include "misc/bot_math_utils.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_player.h"
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

ChaseShootAI::ChaseShootAI(float chaseDurationMs, float shootDurationMs, float directionChangeIntervalMs,
                           float chaseProb, float stopChaseDist)
    : m_chaseDurationMs(chaseDurationMs)
    , m_directionChangeIntervalMs(directionChangeIntervalMs)
    , m_shootDurationMs(shootDurationMs)
    , m_chaseProb(chaseProb)
    , m_stopChaseDist(stopChaseDist)
    , m_generator(std::random_device()())
    , m_distribution(0.0, 1.0)
    , m_directions{ {1.0f, 0.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f}, {0.0f, -1.0f} }
    , m_numDirections(4)
{
}

bool ChaseShootAI::init(const rapidjson::Value& elem)
{
    std::vector<JsonParseParam> params =
    {
        {&m_chaseDurationMs,           "chaseDuration",           JSONTYPE_FLOAT},
        {&m_directionChangeIntervalMs, "directionChangeInterval", JSONTYPE_FLOAT},
        {&m_shootDurationMs,           "shootDuration",           JSONTYPE_FLOAT},
        {&m_chaseProb,                 "chaseProb",               JSONTYPE_FLOAT},
        {&m_stopChaseDist,             "stopChaseDist",           JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

void ChaseShootAI::apply(Robot& robot, float delta, GameScreen& screen)
{
    GameMap& map = screen.getMap();
    Player* player = map.getPlayer();

    bool dontApply = robot.testFlag(GAME_OBJ_FLAG_DEAD) ||
                     player == nullptr ||
                     player->testFlag(GAME_OBJ_FLAG_DEAD);

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
            LOG_ERROR("Wrong action: %d", static_cast<int>(robot.getCurAction()));
            break;
    }
}

bool ChaseShootAI::tryChangeAction(Robot& robot, GameScreen& screen)
{
    bool rollDice = false;
    const Player* player = screen.getMap().getPlayer();
    float d = dist(robot.getPosX(), robot.getPosY(), player->getPosX(), player->getPosY());

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
            if (d <= m_stopChaseDist)
            {
                resetAction(robot, ACTION_SHOOT);
                return true;
            }
            else if (elapsedTimeMs(robot.getLastChangeActionTime()) >= m_chaseDurationMs)
            {
                rollDice = true;
            }
            break;
        case ACTION_SHOOT:
            if (d > m_stopChaseDist && elapsedTimeMs(robot.getLastChangeActionTime()) >= m_shootDurationMs)
            {
                rollDice = true;
            }
            break;
        default:
            LOG_WARN("Unexpected action %d", static_cast<int>(robot.getCurAction()));
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

void ChaseShootAI::resetAction(Robot& robot, Action action)
{
    switch (action)
    {
        case ACTION_NONE:
            robot.setCurAction(action);
            robot.setMovingEnabled(false);
            robot.setShootingEnabled(false);
            return;
        case ACTION_SHOOT:
            robot.setCurAction(action);
            robot.setShootingEnabled(true);
            robot.setMovingEnabled(false);
            return;
        case ACTION_CHASE:
            robot.setCurAction(action);
            robot.setMovingEnabled(true);
            robot.setShootingEnabled(false);
            return;
    }
}

void ChaseShootAI::applyChaseAction(Robot& robot, float delta, GameScreen& screen)
{
    bool collide = robot.updateMoveAbility(delta, screen);

    if (robot.testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return;
    }

    TimePoint tp = Clock::now();
    bool changeDirection = timeDistMs(robot.getLastChangeDirectionTime(), tp) >= m_directionChangeIntervalMs;

    if (!collide && !changeDirection)
    {
        return;
    }

    if (!resetChaseDirection(robot, delta, screen))
    {
        if (robot.getShootAbility())
        {
            resetAction(robot, ACTION_SHOOT);
        }
        else
        {
            resetAction(robot, ACTION_NONE);
        }
    }
}

void ChaseShootAI::applyShootAction(Robot& robot, float delta, GameScreen& screen)
{
    Player* player = screen.getMap().getPlayer();
    float directionX, directionY;

    calculateDirection(directionX, directionY, robot.getPosX(), robot.getPosY(), player->getPosX(), player->getPosY());
    robot.setDirection(directionX, directionY);

    robot.updateShootAbility(screen);
}

bool ChaseShootAI::resetChaseDirection(Robot& robot, float delta, GameScreen& screen)
{
    float firstDirectionX, firstDirectionY;

    if (tryFirstDirection(robot, firstDirectionX, firstDirectionY, delta, screen))
    {
        return true;
    }

    int index = findNewDirection(robot, delta, firstDirectionX, firstDirectionY, screen);
    if (index < 0)
    {
        LOG_WARN("Failed to find new direction");
        return false;
    }

    robot.setDirection(m_directions[index].first, m_directions[index].second);
    return true;
}

bool ChaseShootAI::tryFirstDirection(Robot& robot, float& directionX, float& directionY,
                                     float delta, GameScreen& screen)
{
    GameMap& map = screen.getMap();
    Player* player = map.getPlayer();
    float speed = robot.getMoveAbility()->getSpeed();
    float speedX, speedY;
    float newDelta;

    calculateDirection(directionX, directionY, robot.getPosX(), robot.getPosY(), player->getPosX(), player->getPosY());
    speedX = speed * directionX;
    speedY = speed * directionY;

    if (map.checkCollision(newDelta, nullptr, &robot, speedX, speedY, delta))
    {
        return false;
    }

    robot.setDirection(directionX, directionY);
    return true;
}

int ChaseShootAI::findNewDirection(Robot& robot, float delta, float refDirectionX, float refDirectionY,
                                    GameScreen& screen)
{
    int tryDirections[4];
    int numTries = sortDirections(tryDirections, refDirectionX, refDirectionY);

    GameMap& map = screen.getMap();
    float speed = robot.getMoveAbility()->getSpeed();
    float speedX, speedY;
    float newDelta;

    for (int i = 0; i < numTries; ++i)
    {
        int j = tryDirections[i];
        speedX = m_directions[j].first * speed;
        speedY = m_directions[j].second * speed;
        if (!map.checkCollision(newDelta, nullptr, &robot, speedX, speedY, delta))
        {
            return j;
        }
    }

    return -1;
}

int ChaseShootAI::sortDirections(int* result, float refDirectionX, float refDirectionY)
{
    bool matchFound = false;
    int i, j;

    for (i = 0; i < m_numDirections; ++i)
    {
        if (refDirectionX == m_directions[i].first && refDirectionY == m_directions[i].second)
        {
            matchFound = true;
            break;
        }

        j = (i + 1) % m_numDirections;
        bool isBetween = between(refDirectionX, m_directions[i].first, m_directions[j].first) &&
                         between(refDirectionY, m_directions[i].second, m_directions[j].second);

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

void ChaseShootAI::randomChoose(int* result, int i1, int i2)
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
