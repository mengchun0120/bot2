#ifndef INCLUDE_BOT_MOVER_TEMPLATE
#define INCLUDE_BOT_MOVER_TEMPLATE

#include <string>
#include "gameobj/bot_single_unit_template.h"

namespace bot {

class MoverTemplate: public SingleUnitTemplate {
public:
    MoverTemplate();

    virtual ~MoverTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    float getSpeed(int level) const;

    bool setSpeed(float speed);

    bool setSpeedPerLevel(float speedPerLevel);

protected:
    float m_speed;
    float m_speedPerLevel;
};

} // end of namespace bot

#endif

