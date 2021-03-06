#include "misc/bot_log.h"
#include "misc/bot_json_parser.h"
#include "bot_chase_shoot_ai.h"

namespace bot {

AI *AI::create(const std::string &name, const rapidjson::Value &elem)
{
    std::string algorithm;
    if (!JsonParser::parse(algorithm, elem, "algorithm"))
    {
        return nullptr;
    }

    ChaseShootAI *ai = nullptr;

    if (algorithm == "chase_shoot")
    {
        ai = new ChaseShootAI();
    }
    else
    {
        LOG_ERROR("Unsupported AI algorithm");
        return nullptr;
    }

    if (!ai->init(elem))
    {
        delete ai;
        return nullptr;
    }

    return ai;
}

} // end of namespace bot
