#ifndef INCLUDE_BOT_STATUS_BAR
#define INCLUDE_BOT_STATUS_BAR

#include "misc/bot_constants.h"
#include "gametemplate/bot_status_bar_template.h"

namespace bot {

class StatusBar {
public:
    StatusBar();

    ~StatusBar();

    bool init(const StatusBarTemplate* t, float x, float y);

    float getWidth() const
    {
        return m_template->getWidth();
    }

    float getHeight() const
    {
        return m_template->getHeight();
    }

    void setText(const char* text);

    void setText(int i);

    void setText(float f);

    void draw();

private:
    const StatusBarTemplate* m_template;
    float m_texturePos[Constants::NUM_FLOATS_PER_POSITION];
    float m_textPos[Constants::NUM_FLOATS_PER_POSITION];
    char* m_text;
};

};

#endif

