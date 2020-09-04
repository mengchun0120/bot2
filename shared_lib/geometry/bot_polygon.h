#ifndef INCLUDE_BOT_POLYGON
#define INCLUDE_BOT_POLYGON

#include "geometry/bot_shape.h"

namespace bot {

class Polygon: public Shape {
public:
    Polygon();

    virtual ~Polygon();

    bool init(const float *vertices, unsigned int numVertices, bool hasTexCoord);

    virtual void draw(Graphics& g, const float* pos, const float* direction,
                      const Color* fillColor, const Color* borderColor,
                      const unsigned int textureId, const Color* texColor) const;
};

} // end of namespace bot

#endif
