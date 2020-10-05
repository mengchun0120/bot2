#include "opengl/bot_graphics.h"

namespace bot {

bool Graphics::init(const std::string& simpleVertexShaderFile,
                    const std::string& simpleFragShaderFile,
                    const std::string& particleVertexShaderFile,
                    const std::string& particleFragShaderFile,
                    const std::string& fontFolder)
{
    bool ret = m_simpleProgram.init(simpleVertexShaderFile,
                                    simpleFragShaderFile);
    if (!ret)
    {
        return false;
    }

    ret = m_particleProgram.init(particleVertexShaderFile,
                                 particleFragShaderFile);
    if (!ret)
    {
        return false;
    }

    ret = m_textSys.init(fontFolder);
    if (!ret)
    {
        return false;
    }

    return true;
}

} // end of namespace bot
