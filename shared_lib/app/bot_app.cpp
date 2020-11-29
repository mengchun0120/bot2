#include "misc/bot_log.h"
#include "opengl/bot_opengl.h"
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_particle_shader_program.h"
#include "opengl/bot_text_system.h"
#include "gameutil/bot_game_lib.h"
#include "screen/bot_screen_manager.h"
#include "app/bot_app_config.h"
#include "app/bot_app.h"

namespace bot {

std::shared_ptr<App> App::k_app;

bool App::initInstance(Screen::Type startScreenType)
{
    App* app = new App();
    k_app.reset(app);

    if (!app->init(startScreenType))
    {
        k_app.reset();
        return false;
    }

    return true;
}

App::App()
    : m_window(nullptr)
{
    m_viewportSize[0] = 0.0f;
    m_viewportSize[1] = 0.0f;
}

App::~App()
{
    if (!m_window)
    {
        glfwTerminate();
    }
}

bool App::init(Screen::Type startScreenType)
{
    if (!initWindow())
    {
        LOG_ERROR("Failed to initialize window");
        return false;
    }

    if (!initOpenGL())
    {
        LOG_ERROR("Failed to initialize OpenGL");
        return false;
    }

    if (!initInputManager())
    {
        LOG_ERROR("Failed to intialize input manager");
        return false;
    }

    if (!initGame(startScreenType))
    {
        LOG_ERROR("Failed to initialize game");
        return false;
    }

    return true;
}

bool App::run()
{
    float delta;
    int ret;
    ScreenManager& screenMgr = ScreenManager::getInstance();
    InputProcessor processor = std::bind(&ScreenManager::processInput,
                                         &screenMgr,
                                         std::placeholders::_1);

    m_timeDeltaSmoother.start();
    m_inputMgr.start();

    while(glfwWindowShouldClose(m_window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if (!m_inputMgr.processInput(processor))
        {
            break;
        }

        delta = m_timeDeltaSmoother.getTimeDelta();
        ret = screenMgr.update(delta);
        if (ret == 2)
        {
            // the app should exit
            break;
        }
        else if (ret == 1)
        {
            // switched to another screen, clear input
            m_inputMgr.clear();
        }

        screenMgr.present();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    LOG_INFO("Exiting App::run");

    return true;
}

bool App::initWindow()
{
    LOG_INFO("Initializing window");

    if (!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const AppConfig& cfg = AppConfig::getInstance();

    m_window = glfwCreateWindow(cfg.getWidth(),
                                cfg.getHeight(),
                                cfg.getTitle().c_str(),
                                NULL, NULL);
    if (!m_window)
    {
        LOG_ERROR("Failed to open GLFW window");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        LOG_ERROR("Failed to initialize GLEW");
        return false;
    }

    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

    LOG_INFO("Done initializing window");

    return true;
}

bool App::initInputManager()
{
    const AppConfig& cfg = AppConfig::getInstance();

    m_inputMgr.init(m_window, cfg.getEventQueueSize(), m_viewportSize[1]);

    LOG_INFO("Done initializing input manager");

    return true;
}

bool App::initOpenGL()
{
    LOG_INFO("Initializing OpenGL");

    const AppConfig& cfg = AppConfig::getInstance();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool success = SimpleShaderProgram::initInstance(
                                cfg.getSimpleVertexShaderFile(),
                                cfg.getSimpleFragShaderFile());
    if (!success)
    {
        LOG_ERROR("Failed to initialize simple-shader");
        return false;
    }

    success = ParticleShaderProgram::initInstance(
                               cfg.getParticleVertexShaderFile(),
                               cfg.getParticleFragShaderFile());
    if (!success)
    {
        LOG_ERROR("Failed to initialie particle-shader");
        return false;
    }

    success = TextSystem::initInstance(cfg.getFontDir());
    if (!success)
    {
        LOG_ERROR("Failed to initialize text-system");
        return false;
    }

    updateViewport();

    LOG_INFO("Done initializing OpenGL");

    return true;
}

void App::updateViewport()
{
    int width, height;

    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    m_viewportSize[0] = static_cast<float>(width);
    m_viewportSize[1] = static_cast<float>(height);

    LOG_INFO("viewportWidth=%f viewportHeight=%f",
             m_viewportSize[0], m_viewportSize[1]);
}

bool App::initGame(Screen::Type startScreenType)
{
    LOG_INFO("Initializing game");

    if (!initTimeDeltaSmoother())
    {
        LOG_ERROR("Failed to initialize time-delta smoother");
        return false;
    }

    if (!initGameLib())
    {
        LOG_ERROR("Failed to initialize game template lib");
        return false;
    }

    ScreenManager::initInstance(startScreenType);

    LOG_INFO("Done initializing game");

    return true;
}

bool App::initTimeDeltaSmoother()
{
    LOG_INFO("Initializing time-delta smoother");

    const AppConfig& cfg = AppConfig::getInstance();

    m_timeDeltaSmoother.init(cfg.getTimeDeltaHistoryLen());

    LOG_INFO("Done initializing time-delta smoother");

    return true;
}

bool App::initGameLib()
{
    LOG_INFO("Initializing game-lib");

    bool success = GameLib::initInstance();

    if (!success)
    {
        LOG_ERROR("Failed to initialize game-lib");
        return false;
    }

    LOG_INFO("Done initializing game-lib");

    return true;
}

} // end of namespace bot
