#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "opengl/bot_vertex_array.h"
#include "opengl/bot_particle_shader_program.h"

namespace bot {

std::shared_ptr<ParticleShaderProgram> ParticleShaderProgram::k_program;

bool ParticleShaderProgram::initInstance(const std::string& vertexShaderFile,
                                         const std::string& fragShaderFile)
{
    ParticleShaderProgram* program = new ParticleShaderProgram();

    if (!program->init(vertexShaderFile, fragShaderFile))
    {
        delete program;
        return false;
    }

    k_program.reset(program);

    return true;
}

ParticleShaderProgram::ParticleShaderProgram()
    : m_viewportSizeLocation(-1)
    , m_viewportOriginLocation(-1)
    , m_refLocation(-1)
    , m_curTimeLocation(-1)
    , m_accelerationLocation(-1)
    , m_initSpeedLocation(-1)
    , m_particleSizeLocation(-1)
    , m_useTexLocation(-1)
    , m_textureLocation(-1)
    , m_colorLocation(-1)
    , m_directionLocation(-1)
{

}

ParticleShaderProgram::~ParticleShaderProgram()
{
}

bool ParticleShaderProgram::init(const std::string& vertexShaderFile,
                                 const std::string& fragShaderFile)
{
    if (!ShaderProgram::init(vertexShaderFile, fragShaderFile))
    {
        return false;
    }

    loadParam();

    return true;
}

void ParticleShaderProgram::loadParam()
{
    m_viewportSizeLocation = glGetUniformLocation(m_program, "viewportSize");
    m_viewportOriginLocation =
                            glGetUniformLocation(m_program, "viewportOrigin");
    m_refLocation = glGetUniformLocation(m_program, "ref");
    m_curTimeLocation = glGetUniformLocation(m_program, "curTime");
    m_accelerationLocation = glGetUniformLocation(m_program, "acceleration");
    m_initSpeedLocation = glGetUniformLocation(m_program, "initSpeed");
    m_particleSizeLocation = glGetUniformLocation(m_program, "particleSize");
    m_colorLocation = glGetUniformLocation(m_program, "color");
    m_useTexLocation = glGetUniformLocation(m_program, "useTex");
    m_textureLocation = glGetUniformLocation(m_program, "texture");
    m_directionLocation = glGetAttribLocation(m_program, "direction");
}

void ParticleShaderProgram::setTexture(int textureId)
{
    glUniform1i(m_textureLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void ParticleShaderProgram::bindData(const VertexArray& va)
{
    glBindVertexArray(va.vao());
    glBindBuffer(GL_ARRAY_BUFFER, va.vbo());

    glVertexAttribPointer(m_directionLocation, Constants::NUM_FLOATS_DIRECTION,
                          GL_FLOAT, GL_FALSE, va.stride(),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(m_directionLocation);
}

} // end of namespace bot
