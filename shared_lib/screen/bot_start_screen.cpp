#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "opengl/bot_simple_shader_program.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_game_lib.h"
#include "widget/bot_button.h"
#include "screen/bot_start_screen_config.h"
#include "screen/bot_start_screen.h"
#include "screen/bot_screen_manager.h"
#include "app/bot_app.h"

namespace bot {

StartScreen::StartScreen()
{
}

StartScreen::~StartScreen()
{
}

bool StartScreen::init()
{
    const App &app = App::getInstance();
    float viewportWidth = app.getViewportWidth();
    float viewportHeight = app.getViewportHeight();

    m_viewportOrigin[0] = viewportWidth / 2.0f;
    m_viewportOrigin[1] = viewportHeight / 2.0f;

    const GameLib &lib = GameLib::getInstance();

    const StartScreenConfig &cfg = lib.getStartScreenConfig();
    float spacing = cfg.getButtonSpacing();
    float width = cfg.getButtonWidth();
    float height = cfg.getButtonHeight();
    const Rectangle &rect = cfg.getButtonRect();
    const std::vector<std::string> &buttonTexts = cfg.getButtonTexts();

    int n = static_cast<int>(buttonTexts.size());
    float x = (viewportWidth - width) / 2.0f;
    float y = (viewportHeight + height + (n - 1) * spacing) / 2.0f;
    float deltaY = height + spacing;

    std::vector<Button::ActionFunc> funcs = {
        std::bind(&StartScreen::startGame, this),
        std::bind(&StartScreen::loadGame, this),
        std::bind(&StartScreen::showSettings, this),
        std::bind(&StartScreen::exitApp, this)
    };

    m_buttons.init(n);
    for (int i = 0; i < n; ++i, y -= deltaY) {
        Button *button = new Button();
        if (!button->init(x, y, width, height, &rect, buttonTexts[i]))
        {
            LOG_ERROR("Failed to initialize start game button");
            return false;
        }
        button->setActionFunc(funcs[i]);
        m_buttons.setWidget(i, button);
    }

    return true;
}

int StartScreen::update(float delta)
{
    return 0;
}

void StartScreen::present()
{
    SimpleShaderProgram &program = SimpleShaderProgram::getInstance();
    const App &app = App::getInstance();

    program.use();
    program.setViewportSize(app.getViewportSize());
    program.setViewportOrigin(m_viewportOrigin);

    m_buttons.present();
}

int StartScreen::processInput(const InputEvent &e)
{
    return m_buttons.processInput(e);
}

int StartScreen::startGame()
{
    ScreenManager &screenMgr = ScreenManager::getInstance();
    screenMgr.switchScreen(Screen::SCREEN_GAME);
    return 1;
}

int StartScreen::loadGame()
{
    return 0;
}

int StartScreen::showSettings()
{
    return 0;
}

int StartScreen::exitApp()
{
    return 2;
}

} // end of namespace bot
