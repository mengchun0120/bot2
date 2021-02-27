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
                      const std::vector<std::string> &buttonTexts)
{
    int buttonCount = static_cast<int>(buttonTexts.size());

    if (!WidgetGroup::init(buttonCount + 2))
    {
        return false;
    }

    float marginY = (height - msgHeight - buttonHeight -
                     buttonMsgSpacing) / 2.0f;

    m_buttonSpacing = buttonSpacing;
    m_buttonY = y + marginY;

    float msgX, msgY;
    float buttonX;
    const MessageBoxConfig &cfg = GameLib::getInstance().getMessageBoxConfig();

    msgX = x + (width - msgWidth) / 2.0f;
    msgY = m_buttonY + buttonHeight + buttonMsgSpacing;
    buttonX = x + (width - buttonCount * buttonWidth -
              (buttonCount - 1) * buttonSpacing) / 2.0f;

    bool ret =
        initBack(cfg, x, y, width, height) &&
        initMsg(cfg, msgX, msgY, msgWidth, msgHeight) &&
        initButtons(buttonX, m_buttonY, buttonWidth, buttonHeight,
                    buttonSpacing, buttonTexts);

    if (!ret)
    {
        LOG_ERROR("Failed to initialize message box");
        return false;
    }

    return true;
}

bool MessageBox::initBack(const MessageBoxConfig &cfg,
                          float x, float y,
                          float width, float height)
{
    if (!m_backRect.init(width, height, false))
    {
        LOG_ERROR("Failed to initialize backRect");
        return false;
    }

    Box *back = new Box();

    bool ret = back->init(x, y, width, height,
                          &m_backRect, nullptr,
                          cfg.getBoxBorderColor(), cfg.getBoxFillColor(),
                          true, false);
    if (!ret)
    {
        LOG_ERROR("Failed to initialize background");
        delete back;
        return false;
    }

    setWidget(BACK_IDX, back);

    return true;
}

bool MessageBox::initMsg(const MessageBoxConfig &cfg,
                         float x, float y,
                         float msgWidth, float msgHeight)
{
    if (!m_msgRect.init(msgWidth, msgHeight, false))
    {
        LOG_ERROR("Failed to initialize msgRect");
        return false;
    }

    Label *label = new Label();

    bool ret = label->init(x, y,
                           msgWidth, msgHeight,
                           &m_msgRect,
                           "", cfg.getTextColor(),
                           nullptr, nullptr,
                           ALIGN_HMIDDLE, ALIGN_VMIDDLE,
                           TEXT_SIZE_SMALL);
    if (!ret)
    {
        LOG_ERROR("Failed to initialize label");
        return false;
    }

    setWidget(MSG_IDX, label);

    return true;
}

bool MessageBox::initButtons(float x, float y,
                             float buttonWidth, float buttonHeight,
                             float buttonSpacing,
                             const std::vector<std::string> &buttonTexts)
{
    if (!m_buttonRect.init(buttonWidth, buttonHeight, true))
    {
        LOG_ERROR("Failed to initialize buttonRect");
        return false;
    }

    float dx = buttonWidth + buttonSpacing;
    int buttonCount = static_cast<int>(buttonTexts.size());

    for (int i = 0; i < buttonCount; ++i)
    {
        Button *button = new Button();

        bool ret = button->init(x, y,
                                buttonWidth, buttonHeight,
                                &m_buttonRect,
                                buttonTexts[i],
                                TEXT_SIZE_SMALL);
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
    const Widget *back = getWidget(BACK_IDX);
    float dx = x - back->getLeft();
    float dy = y - back->getBottom();
    shiftPos(dx, dy);
}

Button *MessageBox::getButton(int idx)
{
    if (idx < 0 || idx >= getWidgetCount() - BUTTON_START_IDX)
    {
        return nullptr;
    }

    return static_cast<Button*>(getWidget(BUTTON_START_IDX + idx));
}

void MessageBox::setMsg(const std::string &msg)
{
    Label &label = static_cast<Label&>(*m_widgets[MSG_IDX]);
    label.setText(msg);
}

bool MessageBox::setAction(int idx, const Button::ActionFunc &func)
{
    Button *button = getButton(idx);
    if (!button)
    {
        LOG_ERROR("Invalid button idx %d", idx);
        return false;
    }

    button->setActionFunc(func);
    return true;
}

bool MessageBox::setButtonVisible(int idx, bool visible)
{
    Button *button = getButton(idx);
    if (!button)
    {
        LOG_ERROR("Invalid button idx %d", idx);
        return false;
    }

    if (button->visible() == visible)
    {
        return true;
    }

    button->setVisible(visible);

    float totalWidth = 0.0f;
    int visibleCount = 0;
    int widgetCount = getWidgetCount();

    for (int i = BUTTON_START_IDX; i < widgetCount; ++i)
    {
        Button &btn = static_cast<Button&>(*m_widgets[i]);
        if (btn.visible())
        {
            totalWidth += btn.getWidth();
            ++visibleCount;
        }
    }

    if (visibleCount == 0)
    {
        return true;
    }

    const Widget &back = *m_widgets[BACK_IDX];
    float x = back.getLeft() + (back.getWidth() - totalWidth) / 2.0f;

    for (int i = BUTTON_START_IDX; i < widgetCount; ++i)
    {
        Button &btn = static_cast<Button&>(*m_widgets[i]);
        if (btn.visible())
        {
            btn.setPos(x, m_buttonY);
            x += btn.getWidth() + m_buttonSpacing;
        }
    }

    return true;
}

} // end of namespace bot
