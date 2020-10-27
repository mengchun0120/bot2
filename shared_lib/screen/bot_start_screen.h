#ifndef INCLUDE_BOT_START_SCREEN
#define INCLUDE_BOT_START_SCREEN

#include <string>
#include "misc/bot_constants.h"
#include "widget/bot_widget_group.h"
#include "screen/bot_screen.h"

namespace bot {

class StartScreen: public Screen {
public:
    StartScreen();

    virtual ~StartScreen();

    bool init();

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

private:
    int startGame();

    int loadGame();

    int showSettings();

    int exitApp();

private:
    enum {
        BUTTON_START_GAME = 0,
        BUTTON_LOAD_GAME,
        BUTTON_SETTINGS,
        BUTTON_EXIT,
        NUM_BUTTONS
    };

    WidgetGroup m_buttons;
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

