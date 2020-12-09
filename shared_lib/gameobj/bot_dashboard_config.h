#ifndef INCLUDE_BOT_DASHBOARD_CONFIG
#define INCLUDE_BOT_DASHBOARD_CONFIG

#include <string>

namespace bot {

class DashboardConfig {
public:
    DashboardConfig()
        : m_effectSpacing(0.0f)
        , m_effectRingRadius(0.0f)
        , m_effectMargin(0.0f)
        , m_barSpacing(0.0f)
        , m_barMargin(0.0f)
    {}

    ~DashboardConfig()
    {}

    bool init(const std::string& configFile);

    float getEffectSpacing() const
    {
        return m_effectSpacing;
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

    float getStatusMargin() const
    {
        return m_statusMargin;
    }

    float getStatusSpacing() const
    {
        return m_statusSpacing;
    }

private:
    float m_effectSpacing;
    float m_effectRingRadius;
    float m_effectMargin;
    float m_barSpacing;
    float m_barMargin;
    float m_statusMargin;
    float m_statusSpacing;
};

} // end of namespace bot

#endif
