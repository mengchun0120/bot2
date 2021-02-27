#include <stdexcept>
#include <GL/glew.h>
#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "opengl/bot_shader_program.h"

namespace bot {

const char *shaderName(GLenum type)
{
    switch(type) {
        case GL_VERTEX_SHADER:
            return "vertex shader";
        case GL_FRAGMENT_SHADER:
            return "fragment shader";
    }

    return "invalid shader";
}

GLuint compileShader(GLenum type, const std::string &fileName)
{
    std::string source;

    if (!readText(source, fileName))
    {
        return 0;
    }

    const GLchar *cSource = source.c_str();

    GLuint shader = glCreateShader(type);
    if (shader == 0)
    {
        LOG_ERROR("Failed to create shader %d", type);
        return 0;
    }

    GLint len = source.length();

    glShaderSource(shader, 1, (const GLchar * const*)&cSource, &len);
    glCompileShader(shader);

    GLint compileStatus;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == 0)
    {
        GLint infoLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        char *info = new char[infoLen + 1];

        glGetShaderInfoLog(shader, infoLen, NULL, info);
        LOG_INFO("Results of compilation:\n%s", info);

        delete[] info;

        LOG_ERROR("Compilation failed for %s", fileName.c_str());
        glDeleteShader(shader);
        shader = 0;
    }
    else
    {
        LOG_INFO("Compilation is successful for %s", fileName.c_str());
    }

    return shader;
}

GLuint linkProgram(GLuint vertex_shader, GLuint frag_shader)
{
    GLuint program = glCreateProgram();
    if (program == 0)
    {
        LOG_ERROR("Failed to create program");
        return 0;
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    GLint linkStatus;

    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == 0)
    {
        GLint infoLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        char *info = new char[infoLen + 1];

        glGetProgramInfoLog(program, infoLen, NULL, info);
        LOG_INFO("Results of linking:\n%s", info);

        delete[] info;

        LOG_ERROR("Linking failed");
        return 0;
    }
    else
    {
        LOG_INFO("Linking is successful");
    }

    return program;
}

ShaderProgram::ShaderProgram()
    : m_vertexShader(0)
    , m_fragShader(0)
    , m_program(0)
{
}

ShaderProgram::~ShaderProgram()
{
    if (m_program != 0)
    {
        if(m_vertexShader != 0)
        {
            glDetachShader(m_program, m_vertexShader);
        }

        if(m_fragShader != 0)
        {
            glDetachShader(m_program, m_fragShader);
        }
    }

    if (m_vertexShader != 0)
    {
        glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }

    if (m_fragShader != 0)
    {
        glDeleteShader(m_fragShader);
        m_fragShader = 0;
    }

    if (m_program != 0)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

bool ShaderProgram::init(const std::string &vertexShaderFile,
                          const std::string &fragShaderFile)
{
    m_vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderFile);
    if (m_vertexShader == 0)
    {
        return false;
    }

    m_fragShader = compileShader(GL_FRAGMENT_SHADER, fragShaderFile);
    if (m_fragShader == 0)
    {
        return false;
    }

    m_program = linkProgram(m_vertexShader, m_fragShader);

    return true;
}

void ShaderProgram::use()
{
    glUseProgram(m_program);
}

} // end of namespace bot

