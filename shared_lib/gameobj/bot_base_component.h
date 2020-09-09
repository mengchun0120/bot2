#ifndef INCLUDE_BOT_BASE_COMPONENT
#define INCLUDE_BOT_BASE_COMPONENT

#include "gametemplate/bot_base_component_template.h"
#include "gameobj/bot_component.h"

namespace bot {

class BaseComponent: public Component {
public:
    BaseComponent(const BaseComponentTemplate* t);

    virtual ~BaseComponent()
    {}

    const BaseComponentTemplate* getTemplate() const
    {
        return static_cast<const BaseComponentTemplate*>(m_template);
    }

    virtual bool update();

    float getHP() const
    {
        return m_hp;
    }

    void restoreHP()
    {
        m_hp = getTemplate()->getHP();
    }

    void addHP(float delta);

    void clearHP()
    {
        m_hp = 0.0f;
    }

    float getArmor() const
    {
        return m_armor;
    }

    void restoreArmor()
    {
        m_armor = getTemplate()->getArmor();
    }

    void addArmor(float delta);

    void clearArmor()
    {
        m_armor = 0.0f;
    }

    float getPower() const
    {
        return m_power;
    }

    void restorePower()
    {
        m_power = getTemplate()->getPower();
    }

    void addPower(float delta);

    void clearPower()
    {
        m_power = 0.0f;
    }

protected:
    float m_hp;
    float m_armor;
    float m_power;
};

} // end of namespace bot

#endif

