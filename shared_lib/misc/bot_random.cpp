#include <chrono>
#include "misc/bot_random.h"

namespace bot {

Random::Random()
    : m_distribution(0.0, 1.0)
{
    unsigned s = std::chrono::system_clock::now().time_since_epoch().count();
    m_generator.seed(s);
}

} // end of namespace bot

