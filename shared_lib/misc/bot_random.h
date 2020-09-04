#ifndef INCLUDE_BOT_RANDOM
#define INCLUDE_BOT_RANDOM

#include <random>

namespace bot {

class Random {
public:
    Random()
        : m_generator(std::random_device()())
        , m_distribution(0.0, 1.0)
    {}

    ~Random()
    {}

    int get(int lower, int upper)
    {
        return lower + static_cast<int>(m_distribution(m_generator) * (upper - lower));
    }

    float get(float lower, float upper)
    {
        return lower + m_distribution(m_generator) * (upper - lower);
    }

    double get(double lower, double upper)
    {
        return lower + m_distribution(m_generator) * (upper - lower);
    }

private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<float> m_distribution;
};

} // end of namespce bot

#endif
