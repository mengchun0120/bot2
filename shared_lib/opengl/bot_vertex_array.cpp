#include <GL/glew.h>
#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "opengl/bot_vertex_array.h"

namespace bot {

VertexArray::VertexArray()
: m_numVertices(0)
, m_vertexSize(0)
, m_stride(0)
, m_vao(0)
, m_vbo(0)
{
}

VertexArray::~VertexArray()
{
    destroy();
}

bool VertexArray::load(const float* vertices, unsigned int numVertices,
                       unsigned int vertexSize, unsigned int stride)
{
    glGenVertexArrays(1, &m_vao);
    if(m_vao == 0) {
        LOG_ERROR("Failed to generate VAO: %d", glGetError());
        return false;
    }

    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    if(m_vbo == 0) {
        LOG_ERROR("Failed to generate VBO: %d", glGetError());
        return false;
    }

    m_numVertices = numVertices;
    m_vertexSize = vertexSize;
    m_stride = stride;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, vertices, GL_STATIC_DRAW);

    return true;
}

void VertexArray::destroy()
{
    if(m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }

    if(m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
}

} // end of namespace bot

