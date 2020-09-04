#ifndef INCLUDE_BOT_PROGRESS_RING
#define INCLUDE_BOT_PROGRESS_RING

#include <string>
#include <rapidjson/document.h>
#include "opengl/bot_vertex_array.h"

namespace bot {

template <typename T> class NamedMap;
class Color;
class SimpleShaderProgram;

class ProgressRing {
public:
    class Parser {
    public:
        Parser(const NamedMap<Color>& colorLib)
            : m_colorLib(colorLib)
        {}

        ~Parser()
        {}

        ProgressRing* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Color>& m_colorLib;
    };

    ProgressRing();

    ~ProgressRing()
    {}

    bool init(const NamedMap<Color>& colorLib, const rapidjson::Value& elem);

    bool init(const Color* frontColor, const Color* backColor, float radius, int numEdges);

    void draw(Graphics& g, const float* pos, float percentage) const;

private:
    const Color* m_frontColor;
    const Color* m_backColor;
    VertexArray m_vertices;
    int m_maxIdx;
};

} // end of namespace bot

#endif
