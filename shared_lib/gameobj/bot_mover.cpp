#include "misc/bot_log.h"
#include "opengl/bot_graphics.h"
#include "gametemplate/bot_mover_template.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_mover.h"
#include "screen/bot_game_screen.h"

namespace bot {

Mover::Mover()
    : m_moverTemplate(nullptr)
    , m_moverLevel(1)
    , m_moving(false)
    , m_speed(0.0f)
    , m_speedMultiplier(1.0f)
{
}

bool Mover::init(const MoverTemplate* moverTemplate, int moverLevel)
{
    if (moverLevel < 1)
    {
        LOG_ERROR("Invalid mover-level %d", moverLevel);
        return false;
    }

    m_moverTemplate = moverTemplate;
    m_moverLevel = moverLevel;
    m_moving = false;
    m_speedMultiplier = 1.0f;
    resetSpeed();

    return true;
}

void Mover::update(GameScreen& screen, Robot& robot, float delta)
{
    if (!m_moving)
    {
        return;
    }

    //TODO
}

void Mover::present(Graphics& g, const float* pos, const float* direction)
{
    m_moverTemplate->getRect()->draw(g, pos, direction, nullptr, nullptr,
                                     m_moverTemplate->getTexture()->textureId(), nullptr);
}

bool Mover::setSpeedMultiplier(float multiplier)
{
    if (multiplier < 0.0f)
    {
        LOG_ERROR("Invalid speed-multiplier %f", multiplier);
        return false;
    }

    m_speedMultiplier = multiplier;
    resetSpeed();

    return true;
}

void Mover::resetSpeed()
{
    m_speed = m_moverTemplate->getSpeed(m_moverLevel) * m_speedMultiplier;
}

} // end of namespace bot

