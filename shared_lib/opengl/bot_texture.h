#ifndef INCLUDE_BOT_TEXTURE
#define INCLUDE_BOT_TEXTURE

#include <string>
#include <rapidjson/document.h>

namespace bot {

class Texture {
public:
    class Parser {
    public:
        Parser(const std::string& textureDir)
            : m_textureDir(textureDir)
        {}

        ~Parser()
        {}

        Texture* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const std::string& m_textureDir;
    };

    Texture();

    virtual ~Texture();

    bool init(const std::string& textureDir, const rapidjson::Value& elem);

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

