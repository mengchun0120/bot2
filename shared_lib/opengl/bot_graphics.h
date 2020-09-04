#ifndef INCLUDE_BOT_GRAPHICS
#define INCLUDE_BOT_GRAPHICS

#include <string>
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_particle_shader_program.h"
#include "opengl/bot_text_system.h"

namespace bot {

class Graphics {
public:
    Graphics()
    {}

    ~Graphics()
    {}

    bool init(const std::string& simpleVertexShaderFile, const std::string& simpleFragShaderFile,
              const std::string& particleVertexShaderFile, const std::string& particleFragShaderFile,
              const std::string& fontFolder);

    SimpleShaderProgram& getSimpleShader()
    {
        return m_simpleProgram;
    }

    ParticleShaderProgram& getParticleShader()
    {
        return m_particleProgram;
    }

    TextSystem& getTextSystem()
    {
        return m_textSys;
    }

private:
    SimpleShaderProgram m_simpleProgram;
    ParticleShaderProgram m_particleProgram;
    TextSystem m_textSys;
};

} // end of namespace bot

#endif
