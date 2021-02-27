#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

AIRobotTemplate::AIRobotTemplate()
    : m_ai(nullptr)
    , m_goodieSpawnProb(0.0f)
{
}

bool AIRobotTemplate::init(const rapidjson::Value &elem)
{
    if (!RobotTemplate::init(elem))
    {
        LOG_ERROR("Failed to initialize RobotTemplate");
        return false;
    }

    float goodieSpawnProb;
    if (!JsonParser::parse(goodieSpawnProb, elem, "goodieSpawnProb"))
    {
        LOG_ERROR("Failed to parse goodieSpawnProb");
        return false;
    }

    if (!setGoodieSpawnProb(goodieSpawnProb))
    {
        return false;
    }

    std::string aiName;
    if (!JsonParser::parse(aiName, elem, "ai"))
    {
        return false;
    }

    GameLib &lib = GameLib::getInstance();

    m_ai = lib.getAI(aiName);
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
