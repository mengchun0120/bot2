#ifndef INCLUDE_BOT_GAME_LIB
#define INCLUDE_BOT_GAME_LIB

#include <memory>
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "ai/bot_ai.h"
#include "gameobj/bot_tile_template.h"
#include "gameobj/bot_missile_template.h"
#include "gameobj/bot_goodie_template.h"
#include "gameobj/bot_ai_robot_template.h"
#include "gameobj/bot_particle_effect_template.h"
#include "gameobj/bot_base_template.h"
#include "gameobj/bot_weapon_template.h"
#include "gameobj/bot_mover_template.h"
#include "gameobj/bot_player_template.h"
#include "gameutil/bot_map_generator.h"
#include "gameutil/bot_game_config.h"
#include "skill/bot_skill_template.h"
#include "widget/bot_progress_bar_template.h"
#include "widget/bot_status_bar_template.h"
#include "widget/bot_progress_ring.h"
#include "widget/bot_dashboard_config.h"
#include "widget/bot_button_config.h"
#include "widget/bot_message_box_config.h"
#include "screen/bot_start_screen_config.h"
#include "screen/bot_game_screen_config.h"

namespace bot {

class GameLib {
public:
    static bool initInstance();

    static GameLib& getInstance()
    {
        return *k_gameLib;
    }

    ~GameLib()
    {}

    const Texture* getTexture(const std::string& name) const
    {
        return m_textureLib.search(name);
    }

    const NamedMap<Texture>& getTextureLib() const
    {
        return m_textureLib;
    }

    const Rectangle* getRect(const std::string& name) const
    {
        return m_rectLib.search(name);
    }

    const NamedMap<Rectangle>& getRectLib() const
    {
        return m_rectLib;
    }

    const Color* getColor(const std::string& name) const
    {
        return m_colorLib.search(name);
    }

    const NamedMap<Color>& getColorLib() const
    {
        return m_colorLib;
    }

    const TileTemplate* getTileTemplate(const std::string& name) const
    {
        return m_tileTemplateLib.search(name);
    }

    const NamedMap<TileTemplate>& getTileTemplateLib() const
    {
        return m_tileTemplateLib;
    }

    const ParticleEffectTemplate* getParticleEffectTemplate(
                                            const std::string& name) const
    {
        return m_particleEffectTemplateLib.search(name);
    }

    const NamedMap<ParticleEffectTemplate>& getParticleEffectTemplateLib() const
    {
        return m_particleEffectTemplateLib;
    }

    const MissileTemplate* getMissileTemplate(const std::string& name) const
    {
        return m_missileTemplateLib.search(name);
    }

    const NamedMap<MissileTemplate>& getMissileTemplateLib() const
    {
        return m_missileTemplateLib;
    }

    const GoodieTemplate* getGoodieTemplate(const std::string& name) const
    {
        return m_goodieTemplateLib.search(name);
    }

    const NamedMap<GoodieTemplate>& getGoodieTemplateLib() const
    {
        return m_goodieTemplateLib;
    }

    const ProgressRing* getProgressRing(const std::string& name) const
    {
        return m_progressRingLib.search(name);
    }

    const NamedMap<ProgressRing>& getProgressRingLib() const
    {
        return m_progressRingLib;
    }

    const AI* getAI(const std::string& name) const
    {
        return m_aiLib.search(name);
    }

    AI* getAI(const std::string& name)
    {
        return m_aiLib.search(name);
    }

    const NamedMap<AI>& getAILib() const
    {
        return m_aiLib;
    }

    const BaseTemplate* getBaseTemplate(const std::string& name) const
    {
        return m_baseTemplateLib.search(name);
    }

    const NamedMap<BaseTemplate>& getBaseTemplateLib() const
    {
        return m_baseTemplateLib;
    }

    const WeaponTemplate* getWeaponTemplate(const std::string& name) const
    {
        return m_weaponTemplateLib.search(name);
    }

    const NamedMap<WeaponTemplate>& getWeaponTemplateLib() const
    {
        return m_weaponTemplateLib;
    }

    const MoverTemplate* getMoverTemplate(const std::string& name) const
    {
        return m_moverTemplateLib.search(name);
    }

    const NamedMap<MoverTemplate>& getMoverTemplateLib() const
    {
        return m_moverTemplateLib;
    }

    const AIRobotTemplate* getAIRobotTemplate(const std::string& name) const
    {
        return m_aiRobotTemplateLib.search(name);
    }

    const NamedMap<AIRobotTemplate>& getAIRobotTemplateLib() const
    {
        return m_aiRobotTemplateLib;
    }

    const ProgressBarTemplate* getProgressBarTemplate(
                                           const std::string& name) const
    {
        return m_progressBarTemplateLib.search(name);
    }

    const NamedMap<ProgressBarTemplate>& getProgressBarTemplateLib() const
    {
        return m_progressBarTemplateLib;
    }

    const StatusBarTemplate* getStatusBarTemplate(const std::string& name) const
    {
        return m_statusBarTemplateLib.search(name);
    }

    const NamedMap<StatusBarTemplate>& getStatusBarTemplateLib() const
    {
        return m_statusBarTemplateLib;
    }

    const PlayerTemplate& getPlayerTemplate() const
    {
        return m_playerTemplate;
    }

    const DashboardConfig& getDashboardConfig() const
    {
        return m_dashboardConfig;
    }

    const ButtonConfig& getButtonConfig() const
    {
        return m_buttonConfig;
    }

    const MessageBoxConfig& getMessageBoxConfig() const
    {
        return m_messageBoxConfig;
    }

    const StartScreenConfig& getStartScreenConfig() const
    {
        return m_startScreenConfig;
    }

    const GameScreenConfig& getGameScreenConfig() const
    {
        return m_gameScreenConfig;
    }

    const GameConfig& getGameConfig() const
    {
        return m_gameConfig;
    }

    const SkillTemplate* getSkillTemplate(const std::string& name) const
    {
        return m_skillTemplateLib.search(name);
    }

    MapGenerator* getMapGenerator(const std::string& name)
    {
        return m_mapGeneratorLib.search(name);
    }

    const NamedMap<MapGenerator>& getMapGeneratorLib() const
    {
        return m_mapGeneratorLib;
    }

private:
    GameLib()
    {}

    bool load();

private:
    static std::shared_ptr<GameLib> k_gameLib;
    NamedMap<Texture> m_textureLib;
    NamedMap<Rectangle> m_rectLib;
    NamedMap<Color> m_colorLib;
    NamedMap<TileTemplate> m_tileTemplateLib;
    NamedMap<ParticleEffectTemplate> m_particleEffectTemplateLib;
    NamedMap<MissileTemplate> m_missileTemplateLib;
    NamedMap<GoodieTemplate> m_goodieTemplateLib;
    NamedMap<AI> m_aiLib;
    NamedMap<BaseTemplate> m_baseTemplateLib;
    NamedMap<WeaponTemplate> m_weaponTemplateLib;
    NamedMap<MoverTemplate> m_moverTemplateLib;
    NamedMap<AIRobotTemplate> m_aiRobotTemplateLib;
    NamedMap<ProgressRing> m_progressRingLib;
    PlayerTemplate m_playerTemplate;
    NamedMap<MapGenerator> m_mapGeneratorLib;
    NamedMap<ProgressBarTemplate> m_progressBarTemplateLib;
    NamedMap<StatusBarTemplate> m_statusBarTemplateLib;
    DashboardConfig m_dashboardConfig;
    ButtonConfig m_buttonConfig;
    MessageBoxConfig m_messageBoxConfig;
    StartScreenConfig m_startScreenConfig;
    GameScreenConfig m_gameScreenConfig;
    GameConfig m_gameConfig;
    NamedMap<SkillTemplate> m_skillTemplateLib;
};

} // end of namespace bot

#endif
