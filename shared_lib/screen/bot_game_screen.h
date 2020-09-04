#ifndef INCLUDE_BOT_GAME_SCREEN
#define INCLUDE_BOT_GAME_SCREEN

#include "screen/bot_screen.h"
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"
#include "gameutil/bot_game_state.h"
#include "gameobj/bot_dashboard.h"
#include "widget/bot_message_box.h"

namespace bot {

struct MouseMoveEvent;
struct MouseButtonEvent;
struct KeyEvent;
class GameLib;
class Graphics;
class AppConfig;
class ScreenManager;

class GameScreen: public Screen {
public:
    GameScreen();

    virtual ~GameScreen();

    virtual bool init(const AppConfig& cfg, const GameLib* lib, Graphics* g,
                      ScreenManager* screenManager, float viewportWidth, float viewportHeight);

    bool loadMap(const std::string& fileName, const AppConfig& cfg);

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

    GameMap& getMap()
    {
        return m_map;
    }

    const GameMap& getMap() const
    {
        return m_map;
    }

    GameObjectManager& getGameObjManager()
    {
        return m_gameObjManager;
    }

    const GameObjectManager& getGameObjManager() const
    {
        return m_gameObjManager;
    }

    GameState getGameState() const
    {
        return m_state;
    }

private:
    bool updateRobots(float delta);

    bool updateMissiles(float delta);

    void updateEffects(float delta);

    void presentEffects();

    void presentOverlay();

    void clearDeadObjects();

    int handleMouseMove(const MouseMoveEvent& e);

    int handleMouseButton(const MouseButtonEvent& e);

    int handleKey(const KeyEvent& e);

    int switchToStart();

private:
    const GameLib* m_lib;
    Graphics* m_graphics;
    ScreenManager* m_screenManager;
    GameMap m_map;
    GameObjectManager m_gameObjManager;
    GameState m_state;
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
    float m_dashboardOrigin[Constants::NUM_FLOATS_PER_POSITION];
    Dashboard m_dashboard;
    MessageBox m_msgBox;
    bool m_msgBoxVisible;
};

} // end of namespace bot

#endif
