#ifndef INCLUDE_BOT_AI_ROBOT_TEMPLATE
#define INCLUDE_BOT_AI_ROBOT_TEMPLATE

#include <string>
#include "gametemplate/bot_robot_template.h"

namespace bot {

class AI;

class AIRobotTemplate : public RobotTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<BaseTemplate>& baseLib,
               const NamedMap<WeaponTemplate>& weaponLib,
               const NamedMap<MoverTemplate>& moverLib,
               const NamedMap<AI>& aiLib)
            : m_baseLib(baseLib)
            , m_weaponLib(weaponLib)
            , m_moverLib(moverLib)
            , m_aiLib(aiLib)
        {}

        ~Parser()
        {}

        AIRobotTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<BaseTemplate>& m_baseLib;
        const NamedMap<WeaponTemplate>& m_weaponLib;
        const NamedMap<MoverTemplate>& m_moverLib;
        const NamedMap<AI>& m_aiLib;
    };

    AIRobotTemplate();

    virtual ~AIRobotTemplate()
    {}

    bool init(const NamedMap<BaseTemplate>& baseLib,
              const NamedMap<WeaponTemplate>& weaponLib,
              const NamedMap<MoverTemplate>& moverLib,
              const NamedMap<AI>& aiLib,
              const rapidjson::Value& elem);

    AI* getAI() const
    {
        return m_ai;
    }

    void setAI(AI* ai);

    float getGoodieSpawnProb() const
    {
        return m_goodieSpawnProb;
    }

    bool setGoodieSpawnProb(float prob);

protected:
    AI* m_ai;
    float m_goodieSpawnProb;
};

} // end of namespace bot

#endif
