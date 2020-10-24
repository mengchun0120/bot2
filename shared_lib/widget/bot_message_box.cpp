#include "input/bot_input_event.h"
#include "opengl/bot_color.h"
#include "opengl/bot_graphics.h"
#include "widget/bot_message_box_config.h"
#include "widget/bot_message_box.h"

namespace bot {

MessageBox::MessageBox()
    : m_cfg(nullptr)
{
    m_msgPos[0] = 0.0f;
    m_msgPos[1] = 0.0f;
}

void MessageBox::init(const MessageBoxConfig* cfg,
                      const ButtonConfig* buttonCfg,
                      const TextSystem* textSys,
                      float width, float height,
                      float viewportWidth, float viewportHeight,
                      const std::vector<std::sring>& buttonTexts)
{
    m_cfg = cfg;

    float boxWidth = m_cfg->getBoxRect()->width();
    float boxX = m_cfg->getBoxLeft();
    float textWidth, textHeight;

    m_msg = msg;
    textSys.getStringSize(textWidth, textHeight, TEXT_SIZE_BIG, m_msg);
    m_msgPos[0] = boxX + (boxWidth - textWidth) / 2.0f;
    m_msgPos[1] = m_cfg->getBoxTop() - m_cfg->getMsgMarginY() - textHeight;

    int buttonCount = static_cast<int>(buttonTexts.size());
    float buttonWidth = m_cfg->getButtonRect()->width();
    float spacing = m_cfg->getButtonSpacing();
    float buttonX = boxX + (boxWidth - buttonCount * buttonWidth -
                    (buttonCount - 1) * spacing) / 2.0f;

    m_buttons.init(buttonCount);
    for (int i = 0; i < buttonCount; ++i)
    {
        Button* button = new Button();
        button->init(buttonCfg, m_cfg->getButtonRect(), buttonTexts[i]);
        button->setPos(textSys, buttonX, m_cfg->getButtonY());
        m_buttons.setWidget(i, button);
        buttonX += buttonWidth + spacing;
    }
}

void MessageBox::setAction(int buttonIdx, const Button::ActionFunc& func)
{
    Button& button = static_cast<Button&>(m_buttons.getWidget(buttonIdx));
    button.setActionFunc(func);
}

int MessageBox::processInput(const InputEvent& e)
{
    return m_buttons.processInput(e);
}

void MessageBox::show(Graphics& g)
{
    m_cfg->getBoxRect()->draw(g, m_cfg->getBoxPos(), nullptr,
                              m_cfg->getBoxFillColor(),
                              m_cfg->getBoxBorderColor(),
                              0, nullptr);
    g.getTextSystem().drawString(g.getSimpleShader(), m_msg, TEXT_SIZE_BIG,
                                 m_msgPos, m_cfg->getTextColor()->getColor());
    m_buttons.present(g);
}

} // end of namespace bot
