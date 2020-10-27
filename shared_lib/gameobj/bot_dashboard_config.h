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
        , m_headerTopMargin(0.0f)
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

    float getHeaderTopMargin() const
    {
        return m_headerTopMargin;
    }

private:
    float m_effectSpacingX;
    float m_effectStartX;
    float m_effectRingRadius;
    float m_headerTopMargin;
};

} // end of namespace bot

#endif
