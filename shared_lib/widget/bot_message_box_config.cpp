#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "widget/bot_message_box_config.h"
#include "app/bot_app.h"

namespace bot {

MessageBoxConfig::MessageBoxConfig()
    : m_boxFillColor(nullptr)
    , m_boxBorderColor(nullptr)
    , m_textColor(nullptr)
{
}

bool MessageBoxConfig::init(const std::string &configFile,
                            const NamedMap<Color> &colorLib)
{
    rapidjson::Document doc;

    if (!readJson(doc, configFile.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid file format %s", configFile.c_str());
        return false;
    }

    const rapidjson::Value &val = doc.GetObject();
    std::string boxFillColorName, boxBorderColorName, textColorName;
    std::vector<JsonParamPtr> params = {
        jsonParam(boxFillColorName, "boxFillColor"),
        jsonParam(boxBorderColorName, "boxBorderColor"),
        jsonParam(textColorName, "textColor")
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    m_boxFillColor = colorLib.search(boxFillColorName);
    if (!m_boxFillColor)
    {
        LOG_ERROR("Failed to find color %s", boxFillColorName.c_str());
        return false;
    }

    m_textColor = colorLib.search(textColorName);
    if (!m_textColor)
    {
        LOG_ERROR("Failed to find color %s", textColorName.c_str());
        return false;
    }

    m_boxBorderColor = colorLib.search(boxBorderColorName);
    if (!m_boxBorderColor)
    {
        LOG_ERROR("Failed to find color %s", boxBorderColorName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
