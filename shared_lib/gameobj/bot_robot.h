#ifndef INCLUDE_BOT_ROBOT
#define INCLUDE_BOT_ROBOT

#include "structure/bot_linked_list.h"
#include "gameutil/bot_game_object_item.h"
#include "gametemplate/bot_robot_template.h"
#include "gameobj/bot_game_object.h"
#include "gameobj/bot_side.h"
#include "gameobj/bot_action.h"
#include "gameobj/bot_base.h"
#include "gameobj/bot_weapon.h"
#include "gameobj/bot_mover.h"

namespace bot {

class Robot : public GameObject {
    enum {
        HP_STR_LEN = 30
    };

public:
    Robot();

    virtual ~Robot()
    {}

    bool init(const RobotTemplate* t, Side side, int hpLevel, int hpRestoreLevel,
              int armorLevel, int armorRepairLevel, int powerLevel, int powerRestoreLevel,
              int weaponLevel, int missileLevel, int moverLevel, float x, float y,
              float directionX, float directionY);

    virtual void present(Graphics& g);

    void shiftPos(float deltaX, float deltaY);

    void setPos(float x, float y);

    const float* getDirection() const
    {
        return m_direction;
    }

    float getDirectionX() const
    {
        return m_direction[0];
    }

    float getDirectionY() const
    {
        return m_direction[1];
    }

    void setDirection(float directionX, float directionY);

    float getHP() const
    {
        return m_base.getHP();
    }

    float getHPRatio() const
    {
        return m_base.getHPRatio();
    }

    bool addHP(float delta);

    void refillHP();

    const char* getHPStr() const
    {
        return m_hpStr;
    }

    const RobotTemplate* getTemplate() const
    {
        return static_cast<const RobotTemplate*>(m_template);
    }

    void setMovingEnabled(bool enabled)
    {
        m_mover.setMoving(enabled);
    }

    bool isMoving() const
    {
        return m_mover.isMoving();
    }

    float getSpeed() const
    {
        return m_mover.getSpeed();
    }

    void setShootingEnabled(bool enabled)
    {
        m_weapon.setFiring(enabled);
    }

    bool isShooting() const
    {
        return m_weapon.isFiring();
    }

    Side getSide() const
    {
        return m_side;
    }


    virtual bool updateMoveAbility(float delta, GameScreen& gameScreen);

    virtual void updateShootAbility(GameScreen& gameScreen);

private:
    void resetWeaponPos();

    void resetMoverPos();

    void processCollisions(LinkedList<GameObjectItem>& collideObjs, GameScreen& gameScreen);

    void resetHPStr();

protected:
    BaseComponent m_base;
    WeaponComponent m_weapon;
    MoverComponent m_mover;
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    Side m_side;
    char m_hpStr[HP_STR_LEN];
};

} // end of namespace bot

#endif
