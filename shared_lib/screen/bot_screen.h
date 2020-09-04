#ifndef INCLUDE_BOT_SCREEN
#define INCLUDE_BOT_SCREEN

namespace bot {

class App;
struct InputEvent;

class Screen {
public:
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

