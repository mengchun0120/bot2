#include "misc/bot_log.h"
#include "opengl/bot_opengl.h"
#include "app/bot_app_config.h"
#include "app/bot_app.h"

namespace bot {

App::App()
    : m_window(nullptr)
    , m_config(nullptr)
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

bool App::init(const AppConfig* cfg, Screen::Type startScreenType)
{
    m_config = cfg;

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
    InputProcessor processor = std::bind(&ScreenManager::processInput,
                                         &m_screenMgr,
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
        ret = m_screenMgr.update(delta);
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

        m_screenMgr.present();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

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

    m_window = glfwCreateWindow(m_config->getWidth(),
                                m_config->getHeight(),
                                m_config->getTitle().c_str(),
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
    m_inputMgr.init(m_window, m_config->getEventQueueSize(), m_viewportSize[1]);

    LOG_INFO("Done initializing input manager");

    return true;
}

bool App::initOpenGL()
{
    LOG_INFO("Initializing OpenGL");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool ret = m_graphics.init(m_config->getSimpleVertexShaderFile(),
                               m_config->getSimpleFragShaderFile(),
                               m_config->getParticleVertexShaderFile(),
                               m_config->getParticleFragShaderFile(),
                               m_config->getFontDir());

    if (!ret)
    {
        LOG_ERROR("Failed to initialize graphics");
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

    m_screenMgr.init(m_config, &m_gameLib, &m_graphics, startScreenType,
                     m_viewportSize[0], m_viewportSize[1]);

    LOG_INFO("Done initializing game");

    return true;
}

bool App::initTimeDeltaSmoother()
{
    LOG_INFO("Initializing time-delta smoother");

    m_timeDeltaSmoother.init(m_config->getTimeDeltaHistoryLen());

    LOG_INFO("Done initializing time-delta smoother");

    return true;
}

bool App::initGameLib()
{
    LOG_INFO("Initializing game template libraries");

    bool success = m_gameLib.load(m_viewportSize[0], m_viewportSize[1],
                                  *m_config);

    if (!success)
    {
        return false;
    }

    LOG_INFO("Done initializing game template libraries");

    return true;
}

} // end of namespace bot
