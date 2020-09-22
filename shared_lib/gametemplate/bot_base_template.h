#ifndef INCLUDE_BOT_BASE_TEMPLATE
#define INCLUDE_BOT_BASE_TEMPLATE

#include <string>
#include "misc/bot_constants.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class BaseTemplate: public SingleUnitTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
        {}

        ~Parser()
        {}

        BaseTemplate* create(std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
    };

public:
    BaseTemplate();

    virtual ~BaseTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    float getHP(int level) const
    {
        return m_hp + m_hpPerLevel * (level - 1);
    }

    void setHP(float hp);

    bool setHPPerLevel(float hpPerLevel);

    float getHPRestoreRate(int level) const
    {
        return m_hpRestoreRate + m_hpRestoreRatePerLevel * (level - 1);
    }

    bool setHPRestoreRate(float rate);

    bool setHPRestoreRatePerLevel(float ratePerLevel);

    float getArmor(int level) const
    {
        return m_armor + m_armorPerLevel * (level - 1);
    }

    bool setArmor(float armor);

    bool setArmorPerLevel(float armorPerLevel);

    float getArmorRepairRate(int level) const
    {
        return m_armorRepairRate + m_armorRepairRatePerLevel * (level - 1);
    }

    bool setArmorRepairRate(float rate);

    bool setArmorRepairRatePerLevel(float ratePerLevel);

    float getPower(int level) const
    {
        return m_power + m_powerPerLevel * (level - 1);
    }

    bool setPower(float power);

    bool setPowerPerLevel(float powerPerLevel);

    float getPowerRestoreRate(int level) const
    {
        return m_powerRestoreRate + m_powerRestoreRatePerLevel * (level - 1);
    }

    bool setPowerRestoreRate(float rate);

    bool setPowerRestoreRatePerLevel(float ratePerLevel);

    int getMissileCapacity(int level) const
    {
        return m_missileCapacity + m_missileCapacityPerLevel * (level - 1);
    }

    bool setMissileCapacity(int capacity);

    bool setMissileCapacityPerLevel(int capacityPerLevel);

    float getWeaponPosX() const
    {
        return m_weaponPos[0];
    }

    float getWeaponPosY() const
    {
        return m_weaponPos[1];
    }

    void setWeaponPos(float x, float y)
    {
        m_weaponPos[0] = x;
        m_weaponPos[1] = y;
    }

    float getMoverPosX() const
    {
        return m_moverPos[0];
    }

    float getMoverPosY() const
    {
        return m_moverPos[1];
    }

    void setMoverPos(float x, float y)
    {
        m_moverPos[0] = x;
        m_moverPos[1] = y;
    }

protected:
    float m_hp;
    float m_hpPerLevel;
    float m_hpRestoreRate;
    float m_hpRestoreRatePerLevel;
    float m_armor;
    float m_armorPerLevel;
    float m_armorRepairRate;
    float m_armorRepairRatePerLevel;
    float m_power;
    float m_powerPerLevel;
    float m_powerRestoreRate;
    float m_powerRestoreRatePerLevel;
    int m_missileCapacity;
    int m_missileCapacityPerLevel;
    float m_weaponPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_moverPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

