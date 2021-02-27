#ifndef INCLUDE_BOT_SKILL_TEMPLATE
#define INCLUDE_BOT_SKILL_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "skill/bot_skill_type.h"
#include "skill/bot_skill_flag.h"

namespace bot {

class SkillTemplate {
public:
    static SkillTemplate *create(const std::string &name,
                                 const rapidjson::Value &elem);

    SkillTemplate();

    virtual ~SkillTemplate()
    {}

    bool init(SkillType type, const rapidjson::Value &elem);

    SkillType getType() const
    {
        return m_type;
    }

    int getFlags() const
    {
        return m_flags;
    }

    bool testFlags(int flags) const
    {
        return (flags & m_flags) != 0;
    }

    bool isOffensive() const
    {
        return testFlags(SKILL_FLAG_OFFENSIVE);
    }

    bool isRange() const
    {
        return testFlags(SKILL_FLAG_RANGE);
    }

    const std::string &getViewName() const
    {
        return m_viewName;
    }

    const std::string &getDescription() const
    {
        return m_description;
    }

    float getPowerCost() const
    {
        return m_powerCost;
    }

    float getCooldownMS() const
    {
        return m_cooldownMS;
    }

protected:
    SkillType m_type;
    int m_flags;
    std::string m_viewName;
    std::string m_description;
    float m_powerCost;
    float m_cooldownMS;
};

} // end of namespace bot

#endif

