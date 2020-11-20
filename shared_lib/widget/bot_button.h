#ifndef INCLUDE_BOT_BUTTON
#define INCLUDE_BOT_BUTTON

#include <functional>
#include <string>
#include "misc/bot_constants.h"
#include "opengl/bot_text_size.h"
#include "widget/bot_widget.h"

namespace bot {

class Color;

class Button : public Widget {
public:
    typedef std::function<int()> ActionFunc;

    Button();

    virtual ~Button()
    {}

    bool init(float x, float y, float width, float height,
              const std::string& text, TextSize textSize=TEXT_SIZE_MEDIUM,
              bool acceptInput=true);

    void setText(const std::string& text);

    void setActionFunc(const ActionFunc& actionFunc)
    {
        m_actionFunc = actionFunc;
    }

    virtual void setPos(float x, float y);

    virtual void shiftPos(float dx, float dy);

    virtual int processKeyEvent(const KeyEvent& event)
    {
        return 0;
    }

    virtual int processMouseMoveEvent(const MouseMoveEvent& event);

    virtual int processMouseButtonEvent(const MouseButtonEvent& event);

    virtual void onLostFocus()
    {}

    virtual void onMouseOut();

    virtual void present();

protected:
    std::string m_text;
    float m_textPos[Constants::NUM_FLOATS_PER_POSITION];
    const Color* m_textColor;
    TextSize m_textSize;
    ActionFunc m_actionFunc;
};

} // end of namespace bot

#endif
