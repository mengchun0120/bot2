#ifndef INCLUDE_BOT_MOVER_COMPONENT
#define INCLUDE_BOT_MOVER_COMPONENT

#include "gametemplate/bot_mover_component_template.h"
#include "gameobj/bot_component.h"

namespace bot {

class MoverComponent: public Component {
public:
    MoverComponent(const MoverComponentTemplate* t);

    virtual ~MoverComponent()
    {}

    const MoverComponentTemplate* getTemplate() const
    {
        return static_cast<const MoverComponentTemplate*>(m_template);
    }

    virtual void update(GameScreen& screen);

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
        return getTemplate()->getSpeed() * m_speedMultiplier;
    }

    void setSpeedMultiplier(float multiplier)
    {
        m_speedMultiplier = multiplier;
    }

protected:
    bool m_moving;
    float m_speedMultiplier;
};

} // end of namespace bot

#endif

