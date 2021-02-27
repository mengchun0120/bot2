#ifndef INCLUDE_BOT_GAME_SCREEN
#define INCLUDE_BOT_GAME_SCREEN

#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"
#include "gameutil/bot_game_state.h"
#include "widget/bot_dashboard.h"
#include "widget/bot_message_box.h"
#include "screen/bot_screen.h"

struct GLFWwindow;

namespace bot {

struct MouseMoveEvent;
struct MouseButtonEvent;
struct KeyEvent;
class ButtonConfig;

class GameScreen: public Screen {
    enum {
        BUTTON_EXIT,
        BUTTON_RESUME
    };

public:
    GameScreen();

    virtual ~GameScreen();

    bool init();

    bool loadMap(const std::string &fileName);

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

    GameMap &getMap()
    {
        return m_map;
    }

    const GameMap &getMap() const
    {
        return m_map;
    }

    GameObjectManager &getGameObjManager()
    {
        return m_gameObjManager;
    }

    const GameObjectManager &getGameObjManager() const
    {
        return m_gameObjManager;
    }

    GameState getGameState() const
    {
        return m_state;
    }

private:
    bool initMessageBox();

    bool updateRobots(float delta);

    bool updateMissiles(float delta);

    void updateEffects(float delta);

    void updateDissolveObjects(float delta);

    void presentEffects();

    void presentOverlay();

    void clearDeadObjects();

    int handleMouseMove(const MouseMoveEvent &e);

    int handleMouseButton(const MouseButtonEvent &e);

    int handleKey(const KeyEvent &e);

    int switchToStart();

    int exit();

    int resume();

private:
    GLFWwindow *m_window;
    GameMap m_map;
    GameState m_state;
    GameObjectManager m_gameObjManager;
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
    float m_dashboardOrigin[Constants::NUM_FLOATS_PER_POSITION];
    Dashboard m_dashboard;
    MessageBox m_msgBox;
};

} // end of namespace bot

#endif
