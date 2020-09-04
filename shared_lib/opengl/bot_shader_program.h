#ifndef INCLUDE_BOT_SHADERPROGRAM
#define INCLUDE_BOT_SHADERPROGRAM

#include <string>

namespace bot {

class ShaderProgram {
public:
    ShaderProgram();

    virtual ~ShaderProgram();

    virtual bool init(const std::string& vertexShaderFile,
                      const std::string& fragShaderFile);

    void use();

protected:
    unsigned int m_vertexShader;
    unsigned int m_fragShader;
    unsigned int m_program;
};

} // end of namespace bot

#endif
