#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_config.h"

namespace bot {

bool GameConfig::load(const std::string& fileName)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid file format %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& val = doc.GetObject();
    std::vector<JsonParamPtr> params = {
        jsonParam(m_dissolveTimeMS, "dissolveTimeMS",
                  gt(m_dissolveTimeMS, 0.0f))
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

