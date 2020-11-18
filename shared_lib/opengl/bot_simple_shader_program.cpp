#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "opengl/bot_vertex_array.h"
#include "opengl/bot_simple_shader_program.h"


namespace bot {

std::shared_ptr<SimpleShaderProgram> SimpleShaderProgram::k_program;

bool SimpleShaderProgram::initInstance(const std::string& vertexShaderFile,
                                       const std::string& fragShaderFile)
{
    SimpleShaderProgram* program = new SimpleShaderProgram();
    k_program.reset(program);

    if (!program->init(vertexShaderFile, fragShaderFile))
    {
        k_program.reset();
        return false;
    }

    return true;
}

SimpleShaderProgram::SimpleShaderProgram()
    : ShaderProgram()
    , m_positionLocation(-1)
    , m_useObjRefLocation(-1)
    , m_objRefLocation(-1)
    , m_viewportSizeLocation(-1)
    , m_viewportOriginLocation(-1)
    , m_colorLocation(-1)
    , m_useColorLocation(-1)
    , m_texPosLocation(-1)
    , m_textureLocation(-1)
    , m_useDirectionLocation(-1)
    , m_directionLocation(-1)
    , m_useTexColorLocation(-1)
    , m_texColorLocation(-1)
{
}

SimpleShaderProgram::~SimpleShaderProgram()
{
}

bool SimpleShaderProgram::init(const std::string& vertexShaderFile,
                               const std::string& fragShaderFile)
{
    if (!ShaderProgram::init(vertexShaderFile, fragShaderFile))
    {
        return false;
    }

    loadParam();

    return true;
}

void SimpleShaderProgram::loadParam()
{
    m_positionLocation = glGetAttribLocation(m_program, "position");
    m_useObjRefLocation = glGetUniformLocation(m_program, "useObjRef");
    m_objRefLocation = glGetUniformLocation(m_program, "objRef");
    m_viewportSizeLocation = glGetUniformLocation(m_program, "viewportSize");
    m_viewportOriginLocation =
                        glGetUniformLocation(m_program, "viewportOrigin");
    m_colorLocation = glGetUniformLocation(m_program, "color");
    m_useColorLocation = glGetUniformLocation(m_program, "useColor");
    m_texPosLocation = glGetAttribLocation(m_program, "texPos");
    m_textureLocation = glGetUniformLocation(m_program, "texture");
    m_useDirectionLocation = glGetUniformLocation(m_program, "useDirection");
    m_directionLocation = glGetUniformLocation(m_program, "direction");
    m_useTexColorLocation = glGetUniformLocation(m_program, "useTexColor");
    m_texColorLocation = glGetUniformLocation(m_program, "texColor");
}

void SimpleShaderProgram::setTexture(unsigned int textureId)
{
    glUniform1i(m_textureLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void SimpleShaderProgram::setPosition(const VertexArray& vertexArray,
                                      bool hasTexCoord)
{
    glBindVertexArray(vertexArray.vao());
    glBindBuffer(GL_ARRAY_BUFFER, vertexArray.vbo());

    glVertexAttribPointer(m_positionLocation,
                          Constants::NUM_FLOATS_PER_POSITION,
                          GL_FLOAT, GL_FALSE,
                          vertexArray.stride(), (void *)0);
    glEnableVertexAttribArray(m_positionLocation);

    if (hasTexCoord)
    {
        glVertexAttribPointer(m_texPosLocation,
                              Constants::NUM_FLOATS_PER_TEXCOORD,
                              GL_FLOAT, GL_FALSE, vertexArray.stride(),
                              (void *)(Constants::POSITION_SIZE));
        glEnableVertexAttribArray(m_texPosLocation);
    }
}

} // end of namespace bot

