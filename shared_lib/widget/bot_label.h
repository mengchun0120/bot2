#ifndef INCLUDE_BOT_LABEL
#define INCLUDE_BOT_LABEL

#include <string>
#include "opengl/bot_text_size.h"
#include "widget/bot_align.h"
#include "widget/bot_box.h"

namespace bot {

class Color;

class Label: public Box {
public:
    Label();

    virtual ~Label()
    {}

    bool init(float x, float y,
              float width, float height,
              const Rectangle *rect,
              const std::string &text, const Color *textColor,
              const Color *backColor, const Color *borderColor,
              Align hAlign, Align vAlign,
              TextSize sz=TEXT_SIZE_MEDIUM,
              bool visible=true, bool acceptInput=false);

    const std::string &getText() const
    {
        return m_text;
    }

    void setText(const std::string &text);

    void setTextColor(const Color *color)
    {
        m_textColor = color;
    }

    virtual void setPos(float x, float y);

    virtual void shiftPos(float x, float y);

    virtual void present();

protected:
    void resetTextPos();

protected:
    std::string m_text;
    float m_textPos[Constants::NUM_FLOATS_PER_POSITION];
    TextSize m_size;
    Align m_hAlign, m_vAlign;
    const Color *m_textColor;
};

} // end of namespace bot

#endif

