#ifndef INCLUDE_BOT_VERTEX_ARRAY
#define INCLUDE_BOT_VERTEX_ARRAY

namespace bot {

class VertexArray {
public:
    VertexArray();

    virtual ~VertexArray();

    bool load(const float* vertices, unsigned int numVertices,
              unsigned int vertexSize,  unsigned int stride);

    unsigned int numVertices() const
    {
        return m_numVertices;
    }

    unsigned int vertexSize() const
    {
        return m_vertexSize;
    }

    unsigned int stride() const
    {
        return m_stride;
    }

    unsigned int vao() const
    {
        return m_vao;
    }

    unsigned int vbo() const
    {
        return m_vbo;
    }

protected:
    void destroy();

    unsigned int m_numVertices;
    unsigned int m_vertexSize;
    unsigned int m_stride;
    unsigned int m_vao;
    unsigned int m_vbo;
};

} // end of namespace bot

#endif
