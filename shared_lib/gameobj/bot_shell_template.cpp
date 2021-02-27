#include "misc/bot_json_utils.h"
#include "gameobj/bot_shell_template.h"

namespace bot {

ShellTemplate::ShellTemplate()
    : MissileTemplate()
    , m_explodeBreath(0.0f)
{
}

bool ShellTemplate::init(const rapidjson::Value &elem)
{
    if (!MissileTemplate::init(MISSILE_SHELL, elem))
    {
        return false;
    }

    std::vector<JsonParamPtr> params = {
        jsonParam(m_explodeBreath, "explodeBreath", gt(m_explodeBreath, 0.0f))
    };

    return parseJson(params, elem);
}

} // end of namespace bot

