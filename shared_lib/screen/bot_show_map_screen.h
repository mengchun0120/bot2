#ifndef INCLUDE_BOT_SHOW_MAP_SCREEN
#define INCLUDE_BOT_SHOW_MAP_SCREEN

#include <string>
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"
#include "screen/bot_screen.h"

namespace bot {

class GameLib;
class Graphics;
class KeyEvent;

class ShowMapScreen: public Screen {
public:
    ShowMapScreen();

    virtual ~ShowMapScreen()
    {}

    bool init(const AppConfig& cfg, GameLib* lib, Graphics* g,
              float viewportWidth, float viewportHeight);

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

private:
    int handleKey(const KeyEvent& e);

private:
    const GameLib* m_lib;
    Graphics* m_graphics;
    GameMap m_map;
    GameObjectManager m_gameObjManager;
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

