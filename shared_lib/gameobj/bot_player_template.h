#ifndef INCLUDE_BOT_PLAYER_TEMPLATE
#define INCLUDE_BOT_PLAYER_TEMPLATE

#include "gameobj/bot_robot_template.h"

namespace bot {

class PlayerTemplate: public RobotTemplate {
public:
    PlayerTemplate();

    virtual ~PlayerTemplate();

    bool init(const std::string &fileName);

    int getGold() const
    {
        return m_gold;
    }

    bool setGold(int gold);

    long long getExperience() const
    {
        return m_experience;
    }

    bool setExperience(float experience);

    int getHPLevel() const
    {
        return m_hpLevel;
    }

    bool setHPLevel(int level);

    int getHPRestoreLevel() const
    {
        return m_hpRestoreLevel;
    }

    bool setHPRestoreLevel(int level);

    int getArmorLevel() const
    {
        return m_armorLevel;
    }

    bool setArmorLevel(int level);

    int getArmorRepairLevel() const
    {
        return m_armorRepairLevel;
    }

    bool setArmorRepairLevel(int level);

    int getPowerLevel() const
    {
        return m_powerLevel;
    }

    bool setPowerLevel(int level);

    int getPowerRestoreLevel() const
    {
        return m_powerRestoreLevel;
    }

    bool setPowerRestoreLevel(int level);

    int getWeaponLevel() const
    {
        return m_weaponLevel;
    }

    bool setWeaponLevel(int level);

    int getMoverLevel() const
    {
        return m_moverLevel;
    }

    bool setMoverLevel(int level);

    const std::vector<int> &getSkillLevels() const
    {
        return m_skillLevels;
    }

private:
    void configCoverCollideBreath();

    bool checkSkillLevels();

protected:
    int m_gold;
    int m_experience;
    int m_hpLevel;
    int m_hpRestoreLevel;
    int m_armorLevel;
    int m_armorRepairLevel;
    int m_powerLevel;
    int m_powerRestoreLevel;
    int m_weaponLevel;
    int m_moverLevel;
    std::vector<int> m_skillLevels;
};

} // end of namespace bot

#endif
