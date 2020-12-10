#include "misc/bot_log.h"
#include "input/bot_input_event.h"
#include "opengl/bot_simple_shader_program.h"
#include "gameutil/bot_game_lib.h"
#include "gameutil/bot_map_generator.h"
#include "gameutil/bot_game_map_loader.h"
#include "screen/bot_show_map_screen.h"
#include "app/bot_app_config.h"
#include "app/bot_app.h"

namespace bot {

ShowMapScreen::ShowMapScreen()
{
}

bool ShowMapScreen::init()
{
    const AppConfig& cfg = AppConfig::getInstance();
    const std::string& generatorName = cfg.getMapGenerator();
    const std::string& mapFile = cfg.getMapFile();
    GameLib& lib = GameLib::getInstance();

    if (!generatorName.empty())
    {
        LOG_INFO("Generating map %s", mapFile.c_str());

        MapGenerator* generator = lib.getMapGenerator(generatorName);
        if (!generator)
        {
            LOG_ERROR("Failed to find map generator %s", generatorName.c_str());
            return false;
        }

        if (!generator->generate(mapFile.c_str()))
        {
            LOG_ERROR("Failed to generate map %s", mapFile.c_str());
            return false;
        }

        LOG_INFO("Done generating map %s", mapFile.c_str());
    }

    m_gameObjManager.init(nullptr);

    const App& app = App::getInstance();

    GameMapLoader mapLoader(m_gameObjManager, cfg.getMapPoolFactor());
    bool success = mapLoader.load(m_map, mapFile, 1, app.getViewportWidth(),
                                  app.getViewportHeight());
    if (!success)
    {
        return false;
    }

    LOG_INFO("Done loading map");

    return true;
}

int ShowMapScreen::update(float delta)
{
    return 0;
}

void ShowMapScreen::present()
{
    SimpleShaderProgram& program = SimpleShaderProgram::getInstance();
    const App& app = App::getInstance();

    program.use();
    program.setViewportSize(app.getViewportSize());
    program.setViewportOrigin(m_map.getViewportPos());

    m_map.present();
}

int ShowMapScreen::processInput(const InputEvent &e)
{
    switch(e.m_type)
    {
        case InputEvent::ET_MOUSE_MOVE:
            return 0;
        case InputEvent::ET_MOUSE_BUTTON:
            return 0;
        case InputEvent::ET_KEY:
            return handleKey(e.m_keyEvent);
        default:
            LOG_ERROR("Invalid input type %d", static_cast<int>(e.m_type));
    }

    return 0;
}

int ShowMapScreen::handleKey(const KeyEvent& e)
{
    const float* viewportPos = m_map.getViewportPos();

    switch(e.m_key)
    {
        case GLFW_KEY_UP:
        {
            m_map.setViewportPos(viewportPos[0], viewportPos[1] + 20.0f);
            return 0;
        }
        case GLFW_KEY_DOWN:
        {
            m_map.setViewportPos(viewportPos[0], viewportPos[1] - 20.0f);
            return 0;
        }
        case GLFW_KEY_LEFT:
        {
            m_map.setViewportPos(viewportPos[0] - 20.0f, viewportPos[1]);
            return 0;
        }
        case GLFW_KEY_RIGHT:
        {
            m_map.setViewportPos(viewportPos[0] + 20.0f, viewportPos[1]);
            return 0;
        }
        case GLFW_KEY_ESCAPE:
        {
            return 2;
        }
    }

    return 0;
}

} // end of namespace bot

