#ifndef INCLUDE_BOT_TEXTURE
#define INCLUDE_BOT_TEXTURE

#include <string>
#include <rapidjson/document.h>

namespace bot {

class Texture {
public:
    Texture();

    virtual ~Texture();

    bool init(const rapidjson::Value& elem);

    bool init(const std::string& imageFile);

    unsigned int textureId() const
    {
        return m_textureId;
    }

    int width() const
    {
        return m_width;
    }

    int height() const
    {
        return m_height;
    }

    int numChannels() const
    {
        return m_numChannels;
    }

private:
    unsigned int m_textureId;
    int m_width, m_height, m_numChannels;
};

} // end of namespace bot

#endif

