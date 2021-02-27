#ifndef INCLUDE_BOT_SHOW_MAP_SCREEN
#define INCLUDE_BOT_SHOW_MAP_SCREEN

#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"
#include "screen/bot_screen.h"

namespace bot {

class KeyEvent;

class ShowMapScreen: public Screen {
public:
    ShowMapScreen();

    virtual ~ShowMapScreen()
    {}

    bool init();

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

private:
    int handleKey(const KeyEvent &e);

private:
    GameMap m_map;
    GameObjectManager m_gameObjManager;
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

