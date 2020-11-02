#ifndef INCLUDE_BOT_JSON_VALIDATOR
#define INCLUDE_BOT_JSON_VALIDATOR

#include <string>

namespace bot {

template <typename T>
class Validator {
public:
    Validator()
    {}

    virtual ~Validator()
    {}

    virtual bool validate(const T& t, const std::string& name) = 0;
};

template <typename T>
class Validator {
};

} // end of namespace bot

#endif

