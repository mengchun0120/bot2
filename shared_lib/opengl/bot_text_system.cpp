#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_text_system.h"

namespace bot {

std::string getTextSystemImageFile(const std::string& fontFolder, int ch)
{
    std::string file = "ascii_" + std::to_string(ch) + ".png";
    return constructPath({ fontFolder, file });
}

bool loadTextSystemTextures(const std::string& fontFolder, Texture *textures)
{
    for (int ch = TextSystem::MIN_CHAR; ch <= TextSystem::MAX_CHAR; ++ch)
    {
        std::string fileName = getTextSystemImageFile(fontFolder, ch);
        if (!textures[ch-TextSystem::MIN_CHAR].init(fileName))
        {
            LOG_ERROR("Failed to create texture for ascii %d", ch);
            return false;
        }
    }

    return true;
}

bool loadTextSystemRectangles(std::unordered_map<int, Rectangle> *rects,
                              Rectangle *rectMap[][TextSystem::CHAR_COUNT],
                              const Texture* textures)
{
    const float SCALE_FACTOR[] = {1.0f, 0.75f, 0.5f, 0.36f};

    for(int size = TEXT_SIZE_BIG; size < TEXT_SIZE_COUNT; ++size)
    {
        for (int ch = 0; ch < TextSystem::CHAR_COUNT; ++ch)
        {
            int height = static_cast<int>(textures[ch].height() *
                                          SCALE_FACTOR[size]);
            int width = static_cast<int>(textures[ch].width() *
                                         SCALE_FACTOR[size]);

            auto it = rects[size].find(width);
            if (it == rects[size].end())
            {
                bool ret = rects[size][width].init(
                                            static_cast<float>(width),
                                            static_cast<float>(height), true);
                if(!ret)
                {
                    LOG_ERROR("Failed to create Rectangle for size=%d width=%d",
                              size, width);
                    return false;
                }

                rectMap[size][ch] = &(rects[size][width]);
            }
            else
            {
                rectMap[size][ch] = &(it->second);
            }
        }
    }

    return true;
}

std::shared_ptr<TextSystem> TextSystem::k_textSys;

bool TextSystem::initInstance(const std::string& fontFolder)
{
    TextSystem* textSys = new TextSystem();
    k_textSys.reset(textSys);

    if (!textSys->init(fontFolder))
    {
        k_textSys.reset();
        return false;
    }

    return true;
}

TextSystem::TextSystem()
{
}

TextSystem::~TextSystem()
{
}

bool TextSystem::init(const std::string& fontFolder)
{
    if (!loadTextSystemTextures(fontFolder, m_textures))
    {
        return false;
    }

    if (!loadTextSystemRectangles(m_rects, m_rectMap, m_textures))
    {
        return false;
    }

    return true;
}

void TextSystem::drawString(const char* str, TextSize size,
                            const float* pos, const float* color) const
{
    if (!isValidTextSize(size))
    {
        LOG_ERROR("Invalid text-size %d", static_cast<int>(size));
        return;
    }

    if (str[0] == '\0')
    {
        return;
    }

    SimpleShaderProgram& program = SimpleShaderProgram::getInstance();

    program.setUseColor(false);
    program.setUseObjRef(true);

    if (color)
    {
        program.setUseTexColor(true);
        program.setTexColor(color);
    }
    else
    {
        program.setUseTexColor(false);
    }

    float realPos[] = { pos[0], pos[1] };
    Rectangle* rect = m_rectMap[size][str[0] - MIN_CHAR];
    float halfWidth = rect->width() / 2.0f;
    realPos[0] += halfWidth;
    realPos[1] += rect->height() / 2.0f;
    const char* p = str;

    while(true)
    {
        program.setObjRef(realPos);
        program.setPosition(rect->vertexArray(), true);
        program.setTexture(m_textures[*p - MIN_CHAR].textureId());
        glDrawArrays(GL_TRIANGLE_FAN, 0, rect->vertexArray().numVertices());

        ++p;
        if (*p == '\0')
        {
            break;
        }

        realPos[0] += halfWidth;
        rect = m_rectMap[size][*p - MIN_CHAR];
        halfWidth = rect->width() / 2.0f;
        realPos[0] += halfWidth;
    }
}

void TextSystem::getStringSize(float& width, float& height, TextSize sz,
                               const char* str) const
{
    if (*str == '\0')
    {
        width = 0.0f;
        height = 0.0f;
        return;
    }

    const char* p = str;

    const Rectangle& rect = getRect(sz, *p);
    height = rect.height();

    float w = rect.width();

    for (++p; *p != '\0'; ++p)
    {
        const Rectangle& r = getRect(sz, *p);
        w += r.width();
    }

    width = w;
}

} // close of namespace bot

