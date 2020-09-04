#ifndef INCLUDE_BOT_SIMPLE_SHADER_PROGRAM
#define INCLUDE_BOT_SIMPLE_SHADER_PROGRAM

#include "opengl/bot_opengl.h"
#include "opengl/bot_shader_program.h"

namespace bot {

class VertexArray;

class SimpleShaderProgram: public ShaderProgram {
public:
    SimpleShaderProgram();

    virtual ~SimpleShaderProgram();

    virtual bool init(const std::string& vertexShaderFile, const std::string& fragShaderFile);

    void setUseObjRef(bool enabled)
    {
        glUniform1i(m_useObjRefLocation, enabled ? 1 : 0);
    }

    void setObjRef(const float *objRef)
    {
        glUniform2fv(m_objRefLocation, 1, objRef);
    }

    void setViewportSize(const float *viewportSize)
    {
        glUniform2fv(m_viewportSizeLocation, 1, viewportSize);
    }

    void setViewportOrigin(const float *viewportOrigin)
    {
        glUniform2fv(m_viewportOriginLocation, 1, viewportOrigin);
    }

    void setPosition(const VertexArray &vertexArray, bool hasTexCoord);

    void setColor(const float *color)
    {
        glUniform4fv(m_colorLocation, 1, color);
    }

    void setUseColor(bool use)
    {
        glUniform1i(m_useColorLocation, use ? 1 : 0);
    }

    void setTexture(unsigned int textureId);

    void setUseDirection(bool use)
    {
        glUniform1i(m_useDirectionLocation, use ? 1 : 0);
    }

    void setDirection(const float *direction)
    {
        glUniform2fv(m_directionLocation, 1, direction);
    }

    void setUseTexColor(bool use)
    {
        glUniform1i(m_useTexColorLocation, use ? 1 : 0);
    }

    void setTexColor(const float *color)
    {
        glUniform4fv(m_texColorLocation, 1, color);
    }

protected:
    void loadParam();

    int m_positionLocation;
    int m_useObjRefLocation;
    int m_objRefLocation;
    int m_viewportSizeLocation;
    int m_viewportOriginLocation;
    int m_colorLocation;
    int m_useColorLocation;
    int m_texPosLocation;
    int m_textureLocation;
    int m_useDirectionLocation;
    int m_directionLocation;
    int m_useTexColorLocation;
    int m_texColorLocation;
};

} // end of namespace zubot

#endif

