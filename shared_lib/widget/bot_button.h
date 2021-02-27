#ifndef INCLUDE_BOT_BUTTON
#define INCLUDE_BOT_BUTTON

#include <functional>
#include <string>
#include "opengl/bot_text_size.h"
#include "widget/bot_box.h"

namespace bot {

class Button : public Box {
public:
    typedef std::function<int()> ActionFunc;

    Button();

    virtual ~Button()
    {}

    bool init(float x, float y,
              float width, float height,
              const Rectangle *rect,
              const std::string &text,
              TextSize textSize=TEXT_SIZE_MEDIUM,
              bool visible=true, bool acceptInput=true);

    virtual void present();

    void setText(const std::string &text);

    void setActionFunc(const ActionFunc &actionFunc)
    {
        m_actionFunc = actionFunc;
    }

    virtual void setPos(float x, float y);

    virtual void shiftPos(float dx, float dy);

    virtual int processKeyEvent(const KeyEvent &event);

    virtual int processMouseMoveEvent(const MouseMoveEvent &event);

    virtual int processMouseButtonEvent(const MouseButtonEvent &event);

    virtual void onMouseOut();

protected:
    void resetTextPos();

protected:
    std::string m_text;
    float m_textPos[Constants::NUM_FLOATS_PER_POSITION];
    const Color *m_textColor;
    TextSize m_textSize;
    ActionFunc m_actionFunc;
};

} // end of namespace bot

#endif

