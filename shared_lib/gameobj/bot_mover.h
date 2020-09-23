#ifndef INCLUDE_BOT_MOVER
#define INCLUDE_BOT_MOVER

namespace bot {

class MoverTemplate;
class GameScreen;
class Robot;
class Graphics;

class Mover {
public:
    Mover();

    virtual ~Mover()
    {}

    bool init(const MoverTemplate* moverTemplate, int moverLevel);

    void update(GameScreen& screen, Robot& robot, float delta);

    void present(Graphics& g, const float* pos, const float* direction);

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
    void resetSpeed();

protected:
    const MoverTemplate* m_moverTemplate;
    int m_moverLevel;
    bool m_moving;
    float m_speed;
    float m_speedMultiplier;
};

} // end of namespace bot

#endif

