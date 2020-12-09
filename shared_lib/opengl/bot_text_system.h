#ifndef INCLUDE_BOT_TEXT_SYSTEM
#define INCLUDE_BOT_TEXT_SYSTEM

#include <unordered_map>
#include <string>
#include <memory>
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_text_size.h"

namespace bot {

class TextSystem {
public:
    static const int MIN_CHAR = 32;
    static const int MAX_CHAR = 126;
    static const int CHAR_COUNT = MAX_CHAR - MIN_CHAR + 1;

    static bool initInstance(const std::string& fontFolder);

    static const TextSystem& getInstance()
    {
        return *k_textSys;
    }

    virtual ~TextSystem();

    const Rectangle& getRect(TextSize sz, char ch) const
    {
        return *(m_rectMap[sz][static_cast<int>(ch) - MIN_CHAR]);
    }

    void drawString(const char* str, TextSize size,
                    const float* pos, const float* color) const;

    void drawString(const std::string& str, TextSize size,
                    const float* pos, const float* color) const
    {
        drawString(str.c_str(), size, pos, color);
    }

    float getCharHeight(TextSize sz) const;

    float getMaxCharWidth(TextSize sz) const;

    void getStringSize(float& width, float& height, TextSize sz,
                       const char* str) const;

    void getStringSize(float& width, float& height, TextSize sz,
                       const std::string& str) const
    {
        getStringSize(width, height, sz, str.c_str());
    }

private:
    TextSystem();

    bool init(const std::string& fontFolder);

private:
    static std::shared_ptr<TextSystem> k_textSys;
    std::unordered_map<int, Rectangle> m_rects[TEXT_SIZE_COUNT];
    float m_charHeight[TEXT_SIZE_COUNT];
    float m_maxCharWidth[TEXT_SIZE_COUNT];
    Rectangle *m_rectMap[TEXT_SIZE_COUNT][CHAR_COUNT];
    Texture m_textures[CHAR_COUNT];
};

} // close of namespace bot

#endif
