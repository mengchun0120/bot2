#include "misc/bot_json_utils.h"
#include "misc/bot_file_utils.h"
#include "misc/bot_log.h"
#include "app/bot_app_config.h"

namespace bot {

bool AppConfig::load(const std::string& appDir, const std::string& cfgFile)
{
    m_appDir = appDir;

    if (!readConfig(cfgFile))
    {
        return false;
    }

    marshalConfig();

    return true;
}

bool AppConfig::readConfig(const std::string& cfgFile)
{
    rapidjson::Document doc;

    if (!readJson(doc, cfgFile.c_str()))
    {
        LOG_ERROR("Failed to read config from %s", cfgFile.c_str());
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("%s has wrong format", cfgFile.c_str());
        return false;
    }

    rapidjson::Value cfg = doc.GetObject();

    std::vector<JsonParseParam> params =
    {
        {&m_resDir,                    "resDir",                    JSONTYPE_STRING},
        {&m_textureDir,                "textureDir",                JSONTYPE_STRING},
        {&m_libDir,                    "libDir",                    JSONTYPE_STRING},
        {&m_textureLib,                "textureLib",                JSONTYPE_STRING},
        {&m_mapDir,                    "mapDir",                    JSONTYPE_STRING},
        {&m_glslDir,                   "glslDir",                   JSONTYPE_STRING},
        {&m_fontDir,                   "fontDir",                   JSONTYPE_STRING},
        {&m_width,                     "width",                     JSONTYPE_INT},
        {&m_height,                    "height",                    JSONTYPE_INT},
        {&m_title,                     "title",                     JSONTYPE_STRING},
        {&m_simpleVertexShaderFile,    "simpleVertexShaderFile",    JSONTYPE_STRING},
        {&m_simpleFragShaderFile,      "simpleFragShaderFile",      JSONTYPE_STRING},
        {&m_particleVertexShaderFile,  "particleVertexShaderFile",  JSONTYPE_STRING},
        {&m_particleFragShaderFile,    "particleFragShaderFile",    JSONTYPE_STRING},
        {&m_eventQueueSize,            "eventQueueSize",            JSONTYPE_INT},
        {&m_timeDeltaHistoryLen,       "timeDeltaHistoryLen",       JSONTYPE_INT},
        {&m_textureLib,                "textureLib",                JSONTYPE_STRING},
        {&m_colorLib,                  "colorLib",                  JSONTYPE_STRING},
        {&m_rectLib,                   "rectLib",                   JSONTYPE_STRING},
        {&m_tileTemplateLib,           "tileTemplateLib",           JSONTYPE_STRING},
        {&m_missileTemplateLib,        "missileTemplateLib",        JSONTYPE_STRING},
        {&m_goodieTemplateLib,         "goodieTemplateLib",         JSONTYPE_STRING},
        {&m_particleEffectTemplateLib, "particleEffectTemplateLib", JSONTYPE_STRING},
        {&m_aiLib,                     "aiLib",                     JSONTYPE_STRING},
        {&m_aiRobotTemplateLib,        "aiRobotTemplateLib",        JSONTYPE_STRING},
        {&m_progressRingLib,           "progressRingLib",           JSONTYPE_STRING},
        {&m_playerTemplateLib,         "playerTemplateLib",         JSONTYPE_STRING},
        {&m_dashboardConfigFile,       "dashboardConfig",           JSONTYPE_STRING},
        {&m_buttonConfigFile,          "buttonConfig",              JSONTYPE_STRING},
        {&m_startScreenConfigFile,     "startScreenConfig",         JSONTYPE_STRING},
        {&m_messageBoxConfigFile,      "messageBoxConfig",          JSONTYPE_STRING},
        {&m_mapFile,                   "mapFile",                   JSONTYPE_STRING},
        {&m_mapGeneratorLib,           "mapGeneratorLib",           JSONTYPE_STRING},
        {&m_mapPoolFactor,             "mapPoolFactor",             JSONTYPE_FLOAT},
        {&m_missilePoolSize,           "missilePoolSize",           JSONTYPE_INT}
    };

    if (!parseJson(params, cfg))
    {
        return false;
    }

    LOG_INFO("App config read successfully from %s", cfgFile.c_str());

    return true;
}

void AppConfig::marshalConfig()
{
    m_resDir                    = constructPath({ m_appDir, m_resDir });
    m_libDir                    = constructPath({ m_resDir, m_libDir });
    m_mapDir                    = constructPath({ m_resDir, m_mapDir });
    m_glslDir                   = constructPath({ m_resDir, m_glslDir });
    m_fontDir                   = constructPath({ m_resDir, m_fontDir });
    m_textureDir                = constructPath({ m_resDir, m_textureDir });
    m_simpleVertexShaderFile    = constructPath({ m_glslDir, m_simpleVertexShaderFile });
    m_simpleFragShaderFile      = constructPath({ m_glslDir, m_simpleFragShaderFile });
    m_particleVertexShaderFile  = constructPath({ m_glslDir, m_particleVertexShaderFile });
    m_particleFragShaderFile    = constructPath({ m_glslDir, m_particleFragShaderFile });
    m_textureLib                = constructPath({ m_libDir, m_textureLib });
    m_colorLib                  = constructPath({ m_libDir, m_colorLib });
    m_rectLib                   = constructPath({ m_libDir, m_rectLib });
    m_tileTemplateLib           = constructPath({ m_libDir, m_tileTemplateLib });
    m_particleEffectTemplateLib = constructPath({ m_libDir, m_particleEffectTemplateLib });
    m_missileTemplateLib        = constructPath({ m_libDir, m_missileTemplateLib });
    m_goodieTemplateLib         = constructPath({ m_libDir, m_goodieTemplateLib });
    m_aiLib                     = constructPath({ m_libDir, m_aiLib });
    m_aiRobotTemplateLib        = constructPath({ m_libDir, m_aiRobotTemplateLib });
    m_progressRingLib           = constructPath({ m_libDir, m_progressRingLib });
    m_playerTemplateLib         = constructPath({ m_libDir, m_playerTemplateLib });
    m_dashboardConfigFile       = constructPath({ m_libDir, m_dashboardConfigFile });
    m_buttonConfigFile          = constructPath({ m_libDir, m_buttonConfigFile });
    m_startScreenConfigFile     = constructPath({ m_libDir, m_startScreenConfigFile });
    m_messageBoxConfigFile      = constructPath({ m_libDir, m_messageBoxConfigFile });
    m_mapFile                   = constructPath({ m_mapDir, m_mapFile });
    m_mapGeneratorLib           = constructPath({ m_libDir, m_mapGeneratorLib });
}

} // end of namespace bot
