#ifndef INCLUDE_BULLET_TEMPLATE
#define INCLUDE_BULLET_TEMPLATE

#include "gameobj/bot_missile_template.h"

namespace bot {

class BulletTemplate: public MissileTemplate {
public:
    BulletTemplate();

    virtual ~BulletTemplate()
    {}

    bool init(const rapidjson::Value& elem);
};

} // end of namespace bot

#endif

