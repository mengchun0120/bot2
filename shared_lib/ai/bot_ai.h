#ifndef INCLUDE_BOT_AI
#define INCLUDE_BOT_AI

#include <string>
#include <rapidjson/document.h>

namespace bot {

class Robot;
class GameScreen;

class AI {
public:
    class Parser {
    public:
        Parser()
        {}

        ~Parser()
        {}

        AI* create(const std::string& name, const rapidjson::Value& elem);
    };

    static AI* create(const rapidjson::Value& elem);

    AI()
    {}

    virtual ~AI()
    {}

    virtual bool init(const rapidjson::Value& elem) = 0;

    virtual void apply(Robot& robot, float delta, GameScreen& screen) = 0;
};

} // end of namespace bot

#endif
