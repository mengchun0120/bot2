#ifndef INCLUDE_BOT_MOVER_TEMPLATE
#define INCLUDE_BOT_MOVER_TEMPLATE

#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class MoverTemplate: public SingleUnitTemplate {
public:
    MoverTemplate();

    virtual ~MoverTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    float getSpeed(int level) const
    {
        return m_speed + m_speedPerLevel * (level - 1);
    }

    bool setSpeed(float speed);

    bool setSpeedPerLevel(float speedPerLevel);

protected:
    float m_speed;
    float m_speedPerLevel;
};

} // end of namespace bot

#endif

