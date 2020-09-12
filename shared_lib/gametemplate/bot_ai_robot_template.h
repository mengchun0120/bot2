#ifndef INCLUDE_BOT_AI_ROBOT_TEMPLATE
#define INCLUDE_BOT_AI_ROBOT_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "gametemplate/bot_robot_template.h"

namespace bot {

class ComponentTemplate;
class AI;

class AIRobotTemplate : public RobotTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<ComponentTemplate>& componentLib, const NamedMap<AI>& aiLib)
            : m_componentLib(componentLib)
            , m_aiLib(aiLib)
        {}

        ~Parser()
        {}

        AIRobotTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<ComponentTemplate>& m_componentLib;
        const NamedMap<AI>& m_aiLib;
    };

    AIRobotTemplate()
        : RobotTemplate()
        , m_ai(nullptr)
    {}

    bool init(const NamedMap<ComponentTemplate>& componentLib, const NamedMap<AI>& aiLib,
              const rapidjson::Value& elem);

    AI* getAI() const
    {
        return m_ai;
    }

    float getGoodieSpawnProb() const
    {
        return m_goodieSpawnProb;
    }

protected:
    AI* m_ai;
    float m_goodieSpawnProb;
};

} // end of namespace bot

#endif
