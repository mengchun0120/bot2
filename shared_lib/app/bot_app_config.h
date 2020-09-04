#ifndef INCLUDE_BOT_APP_CONFIG
#define INCLUDE_BOT_APP_CONFIG

#include <string>

namespace bot {

class AppConfig {
public:
    AppConfig()
    {}

    ~AppConfig()
    {}

    bool load(const std::string& appDir, const std::string& cfgFile);

    const std::string& getResDir() const
    {
        return m_resDir;
    }

    int getWidth() const
    {
        return m_width;
    }

    int getHeight() const
    {
        return m_height;
    }

    const std::string& getTitle() const
    {
        return m_title;
    }

    const std::string& getGlslDir() const
    {
        return m_glslDir;
    }

    const std::string& getSimpleVertexShaderFile() const
    {
        return m_simpleVertexShaderFile;
    }

    const std::string& getSimpleFragShaderFile() const
    {
        return m_simpleFragShaderFile;
    }

    const std::string& getParticleVertexShaderFile() const
    {
        return m_particleVertexShaderFile;
    }

    const std::string& getParticleFragShaderFile() const
    {
        return m_particleFragShaderFile;
    }

    int getEventQueueSize() const
    {
        return m_eventQueueSize;
    }

    int getTimeDeltaHistoryLen() const
    {
        return m_timeDeltaHistoryLen;
    }

    const std::string& getFontDir() const
    {
        return m_fontDir;
    }

    const std::string& getTextureDir() const
    {
        return m_textureDir;
    }

    const std::string& getLibDir() const
    {
        return m_libDir;
    }

    const std::string& getMapDir() const
    {
        return m_mapDir;
    }

    const std::string& getTextureLib() const
    {
        return m_textureLib;
    }

    const std::string& getColorLib() const
    {
        return m_colorLib;
    }

    const std::string& getRectLib() const
    {
        return m_rectLib;
    }

    const std::string& getTileTemplateLib() const
    {
        return m_tileTemplateLib;
    }

    const std::string& getParticleEffectTemplateLib() const
    {
        return m_particleEffectTemplateLib;
    }

    const std::string& getMissileTemplateLib() const
    {
        return m_missileTemplateLib;
    }

    const std::string& getGoodieTemplateLib() const
    {
        return m_goodieTemplateLib;
    }

    const std::string& getAILib() const
    {
        return m_aiLib;
    }

    const std::string& getAIRobotTemplateLib() const
    {
        return m_aiRobotTemplateLib;
    }

    const std::string& getProgressRingLib() const
    {
        return m_progressRingLib;
    }

    const std::string& getPlayerTemplateLib() const
    {
        return m_playerTemplateLib;
    }

    const std::string& getDashboardConfigFile() const
    {
        return m_dashboardConfigFile;
    }

    const std::string& getMapFile() const
    {
        return m_mapFile;
    }

    float getMapPoolFactor() const
    {
        return m_mapPoolFactor;
    }

    int getMissilePoolSize() const
    {
        return m_missilePoolSize;
    }

    const std::string& getButtonConfigFile() const
    {
        return m_buttonConfigFile;
    }

    const std::string& getStartScreenConfigFile() const
    {
        return m_startScreenConfigFile;
    }

    const std::string& getMessageBoxConfigFile() const
    {
        return m_messageBoxConfigFile;
    }

    const std::string& getMapGeneratorLib() const
    {
        return m_mapGeneratorLib;
    }

private:
    bool readConfig(const std::string& cfgFile);

    void marshalConfig();

private:
    std::string m_appDir;
    std::string m_resDir;
    std::string m_libDir;
    std::string m_mapDir;
    std::string m_glslDir;
    std::string m_fontDir;
    std::string m_textureDir;
    int m_width;
    int m_height;
    std::string m_title;
    std::string m_simpleVertexShaderFile;
    std::string m_simpleFragShaderFile;
    std::string m_particleVertexShaderFile;
    std::string m_particleFragShaderFile;
    int m_eventQueueSize;
    int m_timeDeltaHistoryLen;
    std::string m_textureLib;
    std::string m_colorLib;
    std::string m_rectLib;
    std::string m_tileTemplateLib;
    std::string m_particleEffectTemplateLib;
    std::string m_missileTemplateLib;
    std::string m_goodieTemplateLib;
    std::string m_aiLib;
    std::string m_aiRobotTemplateLib;
    std::string m_progressRingLib;
    std::string m_playerTemplateLib;
    std::string m_dashboardConfigFile;
    std::string m_buttonConfigFile;
    std::string m_startScreenConfigFile;
    std::string m_messageBoxConfigFile;
    std::string m_mapFile;
    std::string m_mapGeneratorLib;
    float m_mapPoolFactor;
    int m_missilePoolSize;
};

} // end of namespace bot

#endif
