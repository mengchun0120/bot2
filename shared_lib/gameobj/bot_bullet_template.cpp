#include "gameobj/bot_bullet_template.h"

namespace bot {

BulletTemplate::BulletTemplate()
    : MissileTemplate()
{
}

bool BulletTemplate::init(const rapidjson::Value& elem)
{
    return MissileTemplate::init(MISSILE_BULLET, elem);
}

} // end of namespace bot

