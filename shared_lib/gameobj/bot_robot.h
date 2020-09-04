#ifndef INCLUDE_BOT_ROBOT
#define INCLUDE_BOT_ROBOT

#include "misc/bot_time_utils.h"
#include "structure/bot_linked_list.h"
#include "gameobj/bot_game_object.h"
#include "gameobj/bot_move_ability.h"
#include "gameobj/bot_shoot_ability.h"
#include "gameobj/bot_side.h"
#include "gameobj/bot_action.h"
#include "gameutil/bot_game_object_item.h"
#include "gametemplate/bot_robot_template.h"

namespace bot {

class Ability;

class Robot : public GameObject {
    struct Component {
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

public:
    Robot(const RobotTemplate* t);

    virtual ~Robot();

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

    int getHP() const
    {
        return m_hp;
    }

    virtual bool addHP(int deltaHP);

    const RobotTemplate* getTemplate() const
    {
        return static_cast<const RobotTemplate*>(m_template);
    }

    MoveAbility* getMoveAbility()
    {
        return static_cast<MoveAbility*>(m_abilities[ABILITY_MOVE]);
    }

    const MoveAbility* getMoveAbility() const
    {
        return static_cast<const MoveAbility*>(m_abilities[ABILITY_MOVE]);
    }

    Component* getComponentForMoveAbility();

    ShootAbility* getShootAbility()
    {
        return static_cast<ShootAbility*>(m_abilities[ABILITY_SHOOT]);
    }

    const ShootAbility* getShootAbility() const
    {
        return static_cast<const ShootAbility*>(m_abilities[ABILITY_SHOOT]);
    }

    bool resetShootPos();

    Component* getComponentForShootAbility();

    bool setMovingEnabled(bool enabled);

    bool isMoving() const;

    bool setShootingEnabled(bool enabled);

    bool isShooting() const;

    Side getSide() const
    {
        return m_side;
    }

    void setSide(Side side)
    {
        m_side = side;
    }

    const TimePoint& getLastChangeActionTime() const
    {
        return m_lastChangeActionTime;
    }

    const TimePoint& getLastChangeDirectionTime() const
    {
        return m_lastChangeDirectionTime;
    }

    Action getCurAction() const
    {
        return m_curAction;
    }

    void setCurAction(Action action);

    virtual bool updateMoveAbility(float delta, GameScreen& gameScreen);

    virtual void updateShootAbility(GameScreen& gameScreen);

    float getGoodieSpawnProb() const
    {
        return getTemplate()->getGoodieSpawnProb();
    }

    float getHPRatio() const
    {
        return m_hpRatio;
    }

    void refillHP();

private:
    void initComponents();

    void initAbilities();

    void processCollisions(LinkedList<GameObjectItem>& collideObjs, GameScreen& gameScreen);

protected:
    int m_hp;
    float m_hpRatio;
    Side m_side;
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    Ability* m_abilities[NUM_OF_ABILITIES];
    std::vector<Component> m_components;
    TimePoint m_lastChangeActionTime;
    TimePoint m_lastChangeDirectionTime;
    Action m_curAction;
};

} // end of namespace bot

#endif
