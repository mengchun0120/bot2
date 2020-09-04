#ifndef INCLUDE_BOT_MESSAGE_BOX_CONFIG
#define INCLUDE_BOT_MESSAGE_BOX_CONFIG

#include <vector>
#include <string>
#include "misc/bot_constants.h"

namespace bot {

template <typename T> class NamedMap;
class Rectangle;
class Color;

class MessageBoxConfig {
public:
    MessageBoxConfig();

    ~MessageBoxConfig()
    {}

    bool init(const std::string& configFile, float viewportWidth, float viewportHeight,
              const NamedMap<Rectangle>& rectLib, const NamedMap<Color>& colorLib);

    const Rectangle* getBoxRect() const
    {
        return m_boxRect;
    }

    const Rectangle* getButtonRect() const
    {
        return m_buttonRect;
    }

    const Color* getBoxFillColor() const
    {
        return m_boxFillColor;
    }

    const Color* getBoxBorderColor() const
    {
        return m_boxBorderColor;
    }

    const Color* getTextColor() const
    {
        return m_textColor;
    }

    const float* getBoxPos() const
    {
        return m_boxPos;
    }

    float getBoxLeft() const
    {
        return m_boxLeft;
    }

    float getBoxTop() const
    {
        return m_boxTop;
    }

    const std::vector<std::string>& getButtonTexts() const
    {
        return m_buttonTexts;
    }

    float getButtonSpacing() const
    {
        return m_buttonSpacing;
    }

    float getMsgMarginY() const
    {
        return m_msgMarginY;
    }

    float getButtonY() const
    {
        return m_buttonY;
    }

private:
    const Rectangle* m_boxRect;
    const Rectangle* m_buttonRect;
    const Color* m_boxFillColor;
    const Color* m_boxBorderColor;
    const Color* m_textColor;
    float m_boxPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_boxTop, m_boxLeft;
    std::vector<std::string> m_buttonTexts;
    float m_buttonSpacing;
    float m_msgMarginY;
    float m_buttonY;
};

} // end of namespace bot

#endif
