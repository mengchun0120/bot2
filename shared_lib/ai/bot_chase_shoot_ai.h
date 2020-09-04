#ifndef INCLUDE_BOT_CHASE_SHOOT_AI
#define INCLUDE_BOT_CHASE_SHOOT_AI

#include <utility>
#include <random>
#include "misc/bot_time_utils.h"
#include "gameobj/bot_action.h"
#include "ai/bot_ai.h"

namespace bot {

class ChaseShootAI : public AI {
public:
    ChaseShootAI();

    ChaseShootAI(float chaseDurationMs, float shootDurationMs, float directionChangeIntervalMs,
                 float chaseProb, float stopChaseDist);

    virtual ~ChaseShootAI()
    {}

    virtual bool init(const rapidjson::Value& elem);

    virtual void apply(Robot& robot, float delta, GameScreen& screen);

protected:
    bool tryChangeAction(Robot& robot, GameScreen& screen);

    void resetAction(Robot& robot, Action action);

    void applyChaseAction(Robot& robot, float delta, GameScreen& screen);

    void applyShootAction(Robot& robot, float delta, GameScreen& screen);

    bool resetChaseDirection(Robot& robot, float delta, GameScreen& screen);

    bool tryFirstDirection(Robot& robot, float& directionX, float& directionY, float delta, GameScreen& screen);

    int findNewDirection(Robot& robot, float delta, float refDirectionX, float refDirectionY, GameScreen& screen);

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
