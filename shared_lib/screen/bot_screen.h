#ifndef INCLUDE_BOT_SCREEN
#define INCLUDE_BOT_SCREEN

namespace bot {

class AppConfig;
class GameLib;
class Graphics;
class ScreenManager;
struct InputEvent;

class Screen {
public:
    enum Type {
        SCREEN_START,
        SCREEN_GAME,
        SCREEN_NONE
    };

    static Screen* create(Type type, const AppConfig* cfg, const GameLib* lib, Graphics* g,
                          ScreenManager* screenMgr, float viewportWidth, float viewportHeight);

    Screen()
    {}

    virtual ~Screen()
    {}

    // Returns 0 when the screen has been updated;
    // 1 when switched to another screen;
    // 2 when the app should exit
    virtual int update(float delta) = 0;

    virtual void present() = 0;

    // Returns 0 when the screen is ready to process next input;
    // 1 when switched to another screen;
    // 2 when the app should exit
    virtual int processInput(const InputEvent &e) = 0;
};

} // end of namespace bot

#endif

