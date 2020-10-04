#ifndef INCLUDE_BOT_BASE
#define INCLUDE_BOT_BASE

#include "misc/bot_constants.h"
#include "misc/bot_math_utils.h"
#include "misc/bot_time_utils.h"

namespace bot {

class BaseTemplate;
class Graphics;
class GameScreen;

class Base {
    enum {
        HP_PERCENT_STR_LEN = 5
    };

public:
    Base();

    virtual ~Base()
    {}

    bool init(const BaseTemplate* t, int hpLevel, int hpRestoreLevel,
              int armorLevel, int armorRepairLevel,
              int powerLevel, int powerRestoreLevel,
              float x, float y, float directionX, float directionY);

    void update(GameScreen& screen);

    void present(Graphics& g, const float* pos, const float* direction);

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

    const char* getHPPercentStr() const
    {
        return m_hpPercentStr;
    }

    float getArmor() const
    {
        return m_armor;
    }

    void setArmor(float armor)
    {
        m_armor = clamp(armor, 0.0f, m_maxArmor);
    }

    float getPower() const
    {
        return m_power;
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

    const float* getWeaponPos() const
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

    const float* getMoverPos() const
    {
        return m_moverPos;
    }

    void shiftWeaponMoverPos(float deltaX, float deltaY);

    void setWeaponMoverPos(float x, float y,
                           float directionX, float directionY);

private:
    void resetHPPercentStr();

protected:
    const BaseTemplate* m_baseTemplate;
    float m_hp, m_maxHP, m_hpRestoreRate, m_hpPercent;
    float m_armor, m_maxArmor, m_armorRepairRate;
    float m_power, m_maxPower, m_powerRestoreRate;
    TimePoint m_lastUpdateTime;
    float m_weaponPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_moverPos[Constants::NUM_FLOATS_PER_POSITION];
    char m_hpPercentStr[HP_PERCENT_STR_LEN];
};

} // end of namespace bot

#endif

