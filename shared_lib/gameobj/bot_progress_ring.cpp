#include <cmath>
#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_color.h"
#include "opengl/bot_graphics.h"
#include "gameobj/bot_progress_ring.h"

namespace bot {

ProgressRing* ProgressRing::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    ProgressRing* ring = new ProgressRing();
    if (!ring->init(m_colorLib, elem))
    {
        delete ring;
        return nullptr;
    }
    return ring;
}

ProgressRing::ProgressRing()
    : m_frontColor(nullptr)
    , m_backColor(nullptr)
    , m_maxIdx(0)
{
}

bool ProgressRing::init(const NamedMap<Color>& colorLib, const rapidjson::Value& elem)
{
    std::string backColorName, frontColorName;
    float radius;
    int numEdges;

    std::vector<JsonParseParam> params = {
        {&backColorName,  "backColor",  JSONTYPE_STRING},
        {&frontColorName, "frontColor", JSONTYPE_STRING},
        {&radius,         "radius",     JSONTYPE_FLOAT},
        {&numEdges,       "numEdges",   JSONTYPE_INT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const Color* backColor = colorLib.search(backColorName);
    if (!backColor)
    {
        LOG_ERROR("Failed to find backColor %s", backColorName.c_str());
        return false;
    }

    const Color* frontColor = colorLib.search(frontColorName);
    if (!frontColor)
    {
        LOG_ERROR("Failed to find frontColor %s", frontColorName.c_str());
        return false;
    }

    if (!init(frontColor, backColor, radius, numEdges))
    {
        LOG_ERROR("Failed to initialize progress ring");
        return false;
    }

    return true;
}

bool ProgressRing::init(const Color* frontColor, const Color* backColor, float radius, int numEdges)
{
    m_frontColor = frontColor;
    m_backColor = backColor;
    m_maxIdx = numEdges - 1;

    int numFloatsPerTriangle = Constants::NUM_FLOATS_PER_POSITION * 3;
    int numFloats = numEdges * numFloatsPerTriangle;
    float* vertices = new float[numFloats];
    float delta = 2.0f * Constants::PI / numEdges;
    float theta = delta;
    float prevX = 0.0f, prevY = radius;

    for (int i = 0, k = 0; i < numEdges; ++i, k += numFloatsPerTriangle, theta += delta)
    {
        vertices[k] = 0.0f;
        vertices[k + 1] = 0.0f;
        vertices[k + 2] = prevX;
        vertices[k + 3] = prevY;
        prevX = sin(theta) * radius;
        prevY = cos(theta) * radius;
        vertices[k + 4] = prevX;
        vertices[k + 5] = prevY;
    }

    bool ret = m_vertices.load(vertices, numEdges * 3, Constants::POSITION_SIZE, 0);
    delete[] vertices;

    return ret;
}

void ProgressRing::draw(Graphics& g, const float* pos, float percentage) const
{
    SimpleShaderProgram& program = g.getSimpleShader();

    program.setUseObjRef(true);
    program.setObjRef(pos);
    program.setPosition(m_vertices, false);
    program.setUseColor(true);
    program.setUseDirection(false);

    int finishedIdx = static_cast<int>(percentage * m_maxIdx);

    if (finishedIdx < 0)
    {
        finishedIdx = 0;
    }
    else if(finishedIdx > m_maxIdx)
    {
        finishedIdx = m_maxIdx;
    }

    int finishedVertices = (finishedIdx + 1) * 3;

    program.setColor(m_backColor->getColor());
    glDrawArrays(GL_TRIANGLES, 0, finishedVertices);

    if (finishedVertices < static_cast<int>(m_vertices.numVertices()))
    {
        program.setColor(m_frontColor->getColor());
        glDrawArrays(GL_TRIANGLES, finishedVertices, m_vertices.numVertices() - finishedVertices);
    }
}

} // end of namespace bot
