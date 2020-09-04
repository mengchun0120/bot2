#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gameobj/bot_dashboard_config.h"

namespace bot {

bool DashboardConfig::init(const std::string& configFile, const NamedMap<Texture>& textureLib,
                           const NamedMap<Rectangle>& rectLib, const NamedMap<Color>& colorLib)
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

    std::string hpRectName, hpTextureName;
    std::string hpGoodColorName, hpBadColorName, hpCriticalColorName;
    std::string goldRectName, goldTextureName, goldTextColorName;

    std::vector<JsonParseParam> params = {
        {&hpRectName,            "hpRect",             JSONTYPE_STRING},
        {&hpTextureName,         "hpTexture",          JSONTYPE_STRING},
        {&hpGoodColorName,       "hpGoodColor",        JSONTYPE_STRING},
        {&hpBadColorName,        "hpBadColor",         JSONTYPE_STRING},
        {&hpCriticalColorName,   "hpCriticalColor",    JSONTYPE_STRING},
        {&m_hpIconX,             "hpIconX",            JSONTYPE_FLOAT},
        {&m_hpTextX,             "hpTextX",            JSONTYPE_FLOAT},
        {&goldRectName,          "goldRect",           JSONTYPE_STRING},
        {&goldTextureName,       "goldTexture",        JSONTYPE_STRING},
        {&goldTextColorName,     "goldTextColor",      JSONTYPE_STRING},
        {&m_goldIconX,           "goldIconX",          JSONTYPE_FLOAT},
        {&m_goldTextX,           "goldTextX",          JSONTYPE_FLOAT},
        {&m_effectSpacingX,      "effectSpacingX",     JSONTYPE_FLOAT},
        {&m_effectStartX,        "effectStartX",       JSONTYPE_FLOAT},
        {&m_effectRingRadius,    "effectRingRadius",   JSONTYPE_FLOAT},
        {&m_headerTopMargin,     "headerTopMargin",    JSONTYPE_FLOAT}
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    m_hpRect = rectLib.search(hpRectName);
    if (!m_hpRect)
    {
        LOG_ERROR("Failed to find hp rect %s", hpRectName.c_str());
        return false;
    }

    m_hpTexture = textureLib.search(hpTextureName);
    if (!m_hpTexture)
    {
        LOG_ERROR("Failed to find hp texture %s", hpTextureName.c_str());
        return false;
    }

    m_hpGoodColor = colorLib.search(hpGoodColorName);
    if (!m_hpGoodColor)
    {
        LOG_ERROR("Failed to find hp good color %s", hpGoodColorName.c_str());
        return false;
    }

    m_hpBadColor = colorLib.search(hpBadColorName);
    if (!m_hpBadColor)
    {
        LOG_ERROR("Failed to find hp bad color %s", hpBadColorName.c_str());
        return false;
    }

    m_hpCriticalColor = colorLib.search(hpCriticalColorName);
    if (!m_hpCriticalColor)
    {
        LOG_ERROR("Failed to find hp critical color %s", hpCriticalColorName.c_str());
        return false;
    }

    m_goldRect = rectLib.search(goldRectName);
    if (!m_goldRect)
    {
        LOG_ERROR("Failed to find gold rect %s", goldRectName.c_str());
        return false;
    }

    m_goldTexture = textureLib.search(goldTextureName);
    if (!m_goldTexture)
    {
        LOG_ERROR("Failed to find gold texture %s", goldTextureName.c_str());
        return false;
    }

    m_goldTextColor = colorLib.search(goldTextColorName);
    if (!m_goldTextColor)
    {
        LOG_ERROR("Failed to find gold text color %s", goldTextColorName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
