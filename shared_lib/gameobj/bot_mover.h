#ifndef INCLUDE_BOT_MOVER
#define INCLUDE_BOT_MOVER

#include "opengl/bot_color.h"

namespace bot {

class MoverTemplate;
class GameScreen;
class Robot;

class Mover {
public:
    Mover();

    virtual ~Mover()
    {}

    bool init(const MoverTemplate* moverTemplate, Robot* robot,
              int moverLevel);

    bool update(GameScreen& screen, float delta);

    void present();

    bool isMoving() const
    {
        return m_moving;
    }

    void setMoving(bool enabled)
    {
        m_moving = enabled;
    }

    float getSpeed() const
    {
        return m_speed;
    }

    bool setSpeedMultiplier(float multiplier);

private:
    void resetSpeed()
    {
        m_speed = m_normalSpeed * m_speedMultiplier;
    }

protected:
    const MoverTemplate* m_moverTemplate;
    Robot* m_robot;
    bool m_moving;
    float m_speed;
    float m_normalSpeed;
    float m_speedMultiplier;
};

} // end of namespace bot

#endif

