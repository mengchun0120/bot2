#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "ai/bot_ai.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_ai_robot_template.h"

namespace bot {

AIRobotTemplate* AIRobotTemplate::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    AIRobotTemplate* t = new AIRobotTemplate();
    if (!t->init(m_textureLib, m_rectLib, m_colorLib, m_missileLib, m_aiLib, elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

bool AIRobotTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                           const NamedMap<Color>& colorLib, const NamedMap<MissileTemplate>& missileLib,
                           const NamedMap<AI>& aiLib, const rapidjson::Value& elem)
{
    if (!RobotTemplate::init(textureLib, rectLib, colorLib, missileLib, elem))
    {
        return false;
    }

    std::string aiName;

    if (!parseJson(aiName, elem, "ai"))
    {
        return false;
    }

    m_ai = aiLib.search(aiName);
    if (!m_ai)
    {
        LOG_ERROR("Failed to find AI %s", aiName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
