#ifndef INCLUDE_BOT_GOODIE_GENERATOR
#define INCLUDE_BOT_GOODIE_GENERATOR

#include <vector>
#include <random>

namespace bot {

template <typename T> class NamedMap;
class GoodieTemplate;

class GoodieGenerator {
public:
    GoodieGenerator();

    ~GoodieGenerator()
    {}

    void init(const NamedMap<GoodieTemplate>& goodieLib);

    int generate(float prob);

private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<float> m_distribution;
    float m_totalWeight;
    int m_maxGoodieIdx;
    std::vector<float> m_weightSums;
};

} // end of namespace bot

#endif
