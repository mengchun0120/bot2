#ifndef INCLUDE_BOT_BOX
#define INCLUDE_BOT_BOX

#include "misc/bot_constants.h"
#include "widget/bot_widget.h"

namespace bot {

class Rectangle;
class Texture;
class Color;

class Box: public Widget {
public:
    Box();

    virtual ~Box()
    {}

    bool init(float x, float y, float width, float height,
              const Rectangle* rect, const Texture* texture,
              const Color* backColor, const Color* borderColor,
              bool visible=true, bool acceptInput=false);

    virtual void present();

    virtual void setPos(float x, float y);

    virtual void shiftPos(float dx, float dy);

protected:
    void resetRectPos();

protected:
    float m_rectPos[Constants::NUM_FLOATS_PER_POSITION];
    const Rectangle* m_rect;
    const Texture* m_texture;
    const Color* m_backColor, * m_borderColor;
};

} // end of namespace bot

#endif

