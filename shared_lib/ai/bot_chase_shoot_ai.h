#ifndef INCLUDE_BOT_CHASE_SHOOT_AI
#define INCLUDE_BOT_CHASE_SHOOT_AI

#include <utility>
#include <random>
#include "ai/bot_ai.h"

namespace bot {

class ChaseShootAI : public AI {
public:
    ChaseShootAI();

    virtual ~ChaseShootAI()
    {}

    virtual bool init(const rapidjson::Value& elem);

    virtual void apply(AIRobot& robot, float delta, GameScreen& screen);

protected:
    bool tryChangeAction(AIRobot& robot, GameScreen& screen);

    void resetAction(AIRobot& robot, Action action);

    void applyChaseAction(AIRobot& robot, float delta, GameScreen& screen);

    void applyShootAction(AIRobot& robot, float delta, GameScreen& screen);

    bool resetChaseDirection(AIRobot& robot, float delta, GameScreen& screen);

    bool tryFirstDirection(AIRobot& robot, float& directionX, float& directionY,
                           float delta, GameScreen& screen);

    int findNewDirection(AIRobot& robot, float delta, float refDirectionX,
                         float refDirectionY, GameScreen& screen);

    int sortDirections(int* result, float refDirectionX, float refDirectionY);

    void randomChoose(int* result, int i1, int i2);

protected:
    float m_chaseDurationMs;
    float m_directionChangeIntervalMs;
    float m_shootDurationMs;
    float m_chaseProb;
    float m_stopChaseDist;
    std::mt19937 m_generator;
    std::uniform_real_distribution<float> m_distribution;
    const std::pair<float, float> m_directions[4];
    const int m_numDirections;
};

} // end of namespace bot

#endif
