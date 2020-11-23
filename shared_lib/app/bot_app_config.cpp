#include "misc/bot_json_utils.h"
#include "misc/bot_file_utils.h"
#include "misc/bot_log.h"
#include "app/bot_app_config.h"

namespace bot {

std::shared_ptr<AppConfig> AppConfig::k_appCfg;

bool AppConfig::initInstance(const std::string& appDir,
                             const std::string& cfgFile)
{
    AppConfig* cfg = new AppConfig();
    k_appCfg.reset(cfg);

    if (!cfg->load(appDir, cfgFile))
    {
        k_appCfg.reset();
        return false;
    }

    return true;
}

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

    std::vector<JsonParamPtr> params =
    {
        jsonParam(m_resDir, "resDir"),
        jsonParam(m_textureDir, "textureDir"),
        jsonParam(m_libDir, "libDir"),
        jsonParam(m_textureLib, "textureLib"),
        jsonParam(m_mapDir, "mapDir"),
        jsonParam(m_glslDir, "glslDir"),
        jsonParam(m_fontDir, "fontDir"),
        jsonParam(m_width, "width"),
        jsonParam(m_height, "height"),
        jsonParam(m_title, "title"),
        jsonParam(m_simpleVertexShaderFile, "simpleVertexShaderFile"),
        jsonParam(m_simpleFragShaderFile, "simpleFragShaderFile"),
        jsonParam(m_particleVertexShaderFile, "particleVertexShaderFile"),
        jsonParam(m_particleFragShaderFile, "particleFragShaderFile"),
        jsonParam(m_eventQueueSize, "eventQueueSize"),
        jsonParam(m_timeDeltaHistoryLen, "timeDeltaHistoryLen"),
        jsonParam(m_textureLib, "textureLib"),
        jsonParam(m_colorLib, "colorLib"),
        jsonParam(m_rectLib, "rectLib"),
        jsonParam(m_tileTemplateLib, "tileTemplateLib"),
        jsonParam(m_missileTemplateLib, "missileTemplateLib"),
        jsonParam(m_goodieTemplateLib, "goodieTemplateLib"),
        jsonParam(m_particleEffectTemplateLib, "particleEffectTemplateLib"),
        jsonParam(m_aiLib, "aiLib"),
        jsonParam(m_baseTemplateLib, "baseTemplateLib"),
        jsonParam(m_weaponTemplateLib, "weaponTemplateLib"),
        jsonParam(m_moverTemplateLib, "moverTemplateLib"),
        jsonParam(m_aiRobotTemplateLib, "aiRobotTemplateLib"),
        jsonParam(m_progressRingLib, "progressRingLib"),
        jsonParam(m_playerTemplate, "playerTemplate"),
        jsonParam(m_dashboardConfigFile, "dashboardConfig"),
        jsonParam(m_buttonConfigFile, "buttonConfig"),
        jsonParam(m_startScreenConfigFile, "startScreenConfig"),
        jsonParam(m_gameScreenConfigFile, "gameScreenConfig"),
        jsonParam(m_messageBoxConfigFile, "messageBoxConfig"),
        jsonParam(m_gameConfigFile, "gameConfig"),
        jsonParam(m_mapFile, "mapFile"),
        jsonParam(m_mapGeneratorLib, "mapGeneratorLib"),
        jsonParam(m_mapPoolFactor, "mapPoolFactor"),
        jsonParam(m_missilePoolSize, "missilePoolSize")
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
    m_simpleVertexShaderFile    = constructPath({ m_glslDir,
                                                  m_simpleVertexShaderFile });
    m_simpleFragShaderFile      = constructPath({ m_glslDir,
                                                  m_simpleFragShaderFile });
    m_particleVertexShaderFile  = constructPath({ m_glslDir,
                                                  m_particleVertexShaderFile });
    m_particleFragShaderFile    = constructPath({ m_glslDir,
                                                  m_particleFragShaderFile });
    m_textureLib                = constructPath({ m_libDir, m_textureLib });
    m_colorLib                  = constructPath({ m_libDir, m_colorLib });
    m_rectLib                   = constructPath({ m_libDir, m_rectLib });
    m_tileTemplateLib           = constructPath({ m_libDir,
                                                  m_tileTemplateLib });
    m_particleEffectTemplateLib = constructPath({
                                                 m_libDir,
                                                 m_particleEffectTemplateLib });
    m_missileTemplateLib        = constructPath({ m_libDir,
                                                  m_missileTemplateLib });
    m_goodieTemplateLib         = constructPath({ m_libDir,
                                                  m_goodieTemplateLib });
    m_aiLib                     = constructPath({ m_libDir, m_aiLib });
    m_baseTemplateLib           = constructPath({ m_libDir,
                                                  m_baseTemplateLib });
    m_weaponTemplateLib         = constructPath({ m_libDir,
                                                  m_weaponTemplateLib });
    m_moverTemplateLib          = constructPath({ m_libDir,
                                                  m_moverTemplateLib });
    m_aiRobotTemplateLib        = constructPath({ m_libDir,
                                                  m_aiRobotTemplateLib });
    m_progressRingLib           = constructPath({ m_libDir,
                                                  m_progressRingLib });
    m_playerTemplate            = constructPath({ m_libDir,
                                                  m_playerTemplate });
    m_dashboardConfigFile       = constructPath({ m_libDir,
                                                  m_dashboardConfigFile });
    m_buttonConfigFile          = constructPath({ m_libDir,
                                                  m_buttonConfigFile });
    m_startScreenConfigFile     = constructPath({ m_libDir,
                                                  m_startScreenConfigFile });
    m_gameScreenConfigFile      = constructPath({ m_libDir,
                                                  im_gameScreenConfigFile });
    m_messageBoxConfigFile      = constructPath({ m_libDir,
                                                  m_messageBoxConfigFile });
    m_gameConfigFile            = constructPath({ m_libDir, m_gameConfigFile });
    m_mapFile                   = constructPath({ m_mapDir, m_mapFile });
    m_mapGeneratorLib           = constructPath({ m_libDir,
                                                  m_mapGeneratorLib });
}

bool AppConfig::setLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid level %d", level);
        return false;
    }

    m_level = level;

    return false;
}

bool AppConfig::setMaxRobotCount(int count)
{
    const int MIN_ROBOT_COUNT = 1;
    const int MAX_ROBOT_COUNT = 200;

    if (count < MIN_ROBOT_COUNT || count > MAX_ROBOT_COUNT)
    {
        LOG_ERROR("Max robot count must be in the range [%d, %d]",
                  MIN_ROBOT_COUNT, MAX_ROBOT_COUNT);
        return false;
    }

    m_maxRobotCount = count;

    return false;
}

} // end of namespace bot
