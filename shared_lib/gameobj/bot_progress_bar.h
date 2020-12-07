#ifndef INCLUDE_BOT_PROGRESS_BAR
#define INCLUDE_BOT_PROGRESS_BAR

#include "misc/bot_constants.h"
#include "opengl/bot_vertex_array.h"

namespace bot {

class ProgressBarTemplate;

class ProgressBar {
public:
    ProgressBar();

    ~ProgressBar()
    {}

    bool init(const ProgressBarTemplate* t, float x, float y,
              float initialRatio=1.0f);

    bool setRatio(float ratio);

    void draw();

private:
    void initBar();

private:
    const ProgressBarTemplate* m_template;
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    int m_visibleVertices;
    VertexArray m_bar;
};

} // end of namespace bot

#endif

