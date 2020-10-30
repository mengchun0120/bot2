#include "input/bot_input_event.h"
#include "opengl/bot_color.h"
#include "widget/bot_button.h"
#include "widget/bot_label.h"
#include "widget/bot_message_box.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

bool MessageBox::init(float x, float y, float width, float height,
                      float msgWidth, float msgHeight,
                      float buttonWidth, float buttonHeight,
                      float buttonSpacing, float buttonMsgSpacing,
                      const std::vector<std::sring>& buttonTexts)
{
    const MessageBoxConfig& cfg = GameLib::getInstance().getMessageBoxConfig();
    int buttonCount = static_cast<int>(buttonTexts.size());
    float msgX, msgY;
    float buttonX, buttonY;

    m_marginY = (height - msgHeight - buttonHeight -  buttonMsgSpacing) / 2.0f;
    msgX = x + (width - msgWidth) / 2.0f;
    msgY = y + m_marginY + buttonHeight + buttonMsgSpacing;
    buttonX = x + (width - buttonCount * buttonWidth -
              (buttonCount - 1) * buttonSpacing) / 2.0f;
    buttonY = y + m_marginY;

    WidgetGroup::init(buttonCount + 2);

    bool ret =
        initBack(cfg, x, y, width, height) &&
        initMsg(cfg, x, y, msgWidth, msgHeight) &&
        initButtons(cfg, x, y, buttonWidth, buttonHeight, buttonSpacing,
                    buttonTexts);

    if (!ret)
    {
        LOG_ERROR("Failed to initialize message box");
        return false;
    }

    return true;
}

bool MessageBox::initBack(const MessageBoxConfig& cfg, float x, float y,
                          float width, float height)
{
    Widget* back = new Widget();

    bool ret = back->init(x, y, width, height, nullptr, cfg.getBoxFillColor(),
                          cfg.getBoxBorderColor());
    if (!ret)
    {
        LOG_ERROR("Failed to initialize background");
        return false;
    }

    back->setAcceptInput(false);

    setWidget(BACK_IDX, back);

    return true;
}

bool MessageBox::initMsg(const MessageBoxConfig& cfg, float x, float y,
                         float msgWidth, float msgHeight)
{
    Label* label = new Label();

    ret = label->init(x, y, msgWidth, msgHeight, "", cfg.getTextColor(),
                      nullptr, nullptr, ALIGN_HMIDDLE, ALIGN_VMIDDLE,
                      TEXT_SIZE_BIG);
    if (!ret)
    {
        LOG_ERROR("Failed to initialize label");
        return false;
    }

    label->setAcceptInput(false);

    setWidget(MSG_IDX, label);

    return true;
}

bool MessageBox::initButtons(float x, float y, float buttonWidth,
                             float buttonHeight, float buttonSpacing,
                             const std::vector<std::string>& buttonTexts)
{
    float dx = buttonWidth + buttonSpacing;

    for (int i = 0; i < buttonCount; ++i)
    {
        Button* button = new Button();

        ret = button->init(x, y, buttonWidth, buttonHeight,
                           buttonTexts[i]);
        if (!ret)
        {
            LOG_ERROR("Failed to initialize button %d", i);
            return false;
        }

        setWidget(BUTTON_START_IDX + i, button);

        x += dx;
    }

    return true;
}

void MessageBox::setPos(float x, float y)
{
    const Widget& back = getWidget(BACK_IDX);
    float dx = x - back.getLeft();
    float dy = y - back.getBottom();
    shiftPos(dx, dy);
}

void MessageBox::setMsg(const std::string& msg)
{
    Label& label = static_cast<Label&>(getWidget(MSG_IDX));
    label.setText(msg);
}

void MessageBox::setAction(int idx, const Button::ActionFunc& func)
{
    Button& button = static_cast<Button&>(getWidget(BUTTON_START_IDX + idx));
    button.setActionFunc(func);
}

void MessageBox::setButtonVisible(int idx, bool visible)
{

}

} // end of namespace bot
