#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "widget/bot_button_config.h"

namespace bot {

bool ButtonConfig::init(const std::string& configFile, const NamedMap<Texture>& textureLib,
                        const NamedMap<Color>& colorLib)
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

    const rapidjson::Value& val = doc.GetObject();
    std::string textureName, normalColorName, hoverColorName, pressColorName;

    std::vector<JsonParseParam> params = {
        {&textureName,     "texture",         JSONTYPE_STRING},
        {&normalColorName, "normalTextColor", JSONTYPE_STRING},
        {&hoverColorName,  "hoverTextColor",  JSONTYPE_STRING},
        {&pressColorName,  "pressTextColor",   JSONTYPE_STRING}
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    m_texture = textureLib.search(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    m_normalTextColor = colorLib.search(normalColorName);
    if (!m_normalTextColor)
    {
        LOG_ERROR("Failed to find color %s", normalColorName.c_str());
        return false;
    }

    m_hoverTextColor = colorLib.search(hoverColorName);
    if (!m_hoverTextColor)
    {
        LOG_ERROR("Failed to find color %s", hoverColorName.c_str());
        return false;
    }

    m_pressTextColor = colorLib.search(pressColorName);
    if (!m_pressTextColor)
    {
        LOG_ERROR("Failed to find color %s", pressColorName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
