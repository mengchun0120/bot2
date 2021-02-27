#include <cmath>
#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "opengl/bot_simple_shader_program.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

ProgressRing::ProgressRing()
    : m_backColor(nullptr)
    , m_radius(0.0f)
    , m_maxIdx(0)
    , m_numFrontColors(0)
{
}

bool ProgressRing::init(const rapidjson::Value &elem)
{
    std::string backColorName;
    std::vector<std::string> frontColorNames;
    int numEdges;

    std::vector<JsonParamPtr> params = {
        jsonParam(backColorName, "backColor"),
        jsonParam(frontColorNames, "frontColors"),
        jsonParam(m_radius, "radius", gt(m_radius, 0.0f)),
        jsonParam(numEdges, "numEdges", gt(numEdges, 3))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib &lib = GameLib::getInstance();

    m_backColor = lib.getColor(backColorName);
    if (!m_backColor)
    {
        LOG_ERROR("Failed to find backColor %s", backColorName.c_str());
        return false;
    }

    m_numFrontColors = static_cast<int>(frontColorNames.size());
    m_frontColors.resize(m_numFrontColors);

    for (int i = 0; i < m_numFrontColors; ++i)
    {
        const Color *color = lib.getColor(frontColorNames[i]);
        if (!color)
        {
            LOG_ERROR("Failed to find frontColor %s",
                      frontColorNames[i].c_str());
            return false;
        }

        m_frontColors[i] = color;
    }

    if (!initVertexArray(numEdges))
    {
        LOG_ERROR("Failed to initialize vertex array for progress ring");
        return false;
    }

    return true;
}

bool ProgressRing::initVertexArray(int numEdges)
{
    m_maxIdx = numEdges - 1;

    int numFloatsPerTriangle = Constants::NUM_FLOATS_PER_POSITION * 3;
    int numFloats = numEdges * numFloatsPerTriangle;
    float *vertices = new float[numFloats];
    float delta = 2.0f * Constants::PI / numEdges;
    float theta = delta;
    float prevX = 0.0f, prevY = m_radius;

    for (int i = 0, k = 0; i < numEdges; ++i)
    {
        vertices[k] = 0.0f;
        vertices[k + 1] = 0.0f;
        vertices[k + 2] = prevX;
        vertices[k + 3] = prevY;
        prevX = sin(theta) * m_radius;
        prevY = cos(theta) * m_radius;
        vertices[k + 4] = prevX;
        vertices[k + 5] = prevY;

        k += numFloatsPerTriangle;
        theta += delta;
    }

    bool ret = m_vertices.load(vertices, numEdges * 3,
                               Constants::POSITION_SIZE, 0);
    delete[] vertices;

    return ret;
}

void ProgressRing::draw(const float *pos, float percentage) const
{
    SimpleShaderProgram &program = SimpleShaderProgram::getInstance();

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
        const Color *frontColor = getFrontColor(percentage);
        program.setColor(frontColor->getColor());
        glDrawArrays(GL_TRIANGLES, finishedVertices,
                     m_vertices.numVertices() - finishedVertices);
    }
}

const Color *ProgressRing::getFrontColor(float percentage) const
{
    int idx = static_cast<int>(floor(percentage * m_numFrontColors));

    if (idx < 0)
    {
        idx = 0;
    }
    else if (idx >= m_numFrontColors)
    {
        idx = m_numFrontColors - 1;
    }

    return m_frontColors[idx];
}

} // end of namespace bot

