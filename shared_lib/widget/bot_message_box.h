#ifndef INCLUDE_BOT_MESSAGE_BOX
#define INCLUDE_BOT_MESSAGE_BOX

#include "misc/bot_constants.h"
#include "geometry/bot_rectangle.h"
#include "widget/bot_widget_group.h"
#include "widget/bot_button.h"

namespace bot {

struct InputEvent;
class MessageBoxConfig;

class MessageBox: public WidgetGroup {
    enum {
        BACK_IDX = 0,
        MSG_IDX = 1,
        BUTTON_START_IDX = 2
    };

public:
    MessageBox()
        : m_buttonY(0.0f)
        , m_buttonSpacing(0.0f)
    {}

    ~MessageBox()
    {}

    bool init(float x, float y, float width, float height,
              float msgWidth, float msgHeight,
              float buttonWidth, float buttonHeight,
              float buttonSpacing, float buttonMsgSpacing,
              const std::vector<std::string> &buttonTexts);

    void setPos(float x, float y);

    void setMsg(const std::string &msg);

    bool setAction(int idx, const Button::ActionFunc &func);

    bool setButtonVisible(int idx, bool visible);

private:
    bool initBack(const MessageBoxConfig &cfg,
                  float x, float y,
                  float width, float height);

    bool initMsg(const MessageBoxConfig &cfg,
                 float x, float y,
                 float msgWidth, float msgHeight);

    bool initButtons(float x, float y,
                     float buttonWidth, float buttonHeight,
                     float buttonSpacing,
                     const std::vector<std::string> &buttonTexts);

    Button *getButton(int idx);

private:
    float m_buttonY;
    float m_buttonSpacing;
    Rectangle m_backRect;
    Rectangle m_msgRect;
    Rectangle m_buttonRect;
};

} // end of namespace bot

#endif
