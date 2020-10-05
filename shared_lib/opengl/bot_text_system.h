#ifndef INCLUDE_BOT_TEXT_SYSTEM
#define INCLUDE_BOT_TEXT_SYSTEM

#include <unordered_map>
#include <string>
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_text_size.h"

namespace bot {

class SimpleShaderProgram;

class TextSystem {
public:
    static const int MIN_CHAR = 32;
    static const int MAX_CHAR = 126;
    static const int CHAR_COUNT = MAX_CHAR - MIN_CHAR + 1;

    TextSystem();

    virtual ~TextSystem();

    bool init(const std::string& fontFolder);

    const Texture& getTexture(char ch) const
    {
        return m_textures[static_cast<int>(ch) - MIN_CHAR];
    }

    const Rectangle& getRect(TextSize sz, char ch) const
    {
        return *(m_rectMap[sz][static_cast<int>(ch) - MIN_CHAR]);
    }

    void drawString(SimpleShaderProgram& program, const char* str,
                    TextSize size, const float* pos,
                    const float* color) const;

    void drawString(SimpleShaderProgram& program, const std::string& str,
                    TextSize size, const float* pos,
                    const float* color) const
    {
        drawString(program, str.c_str(), size, pos, color);
    }

    void getStringSize(float &width, float &height, TextSize sz,
                       const std::string& str) const;

protected:
    std::unordered_map<int, Rectangle> m_rects[TEXT_SIZE_COUNT];
    Rectangle *m_rectMap[TEXT_SIZE_COUNT][CHAR_COUNT];
    Texture m_textures[CHAR_COUNT];
};

} // close of namespace bot

#endif
