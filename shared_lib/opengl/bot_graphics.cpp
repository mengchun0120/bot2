#include "opengl/bot_graphics.h"

namespace bot {

bool Graphics::init(const std::string& simpleVertexShaderFile, const std::string& simpleFragShaderFile,
                    const std::string& particleVertexShaderFile, const std::string& particleFragShaderFile,
                    const std::string& fontFolder)
{
    if (!m_simpleProgram.init(simpleVertexShaderFile, simpleFragShaderFile))
    {
        return false;
    }

    if (!m_particleProgram.init(particleVertexShaderFile, particleFragShaderFile))
    {
        return false;
    }

    if (!m_textSys.init(fontFolder))
    {
        return false;
    }

    return true;
}

} // end of namespace bot
