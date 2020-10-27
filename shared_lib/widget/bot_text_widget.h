#ifndef INCLUDE_BOT_TEXT_WIDGET
#define INCLUDE_BOT_TEXT_WIDGET

#include <string>
#include "widget/bot_align_setting.h"
#include "widget/bot_widget.h"

namespace bot {

class TextWidget: public Widget {
public:
    TextWidget();

    virtual ~TextWidget()
    {}

    bool init(float x, float y, float width, float height,
              const Texture* texture, const Color* borderColor,
              const Color* backColor, const Color* textColor,
              AlignSetting hAlign, AlignSetting vAlign);

    const std::string& getText() const
    {
        return m_text;
    }

    void setText(const std::string& text);

protected:
    std::string m_text;
    const Color* m_textColor;
};

} // end of namespace bot

#endif

