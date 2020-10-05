#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "opengl/bot_graphics.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_game_lib.h"
#include "widget/bot_button.h"
#include "screen/bot_start_screen_config.h"
#include "screen/bot_start_screen.h"
#include "screen/bot_screen_manager.h"

namespace bot {

StartScreen::StartScreen()
    : m_lib(nullptr)
    , m_graphics(nullptr)
    , m_screenManager(nullptr)
{
}

StartScreen::~StartScreen()
{
}

bool StartScreen::init(const GameLib* lib, float viewportWidth,
                       float viewportHeight, ScreenManager* screenManager,
                       Graphics* graphics)
{
    m_lib = lib;
    m_graphics = graphics;
    m_screenManager = screenManager;
    m_viewportSize[0] = viewportWidth;
    m_viewportSize[1] = viewportHeight;
    m_viewportOrigin[0] = viewportWidth / 2.0f;
    m_viewportOrigin[1] = viewportHeight / 2.0f;

    const StartScreenConfig& cfg = m_lib->getStartScreenConfig();
    const Rectangle* rect = cfg.getButtonRect();
    float spacing = cfg.getButtonSpacing();
    const std::vector<std::string>& buttonTexts = cfg.getButtonTexts();

    int n = static_cast<int>(buttonTexts.size());
    float x = (viewportWidth - rect->width()) / 2.0f;
    float y = (viewportHeight + n * rect->height() + (n - 1) * spacing) / 2.0f;
    float deltaY = rect->height() + spacing;

    std::vector<Button::ActionFunc> funcs = {
        std::bind(&StartScreen::startGame, this),
        std::bind(&StartScreen::loadGame, this),
        std::bind(&StartScreen::showSettings, this),
        std::bind(&StartScreen::exitApp, this)
    };

    m_buttons.init(n);
    for (int i = 0; i < n; ++i, y -= deltaY) {
        Button* button = new Button();
        if (!button->init(&m_lib->getButtonConfig(), rect, buttonTexts[i]))
        {
            LOG_ERROR("Failed to initialize start game button");
            return false;
        }
        button->setPos(m_graphics->getTextSystem(), x, y);
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
    SimpleShaderProgram& program = m_graphics->getSimpleShader();

    program.use();
    program.setViewportSize(m_viewportSize);
    program.setViewportOrigin(m_viewportOrigin);

    m_buttons.present(*m_graphics);
}

int StartScreen::processInput(const InputEvent& e)
{
    return m_buttons.processInput(e);
}

int StartScreen::startGame()
{
    m_screenManager->switchScreen(Screen::SCREEN_GAME);
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
