#ifndef INCLUDE_BOT_PROGRESS_RING
#define INCLUDE_BOT_PROGRESS_RING

#include <vector>
#include <string>
#include <rapidjson/document.h>
#include "opengl/bot_vertex_array.h"

namespace bot {

class Color;

class ProgressRing {
public:
    ProgressRing();

    virtual ~ProgressRing()
    {}

    bool init(const rapidjson::Value& elem);

    float getRadius() const
    {
        return m_radius;
    }

    void draw(const float* pos, float percentage) const;

private:
    bool initVertexArray(int numEdges);

    const Color* getFrontColor(float percentage) const;

private:
    std::vector<const Color*> m_frontColors;
    const Color* m_backColor;
    VertexArray m_vertices;
    float m_radius;
    int m_maxIdx;
    int m_numFrontColors;
};

} // end of namespace bot

#endif
