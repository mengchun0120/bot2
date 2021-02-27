#ifndef INCLUDE_BOT_SHELL_TEMPLATE
#define INCLUDE_BOT_SHELL_TEMPLATE

#include "gameobj/bot_missile_template.h"

namespace bot {

class ShellTemplate: public MissileTemplate {
public:
    ShellTemplate();

    virtual ~ShellTemplate()
    {}

    bool init(const rapidjson::Value &elem);

    float getExplodeBreath() const
    {
        return m_explodeBreath;
    }

protected:
    float m_explodeBreath;
};

} // end of namespace bot

#endif

