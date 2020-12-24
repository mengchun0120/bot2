#ifndef INCLUDE_BOT_POLYGON
#define INCLUDE_BOT_POLYGON

#include "opengl/bot_texture.h"
#include "geometry/bot_shape.h"

namespace bot {

class Polygon: public Shape {
public:
    Polygon();

    virtual ~Polygon();

    bool init(const float *vertices, unsigned int numVertices,
              bool hasTexCoord);

    virtual void draw(const float* pos, const float* direction,
                      const Color* fillColor, const Color* borderColor,
                      unsigned int textureId,
                      const Color* texColor) const;

    virtual void draw(const float* pos, const float* direction,
                      const Color* fillColor, const Color* borderColor,
                      const Texture& texture, const Color* texColor) const
    {
        draw(pos, direction, fillColor, borderColor,
             texture.textureId(), texColor);
    }
};

} // end of namespace bot

#endif
