#ifndef INCLUDE_BOT_ABILITY_TEMPLATE
#define INCLUDE_BOT_ABILITY_TEMPLATE

#include "gametemplate/bot_ability_type.h"

namespace bot {

class AbilityTemplate {
public:
    AbilityTemplate(AbilityType type)
        : m_type(type)
    {}

    virtual ~AbilityTemplate()
    {}

    AbilityType getType() const
    {
        return m_type;
    }

protected:
    AbilityType m_type;
};

} // end of namespace bot

#endif