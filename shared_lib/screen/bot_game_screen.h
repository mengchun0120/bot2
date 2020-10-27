#ifndef INCLUDE_BOT_GAME_SCREEN
#define INCLUDE_BOT_GAME_SCREEN

#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"
#include "gameutil/bot_game_state.h"
#include "gameobj/bot_dashboard.h"
#include "widget/bot_message_box.h"
#include "screen/bot_screen.h"

namespace bot {

struct MouseMoveEvent;
struct MouseButtonEvent;
struct KeyEvent;

class GameScreen: public Screen {
    enum {
        MSGBOX_ESCAPE_GAME,
        MSGBOX_VICTORY,
        MSGBOX_DEFEAT,
        MSGBOX_COUNT
    };
public:
    GameScreen();

    virtual ~GameScreen();

    bool init();

    bool loadMap(const std::string& fileName);

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
    void initMessageBoxes(const MessageBoxConfig& msgBoxCfg,
                          const ButtonConfig& buttonCfg);

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

    int exitGame();

    int resumeGame();

    int restartGame();

private:
    GameMap m_map;
    GameState m_state;
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
    float m_dashboardOrigin[Constants::NUM_FLOATS_PER_POSITION];
    Dashboard m_dashboard;
    /*std::vector<MessageBox> m_msgBox;
    int m_visibleMsgBoxIdx;*/
};

} // end of namespace bot

#endif
