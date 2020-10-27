#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"
#include "app/bot_app_config.h"

namespace bot {

std::shared_ptr<GameLib> GameLib::k_gameLib;

bool GameLib::initInstance(float viewportWidth, float viewportHeight,
                           const AppConfig& cfg)
{
    GameLib* lib = new GameLib();

    if (!lib->load(viewportWidth, viewportHeight, cfg))
    {
        delete lib;
        return false;
    }

    k_gameLib.reset(lib);

    return true;
}

bool GameLib::load(float viewportWidth, float viewportHeight,
                   const AppConfig& cfg)
{
    Texture::Parser textureParser(cfg.getTextureDir());
    bool ret = m_textureLib.load(cfg.getTextureLib().c_str(), textureParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read texture lib from %s",
                  cfg.getTextureLib().c_str());
        return false;
    }
    LOG_INFO("Done loading texture library from %s",
             cfg.getTextureLib().c_str());

    Rectangle::Parser rectParser;
    ret = m_rectLib.load(cfg.getRectLib().c_str(), rectParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read rect lib from %s",
                  cfg.getRectLib().c_str());
        return false;
    }
    LOG_INFO("Done loading rectangle library from %s",
             cfg.getRectLib().c_str());

    Color::Parser colorParser;
    ret = m_colorLib.load(cfg.getColorLib().c_str(), colorParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read color lib from %s",
                  cfg.getColorLib().c_str());
        return false;
    }
    LOG_INFO("Done loading color library from %s",
             cfg.getColorLib().c_str());

    TileTemplate::Parser tileTemplateParser(m_textureLib, m_rectLib);
    ret = m_tileTemplateLib.load(cfg.getTileTemplateLib().c_str(),
                                 tileTemplateParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read tile template lib from %s",
                  cfg.getTileTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading tile template library from %s",
             cfg.getTileTemplateLib().c_str());

    ParticleEffectTemplate::Parser particleEffectParser(m_textureLib,
                                                        m_colorLib);
    ret = m_particleEffectTemplateLib.load(
                            cfg.getParticleEffectTemplateLib().c_str(),
                            particleEffectParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read particle effect lib from %s",
                  cfg.getParticleEffectTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading particle effect lib from %s",
             cfg.getParticleEffectTemplateLib().c_str());

    MissileTemplate::Parser missileParser(m_textureLib, m_rectLib,
                                          m_particleEffectTemplateLib,
                                          m_colorLib);
    ret = m_missileTemplateLib.load(cfg.getMissileTemplateLib().c_str(),
                                    missileParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read missile template lib from %s",
                  cfg.getMissileTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading missile template library from %s",
             cfg.getMissileTemplateLib().c_str());

    ProgressRing::Parser progressRingParser(m_colorLib);
    ret = m_progressRingLib.load(cfg.getProgressRingLib().c_str(),
                                 progressRingParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read progress ring from %s",
                  cfg.getProgressRingLib().c_str());
        return false;
    }
    LOG_INFO("Done loading progress ring library from %s",
             cfg.getProgressRingLib().c_str());

    GoodieTemplate::Parser goodieParser(m_rectLib, m_textureLib,
                                        m_colorLib, m_progressRingLib);
    ret = m_goodieTemplateLib.load(cfg.getGoodieTemplateLib().c_str(),
                                   goodieParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read goodie template lib from %s",
                  cfg.getGoodieTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading goodie template lib from %s",
             cfg.getGoodieTemplateLib().c_str());

    AI::Parser aiParser;
    ret = m_aiLib.load(cfg.getAILib().c_str(), aiParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read AI from %s", cfg.getAILib().c_str());
        return false;
    }
    LOG_INFO("Done loading ai library from %s", cfg.getAILib().c_str());

    BaseTemplate::Parser baseParser(m_textureLib, m_rectLib, m_colorLib);
    ret = m_baseTemplateLib.load(cfg.getBaseTemplateLib().c_str(), baseParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read base-template library from %s",
                  cfg.getBaseTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading base-template library from %s",
             cfg.getBaseTemplateLib().c_str());

    WeaponTemplate::Parser weaponParser(m_textureLib, m_rectLib,
                                        m_missileTemplateLib);
    ret = m_weaponTemplateLib.load(cfg.getWeaponTemplateLib().c_str(),
                                   weaponParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read weapon-template library from %s",
                  cfg.getWeaponTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading weapon-template library from %s",
             cfg.getWeaponTemplateLib().c_str());

    MoverTemplate::Parser moverParser(m_textureLib, m_rectLib);
    ret = m_moverTemplateLib.load(cfg.getMoverTemplateLib().c_str(),
                                  moverParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read mover-template library from %s",
                  cfg.getMoverTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading mover-template library from %s",
             cfg.getMoverTemplateLib().c_str());

    AIRobotTemplate::Parser aiRobotParser(m_baseTemplateLib,
                                          m_weaponTemplateLib,
                                          m_moverTemplateLib,
                                          m_aiLib);
    ret = m_aiRobotTemplateLib.load(cfg.getAIRobotTemplateLib().c_str(),
                                    aiRobotParser);
    if (!ret)
    {
        LOG_ERROR("Failed to read ai-robot template lib from %s",
                  cfg.getAIRobotTemplateLib().c_str());
        return false;
    }
    LOG_INFO("Done loading ai-robot template library from %s",
             cfg.getAIRobotTemplateLib().c_str());

    ret = m_playerTemplate.init(cfg.getPlayerTemplate(),
                                m_textureLib, m_rectLib,
                                m_particleEffectTemplateLib, m_colorLib);
    if (!ret)
    {
        LOG_ERROR("Failed to read player template from %s",
                  cfg.getPlayerTemplate().c_str())
        return false;
    }
    LOG_INFO("Done reading player template from %s",
             cfg.getPlayerTemplate().c_str());

    MapGenerator::Parser mapGeneratorParser(&m_playerTemplate,
                                            m_tileTemplateLib,
                                            m_aiRobotTemplateLib,
                                            cfg.getMaxRobotCount());
    ret = m_mapGeneratorLib.load(cfg.getMapGeneratorLib().c_str(),
                                 mapGeneratorParser);
    if (!ret)
    {
        LOG_ERROR("Failed to load map-generator lib from %s",
                  cfg.getMapGeneratorLib().c_str());
        return false;
    }
    LOG_INFO("Done loading map-generator lib from %s",
             cfg.getMapGeneratorLib().c_str());

    ret = m_dashboardConfig.init(cfg.getDashboardConfigFile());
    if (!ret)
    {
        LOG_ERROR("Failed to load dashboard config from %s",
                  cfg.getDashboardConfigFile().c_str());
        return false;
    }
    LOG_INFO("Done reading dashboard config from %s",
             cfg.getDashboardConfigFile().c_str());

    ret = m_buttonConfig.init(cfg.getButtonConfigFile(),
                              m_textureLib, m_colorLib);
    if (!ret)
    {
        LOG_ERROR("Failed to load button config from %s",
                  cfg.getButtonConfigFile().c_str());
        return false;
    }
    LOG_INFO("Done reading button config from %s",
             cfg.getButtonConfigFile().c_str());

    ret = m_messageBoxConfig.init(cfg.getMessageBoxConfigFile(), viewportWidth,
                                  viewportHeight, m_rectLib, m_colorLib);
    if (!ret)
    {
        LOG_ERROR("Failed to load message-box config from %s",
                  cfg.getMessageBoxConfigFile().c_str());
        return false;
    }
    LOG_INFO("Done loading message-box config from %s",
             cfg.getMessageBoxConfigFile().c_str());

    ret = m_startScreenConfig.init(cfg.getStartScreenConfigFile(), m_rectLib);
    if (!ret)
    {
        LOG_ERROR("Failed to load start-screen config from %s",
                  cfg.getStartScreenConfigFile().c_str());
        return false;
    }
    LOG_INFO("Done loading start-screen config from %s",
             cfg.getStartScreenConfigFile().c_str());

    return true;
}

} // end of namespace bot
