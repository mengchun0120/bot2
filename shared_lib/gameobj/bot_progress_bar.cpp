#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "gametemplate/bot_progress_bar_template.h"
#include "gameobj/bot_progress_bar.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"

namespace bot {

ProgressBar::ProgressBar()
    : m_template(nullptr)
{
}

bool ProgressBar::init(const ProgressBarTemplate* t, float x, float y,
                       float initialRatio)
{
    if (!t)
    {
        LOG_ERROR("ProgressBarTemplate is null");
        return false;
    }

    m_template = t;
    m_pos[0] = x + t->getTexture()->width() / 2.0f;
    m_pos[1] = y + t->getTexture()->height() / 2.0f;

    if (!initBar(x, y))
    {
        return false;
    }

    return true;
}

bool ProgressBar::initBar(float x, float y)
{
    const int NUM_VERTICES = (m_template->getSlotCount() + 1) * 2;
    const int NUM_FLOATS = NUM_VERTICES * Constants::NUM_FLOATS_PER_POSITION;
    std::vector<float> vertices;

    vertices.resize(NUM_FLOATS);

    float barX, barBottom, barTop, deltaX;

    deltaX = m_template->getBarWidth() / m_template->getSlotCount();
    barX = x + m_template->getStartOffsetX();
    barBottom = y + m_template->getStartOffsetY();
    barTop = barBottom + m_template->getBarHeight();

    for (int i = 0, j = 0; i <= m_template->getSlotCount(); ++i)
    {
        vertices[j++] = barX;
        vertices[j++] = barBottom;
        vertices[j++] = barX;
        vertices[j++] = barTop;
        barX += deltaX;
    }

    bool ret = m_bar.load(vertices.data(), NUM_VERTICES,
                          Constants::POSITION_SIZE, 0);
    if (!ret)
    {
        LOG_ERROR("Failed to create VertexArray for progress-bar");
        return false;
    }

    return true;
}

void ProgressBar::draw(float ratio)
{
    ratio = clamp(ratio, 0.0f, 1.0f);

    int barCount = static_cast<int>(ratio * (m_template->getSlotCount()));
    int visibleVertices = barCount > 0 ? (barCount + 1) * 2 : 0;

    m_template->getRect()->draw(m_pos, nullptr, nullptr, nullptr,
                                m_template->getTexture()->textureId(), nullptr);

    if (visibleVertices > 0)
    {
        SimpleShaderProgram& program = SimpleShaderProgram::getInstance();

        program.setUseObjRef(false);
        program.setPosition(m_bar, false);
        program.setUseColor(true);
        program.setUseDirection(false);
        program.setColor(m_template->getColor()->getColor());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, visibleVertices);
    }
}

} // end of namespace bot

