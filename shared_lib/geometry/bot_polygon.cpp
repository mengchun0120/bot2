#include "opengl/bot_color.h"
#include "opengl/bot_graphics.h"
#include "geometry/bot_polygon.h"

namespace bot {

Polygon::Polygon()
: Shape()
{
}

Polygon::~Polygon()
{
}

bool Polygon::init(const float *vertices, unsigned int numVertices, bool hasTexCoord)
{
    unsigned int vertexSize = Constants::POSITION_SIZE;
    unsigned int stride = 0;
    if (hasTexCoord)
    {
        vertexSize += Constants::TEXCOORD_SIZE;
        stride += vertexSize;
    }
    return m_vertexArray.load(vertices, numVertices, vertexSize, stride);
}

void Polygon::draw(Graphics& g, const float* pos, const float* direction,
                   const Color* fillColor, const Color* borderColor,
                   const unsigned int textureId, const Color* texColor) const
{
    SimpleShaderProgram& program = g.getSimpleShader();

    if (pos)
    {
        program.setUseObjRef(true);
        program.setObjRef(pos);
    }
    else
    {
        program.setUseObjRef(false);
    }

    program.setPosition(m_vertexArray, textureId != 0);
    program.setUseColor(textureId == 0);

    if (direction)
    {
        program.setUseDirection(true);
        program.setDirection(direction);
    }
    else
    {
        program.setUseDirection(false);
    }

    if (textureId == 0)
    {
        if (fillColor)
        {
            program.setColor(fillColor->getColor());
            glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertexArray.numVertices());
        }

        if (borderColor)
        {
            program.setColor(borderColor->getColor());
            glDrawArrays(GL_LINE_LOOP, 1, m_vertexArray.numVertices()-2);
        }
    }
    else
    {
        program.setTexture(textureId);

        if(texColor)
        {
            program.setUseTexColor(true);
            program.setTexColor(texColor->getColor());
        }
        else
        {
            program.setUseTexColor(false);
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertexArray.numVertices());
    }
}

} // end of namespace bot
