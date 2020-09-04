#ifndef INCLUDE_BOT_TIME_DELTA_SMOOTHER
#define INCLUDE_BOT_TIME_DELTA_SMOOTHER

#include <vector>
#include "misc/bot_time_utils.h"

namespace bot {

class TimeDeltaSmoother {
public:
    TimeDeltaSmoother();

    virtual ~TimeDeltaSmoother();

    void init(int timeDeltaHistoryLen);

    void start();

    float getTimeDelta();

private:
    std::vector<float> m_timeDeltaHistory;
    float m_sumTimeDelta;
    unsigned int m_totalSlots;
    unsigned int m_lastSlot;
    TimePoint m_prevTime;
};

} // end of namespace bot

#endif

