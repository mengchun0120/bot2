#ifndef INCLUDE_BOT_SHOOT_SILL
#define INCLUDE_BOT_SHOOT_SILL

#include "skill/bot_skill.h"

namespace bot {

template <typename T> class NamedMap;
class MissileTemplate;

class ShootSkill: public Skill {
public:
    ShootSkill();

    virtual ~ShootSkill();

    bool init(const rapidjson::Value& elem);

    const MissileTemplate* getMissileTemplate() const
    {
        return m_missile;
    }

    virtual bool apply(Robot& robot, GameScreen& screen);

protected:
    const MissileTemplate* m_missile;
};

} // end of namespace bot

#endif

