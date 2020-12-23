#ifndef INCLUDE_BOT_SHELL
#define INCLUDE_BOT_SHELL

#include "gameobj/bot_shell_template.h"
#include "gameobj/bot_missile.h"

namespace bot {

class Shell: public Missile {
public:
    Shell();

    virtual ~Shell();

    bool init(const ShellTemplate* t, Side side, float x, float y,
              float directionX, float directionY, float damage);

    const ShellTemplate* getTemplate() const
    {
        return static_cast<const ShellTemplate*>(m_template);
    }

    virtual void present();

    virtual void update(float delta, GameScreen& screen);

    virtual void onDeath(GameScreen& screen);

private:
    void explode(GameScreen& screen);
};

} // end of namespace bot

#endif

