#ifndef INCLUDE_BOT_DASHBOARD_CONFIG
#define INCLUDE_BOT_DASHBOARD_CONFIG

#include <string>

namespace bot {

class DashboardConfig {
public:
    DashboardConfig()
        : m_effectSpacingX(0.0f)
        , m_effectStartX(0.0f)
        , m_effectRingRadius(0.0f)
        , m_effectMargin(0.0f)
        , m_barSpacing(0.0f)
        , m_barMargin(0.0f)
    {}

    ~DashboardConfig()
    {}

    bool init(const std::string& configFile);

    float getEffectSpacingX() const
    {
        return m_effectSpacingX;
    }

    float getEffectStartX() const
    {
        return m_effectStartX;
    }

    float getEffectRingRadius() const
    {
        return m_effectRingRadius;
    }

    float getEffectMargin() const
    {
        return m_effectMargin;
    }

    float getBarSpacing() const
    {
        return m_barSpacing;
    }

    float getBarMargin() const
    {
        return m_barMargin;
    }

private:
    float m_effectSpacingX;
    float m_effectStartX;
    float m_effectRingRadius;
    float m_effectMargin;
    float m_barSpacing;
    float m_barMargin;
};

} // end of namespace bot

#endif
