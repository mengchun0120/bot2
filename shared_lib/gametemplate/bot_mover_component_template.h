#ifndef INCLUDE_BOT_MOVER_COMPONENT_TEMPLATE
#define INCLUDE_BOT_MOVER_COMPONENT_TEMPLATE

#include "gametemplate/bot_component_template.h"

namespace bot {

class MoverComponentTemplate: public ComponentTemplate {
public:
    MoverComponentTemplate()
        : ComponentTemplate(COMPONENT_MOVER)
        , m_speed(0.0f)
    {}

    virtual ~MoverComponentTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    float getSpeed() const
    {
        return m_speed;
    }

protected:
    float m_speed;
};

} // end of namespace bot

#endif

