#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "gametemplate/bot_component_template.h"
#include "gametemplate/bot_player_template.h"

namespace bot {

PlayerTemplate::PlayerTemplate()
    : RobotTemplate()
{

}

PlayerTemplate::~PlayerTemplate()
{
}

bool PlayerTemplate::init(const std::string& playerTemplateFile, const NamedMap<ComponentTemplate>& componentLib)
{
    rapidjson::Document doc;
    if (!readJson(doc, playerTemplateFile.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid format in %s", playerTemplateFile.c_str());
        return false;
    }

    const rapidjson::Value& playerJson = doc.GetObject();

    if (!RobotTemplate::init(componentLib, playerJson))
    {
        return false;
    }

    return true;
}

} // end of namespace bot
