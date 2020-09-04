#ifndef INCLUDE_BOT_SHAPE
#define INCLUDE_BOT_SHAPE

#include "opengl/bot_vertex_array.h"

namespace bot {

class Graphics;
class Color;

class Shape {
public:
    Shape();

    virtual ~Shape();

    virtual void draw(Graphics& g, const float* pos, const float* direction,
                      const Color* fillColor, const Color* borderColor,
                      const unsigned int textureId, const Color* texColor) const = 0;

    const VertexArray& vertexArray() const
    {
        return m_vertexArray;
    }

protected:
    VertexArray m_vertexArray;
};

} // end of namespace bot

#endif
