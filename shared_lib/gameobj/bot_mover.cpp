#include "misc/bot_log.h"
#include "opengl/bot_graphics.h"
#include "gametemplate/bot_mover_template.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_mover.h"
#include "screen/bot_game_screen.h"

namespace bot {

Mover::Mover()
    : m_moverTemplate(nullptr)
    , m_moving(false)
    , m_speed(0.0f)
    , m_normalSpeed(0.0f)
    , m_speedMultiplier(1.0f)
{
}

bool Mover::init(const MoverTemplate* moverTemplate, int moverLevel)
{
    if (moverLevel < 0)
    {
        LOG_ERROR("Invalid mover-level %d", moverLevel);
        return false;
    }

    m_moverTemplate = moverTemplate;
    m_moving = false;
    m_normalSpeed = moverTemplate->getSpeed(moverLevel);
    m_speedMultiplier = 1.0f;
    resetSpeed();

    return true;
}

bool Mover::update(GameScreen& screen, Robot& robot, float delta)
{
    if (!m_moving)
    {
        return false;
    }

    float speedX = m_speed * robot.getDirectionX();
    float speedY = m_speed * robot.getDirectionY();
    float newDelta;
    LinkedList<GameObjectItem> collideObjs;
    GameMap& map = screen.getMap();

    bool collide = map.checkCollision(newDelta, &collideObjs, &robot,
                                      speedX, speedY, delta);

    if (!collideObjs.isEmpty())
    {
        robot.processCollisions(collideObjs, screen);
        map.freeGameObjList(collideObjs);
    }

    robot.shiftPos(speedX * newDelta, speedY * newDelta);
    map.repositionObject(&robot);

    return collide;
}

void Mover::present(Graphics& g, const float* pos, const float* direction)
{
    m_moverTemplate->getRect()->draw(
                                g, pos, direction, nullptr, nullptr,
                                m_moverTemplate->getTexture()->textureId(),
                                nullptr);
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

} // end of namespace bot

