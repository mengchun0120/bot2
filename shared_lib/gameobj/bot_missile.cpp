#include <cassert>
#include <sstream>
#include "misc/bot_log.h"
#include "structure/bot_linked_list.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_collide.h"
#include "gameutil/bot_game_object_item.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_missile.h"
#include "screen/bot_game_screen.h"
#include "screen/bot_screen_manager.h"

namespace bot {

Missile::Missile()
    : GameObject()
    , m_side(SIDE_UNKNOWN)
    , m_damage(0.0f)
    , m_speed(0.0f)
{
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
}

bool Missile::init(const MissileTemplate* t, Side side, float x, float y,
                   float directionX, float directionY,
                   float damage, float speed);
{
    if (!GameObject::init(t, x, y))
    {
        return false;
    }

    if (!isValid(side))
    {
        LOG_ERROR("Invalid side %d", static_cast<int>(side));
        return false;
    }

    if (damage <= 0.0f)
    {
        LOG_ERROR("Invalid damage %f", damage);
        return false;
    }

    if (speed <= 0.0f)
    {
        LOG_ERROR("Invalid speed %f", speed);
        return false;
    }

    setDirection(directionX, directionY);
    m_side = side;
    m_damage = damage;
    m_speed = speed;

    return true;
}

void Missile::shiftPos(float deltaX, float deltaY)
{
    m_pos[0] += deltaX;
    m_pos[1] += deltaY;
}

void Missile::setPos(float x, float y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void Missile::setDirection(float directionX, float directionY)
{
    m_direction[0] = directionX;
    m_direction[1] = directionY;
}

} // end of namespace bot

