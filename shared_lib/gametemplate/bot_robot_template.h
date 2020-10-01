#ifndef INCLUDE_BOT_ROBOT_TEMPLATE
#define INCLUDE_BOT_ROBOT_TEMPLATE

#include "gametemplate/bot_game_object_template.h"

namespace bot {

template <typename T> class NamedMap;
class BaseTemplate;
class WeaponTemplate;
class MoverTemplate;

class RobotTemplate: public GameObjectTemplate {
public:
    RobotTemplate();

    virtual ~RobotTemplate()
    {}

    bool init(const NamedMap<BaseTemplate>& baseLib,
              const NamedMap<WeaponTemplate>& weaponLib,
              const NamedMap<MoverTemplate>& moverLib,
              const rapidjson::Value& elem);

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

protected:
    const BaseTemplate* m_baseTemplate;
    const WeaponTemplate* m_weaponTemplate;
    const MoverTemplate* m_moverTemplate;
};

} // end of namespace bot

#endif
