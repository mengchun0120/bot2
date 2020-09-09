#ifndef INCLUDE_BOT_COMPONENT
#define INCLUDE_BOT_COMPONENT

#include "misc/bot_constants.h"

namespace bot {

class Graphics;

class Component {
public:
    Component(const ComponentTemplate* t)
        : m_template(t)
    {}

    virtual ~Component()
    {}

    void present(Graphics& g, const float* pos, const float* direction);

    virtual bool update() = 0;

protected:
    const ComponentTemplate* m_template;
};

} // end of namespace bot

#endif

