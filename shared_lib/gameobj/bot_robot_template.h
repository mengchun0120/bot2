#ifndef INCLUDE_BOT_ROBOT_TEMPLATE
#define INCLUDE_BOT_ROBOT_TEMPLATE

#include <vector>
#include "gameobj/bot_game_object_template.h"
#include "skill/bot_skill_template.h"

namespace bot {

class BaseTemplate;
class WeaponTemplate;
class MoverTemplate;

class RobotTemplate: public GameObjectTemplate {
public:
    RobotTemplate();

    virtual ~RobotTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    const BaseTemplate* getBaseTemplate() const
    {
        return m_baseTemplate;
    }

    void setBaseTemplate(const BaseTemplate* baseTemplate)
    {
        m_baseTemplate = baseTemplate;
    }

    const WeaponTemplate* getWeaponTemplate() const
    {
        return m_weaponTemplate;
    }

    void setWeaponTemplate(const WeaponTemplate* weaponTemplate)
    {
        m_weaponTemplate = weaponTemplate;
    }

    const MoverTemplate* getMoverTemplate() const
    {
        return m_moverTemplate;
    }

    void setMoverTemplate(const MoverTemplate* moverTemplate)
    {
        m_moverTemplate = moverTemplate;
    }

    unsigned int numSkills() const
    {
        return m_skillTemplates.size();
    }

    const SkillTemplate* getSkillTemplate(int idx) const
    {
        return m_skillTemplates[idx];
    }

protected:
    bool initSkillTemplates(const std::vector<std::string>& skillNames);

protected:
    const BaseTemplate* m_baseTemplate;
    const WeaponTemplate* m_weaponTemplate;
    const MoverTemplate* m_moverTemplate;
    std::vector<const SkillTemplate*> m_skillTemplates;
};

} // end of namespace bot

#endif
