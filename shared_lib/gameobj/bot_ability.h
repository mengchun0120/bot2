#ifndef INCLUDE_BOT_ABILITY
#define INCLUDE_BOT_ABILITY

#include "gametemplate/bot_ability_template.h"

namespace bot {

class Ability {
public:
    Ability(const AbilityTemplate* t)
        : m_template(t)
    {}

    virtual ~Ability()
    {}

    AbilityType getType() const
    {
        return m_template->getType();
    }

    const AbilityTemplate* getTemplate() const
    {
        return m_template;
    }

protected:
    const AbilityTemplate* m_template;
};

} // end of namespace bot

#endif
