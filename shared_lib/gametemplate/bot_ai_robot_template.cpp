#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "gametemplate/bot_base_template.h"
#include "gametemplate/bot_weapon_template.h"
#include "gametemplate/bot_mover_template.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "ai/bot_ai.h"

namespace bot {

AIRobotTemplate* AIRobotTemplate::Parser::create(const std::string& name,
                                                 const rapidjson::Value& elem)
{
    AIRobotTemplate* t = new AIRobotTemplate();
    if (!t->init(m_baseLib, m_weaponLib, m_moverLib, m_aiLib, elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

AIRobotTemplate::AIRobotTemplate()
    : m_ai(nullptr)
    , m_goodieSpawnProb(0.0f)
{
}

bool AIRobotTemplate::init(const NamedMap<BaseTemplate>& baseLib,
                           const NamedMap<WeaponTemplate>& weaponLib,
                           const NamedMap<MoverTemplate>& moverLib,
                           const NamedMap<AI>& aiLib,
                           const rapidjson::Value& elem)
{
    if (!RobotTemplate::init(baseLib, weaponLib, moverLib, elem))
    {
        LOG_ERROR("Failed to initialize RobotTemplate");
        return false;
    }

    float goodieSpawnProb;
    if (!parseJson(goodieSpawnProb, elem, "goodieSpawnProb"))
    {
        LOG_ERROR("Failed to parse goodieSpawnProb");
        return false;
    }

    if (!setGoodieSpawnProb(goodieSpawnProb))
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

bool AIRobotTemplate::setGoodieSpawnProb(float prob)
{
    if (prob < 0.0f || prob > 1.0f)
    {
        LOG_ERROR("Invalid goodie-spawn-prob %f", prob);
        return false;
    }

    m_goodieSpawnProb = prob;
    return true;
}

} // end of namespace bot
