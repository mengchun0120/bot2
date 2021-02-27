#ifndef INCLUDE_BOT_BASE
#define INCLUDE_BOT_BASE

#include "misc/bot_constants.h"
#include "misc/bot_math_utils.h"
#include "misc/bot_time_utils.h"
#include "opengl/bot_color.h"

namespace bot {

class BaseTemplate;
class Robot;

class Base {
    enum {
        HP_PERCENT_STR_LEN = 5
    };

public:
    Base();

    virtual ~Base()
    {}

    bool init(const BaseTemplate *t, Robot *robot,
              int hpLevel, int hpRestoreLevel,
              int armorLevel, int armorRepairLevel,
              int powerLevel, int powerRestoreLevel);

    void update();

    void present();

    float getHP() const
    {
        return m_hp;
    }

    void setHP(float hp);

    void refillHP();

    float getHPPercent() const
    {
        return m_hpPercent;
    }

    const char *getHPPercentStr() const
    {
        return m_hpPercentStr;
    }

    float getArmor() const
    {
        return m_armor;
    }

    float getArmorRatio() const
    {
        return m_armor / m_maxArmor;
    }

    void setArmor(float armor)
    {
        m_armor = clamp(armor, 0.0f, m_maxArmor);
    }

    float getPower() const
    {
        return m_power;
    }

    float getPowerRatio() const
    {
        return m_power / m_maxPower;
    }

    void setPower(float power)
    {
        m_power = clamp(power, 0.0f, m_maxPower);
    }

    float getWeaponPosX() const
    {
        return m_weaponPos[0];
    }

    float getWeaponPosY() const
    {
        return m_weaponPos[1];
    }

    const float *getWeaponPos() const
    {
        return m_weaponPos;
    }

    float getMoverPosX() const
    {
        return m_moverPos[0];
    }

    float getMoverPosY() const
    {
        return m_moverPos[1];
    }

    const float *getMoverPos() const
    {
        return m_moverPos;
    }

    void shiftWeaponMoverPos(float deltaX, float deltaY);

    void resetWeaponMoverPos();

private:
    void resetHPPercentStr();

    void resetHPStrPos();

protected:
    const BaseTemplate *m_baseTemplate;
    Robot *m_robot;
    float m_hp, m_maxHP, m_hpRestoreRate;
    int m_hpPercent;
    float m_armor, m_maxArmor, m_armorRepairRate;
    float m_power, m_maxPower, m_powerRestoreRate;
    TimePoint m_lastUpdateTime;
    float m_weaponPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_moverPos[Constants::NUM_FLOATS_PER_POSITION];
    char m_hpPercentStr[HP_PERCENT_STR_LEN];
    float m_hpStrPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

