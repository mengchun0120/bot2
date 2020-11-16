#include "misc/bot_log.h"
#include "opengl/bot_texture.h"
#include "gametemplate/bot_mover_template.h"
#include "gameobj/bot_robot.h"
#include "screen/bot_game_screen.h"

namespace bot {

Mover::Mover()
    : m_moverTemplate(nullptr)
    , m_robot(nullptr)
    , m_moving(false)
    , m_speed(0.0f)
    , m_normalSpeed(0.0f)
    , m_speedMultiplier(1.0f)
{
}

bool Mover::init(const MoverTemplate* moverTemplate, Robot* robot,
                 int moverLevel)
{
    if (!moverTemplate)
    {
        LOG_ERROR("moverTemplate is null");
        return false;
    }

    if (!robot)
    {
        LOG_ERROR("robot is null");
        return false;
    }

    if (moverLevel < 0)
    {
        LOG_ERROR("Invalid mover-level %d", moverLevel);
        return false;
    }

    m_moverTemplate = moverTemplate;
    m_robot = robot;
    m_moving = false;
    m_normalSpeed = moverTemplate->getSpeed(moverLevel);
    m_speedMultiplier = 1.0f;
    resetSpeed();

    return true;
}

bool Mover::update(GameScreen& screen, float delta)
{
    if (!m_moving)
    {
        return false;
    }

    float speedX = m_speed * m_robot->getDirectionX();
    float speedY = m_speed * m_robot->getDirectionY();
    float newDelta;
    LinkedList<GameObjectItem> collideObjs;
    GameMap& map = screen.getMap();

    bool collide = map.checkCollision(newDelta, &collideObjs, m_robot,
                                      speedX, speedY, delta);

    if (!collideObjs.isEmpty())
    {
        m_robot->processCollisions(collideObjs, screen);
        map.freeGameObjList(collideObjs);
    }

    m_robot->shiftPos(speedX * newDelta, speedY * newDelta);
    map.repositionObject(m_robot);

    return collide;
}

void Mover::present()
{
    const Base& base = m_robot->getBase();
    m_moverTemplate->getRect()->draw(
                                base.getMoverPos(), m_robot->getDirection(),
                                nullptr, nullptr,
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

