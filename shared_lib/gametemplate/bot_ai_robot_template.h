#ifndef INCLUDE_BOT_AI_ROBOT_TEMPLATE
#define INCLUDE_BOT_AI_ROBOT_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "gametemplate/bot_robot_template.h"

namespace bot {

class AI;

class AIRobotTemplate : public RobotTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
               const NamedMap<Color>& colorLib, const NamedMap<MissileTemplate>& missileLib,
               const NamedMap<AI>& aiLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
            , m_colorLib(colorLib)
            , m_missileLib(missileLib)
            , m_aiLib(aiLib)
        {}

        ~Parser()
        {}

        AIRobotTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
        const NamedMap<Color>& m_colorLib;
        const NamedMap<MissileTemplate>& m_missileLib;
        const NamedMap<AI>& m_aiLib;
    };

    static AIRobotTemplate* create(const rapidjson::Value& elem);

    AIRobotTemplate()
        : m_ai(nullptr)
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const NamedMap<Color>& colorLib, const NamedMap<MissileTemplate>& missileLib,
              const NamedMap<AI>& aiLib, const rapidjson::Value& elem);

    AI* getAI() const
    {
        return m_ai;
    }

    void setAI(AI* ai)
    {
        m_ai = ai;
    }

protected:
    AI* m_ai;
};

} // end of namespace bot

#endif
