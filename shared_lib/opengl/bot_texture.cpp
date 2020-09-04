#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "misc/bot_json_utils.h"
#include "opengl/bot_texture.h"

namespace bot {

Texture* Texture::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    Texture* texture = new Texture();
    if (!texture->init(m_textureDir, elem))
    {
        delete texture;
        return nullptr;
    }

    return texture;
}

Texture::Texture()
    : m_textureId(0)
    , m_width(0)
    , m_height(0)
    , m_numChannels(0)
{
}

Texture::~Texture()
{
    if (m_textureId != 0)
    {
        glDeleteTextures(1, &m_textureId);
    }
}

bool Texture::init(const std::string& textureDir, const rapidjson::Value& elem)
{
    std::string fileName;
    if (!parseJson(fileName, elem, "file"))
    {
        LOG_ERROR("failed to parse file");
        return false;
    }

    std::string filePath = constructPath({ textureDir, fileName });

    if (!init(filePath))
    {
        LOG_ERROR("Failed to load texture from %s", filePath.c_str());
        return false;
    }

    return true;
}

bool Texture::init(const std::string& imageFile)
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imageFile.c_str(), &m_width, &m_height,
                                    &m_numChannels, 0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("Failed to load image from %s", imageFile.c_str());
        return false;
    }

    stbi_image_free(data);

    return true;
}

} // end of namespace bot
