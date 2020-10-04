#include "misc/bot_log.h"
#include "opengl/bot_graphics.h"
#include "input/bot_input_event.h"
#include "gameutil/bot_game_lib.h"
#include "gameutil/bot_map_generator.h"
#include "gameutil/bot_game_map_loader.h"
#include "screen/bot_show_map_screen.h"
#include "app/bot_app_config.h"

namespace bot {

ShowMapScreen::ShowMapScreen()
    : m_lib(nullptr)
    , m_graphics(nullptr)
{
}

bool ShowMapScreen::init(const AppConfig& cfg, GameLib* lib, Graphics* g,
                         float viewportWidth, float viewportHeight)
{
    const std::string& generatorName = cfg.getMapGenerator();
    const std::string& mapFile = cfg.getMapFile();

    if (!generatorName.empty())
    {
        LOG_INFO("Generating map %s", mapFile.c_str());

        MapGenerator* generator = lib->getMapGenerator(generatorName);
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

    m_lib = lib;
    m_graphics = g;
    m_viewportSize[0] = viewportWidth;
    m_viewportSize[1] = viewportHeight;
    m_gameObjManager.init(&m_map, cfg, m_lib);

    GameMapLoader mapLoader(m_gameObjManager, cfg.getMapPoolFactor());
    if (!mapLoader.load(m_map, mapFile, 1, viewportWidth, viewportHeight))
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
    SimpleShaderProgram& simpleShaderProgram = m_graphics->getSimpleShader();

    simpleShaderProgram.use();
    simpleShaderProgram.setViewportSize(m_viewportSize);
    simpleShaderProgram.setViewportOrigin(m_map.getViewportPos());

    m_map.present(*m_graphics);
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

