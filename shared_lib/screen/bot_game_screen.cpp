#include <functional>
#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "misc/bot_math_utils.h"
#include "input/bot_input_event.h"
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_particle_shader_program.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_game_map_loader.h"
#include "gameutil/bot_game_lib.h"
#include "screen/bot_game_screen.h"
#include "screen/bot_game_screen_config.h"
#include "screen/bot_screen_manager.h"
#include "app/bot_app_config.h"
#include "app/bot_app.h"

using namespace rapidjson;

namespace bot {

GameScreen::GameScreen()
    : m_window(nullptr)
    , m_state(GAME_STATE_INIT)
{
}

GameScreen::~GameScreen()
{
}

bool GameScreen::init()
{
    LOG_INFO("Initializing GameScreen");

    App &app = App::getInstance();
    const AppConfig &cfg = AppConfig::getInstance();
    float viewportWidth = app.getViewportWidth();
    float viewportHeight = app.getViewportHeight();

    m_window = app.getWindow();

    if (!m_dashboard.init(this))
    {
        return false;
    }

    m_gameObjManager.init(&m_dashboard);

    if (!initMessageBox())
    {
        return false;
    }

    if (!loadMap(cfg.getMapFile()))
    {
        LOG_ERROR("Failed to load map from %s", cfg.getMapFile().c_str());
        return false;
    }

    LOG_INFO("Done loading dashboard");

    m_dashboardOrigin[0] = viewportWidth / 2.0f;
    m_dashboardOrigin[1] = viewportHeight / 2.0f;

    m_state = GAME_STATE_RUNNING;

    return true;
}

bool GameScreen::initMessageBox()
{
    const GameScreenConfig &cfg = GameLib::getInstance().getGameScreenConfig();
    const App &app = App::getInstance();
    float x = (app.getViewportWidth() - cfg.getWidth()) / 2.0f;
    float y = (app.getViewportHeight() - cfg.getHeight()) / 2.0f;

    bool ret = m_msgBox.init(x, y, cfg.getWidth(), cfg.getHeight(),
                             cfg.getTextWidth(), cfg.getTextHeight(),
                             cfg.getButtonWidth(), cfg.getButtonHeight(),
                             cfg.getButtonSpacing(), cfg.getTextButtonSpacing(),
                             cfg.getButtonTexts());

    if (!ret)
    {
        LOG_ERROR("Failed to initialize message-box");
        return false;
    }

    m_msgBox.setAction(BUTTON_EXIT, std::bind(&GameScreen::exit, this));
    m_msgBox.setAction(BUTTON_RESUME, std::bind(&GameScreen::resume, this));

    m_msgBox.setVisible(false);

    return true;
}

bool GameScreen::loadMap(const std::string &fileName)
{
    LOG_INFO("Loading map from %s", fileName.c_str());

    const App &app = App::getInstance();
    const AppConfig &cfg = AppConfig::getInstance();

    GameMapLoader mapLoader(m_gameObjManager, cfg.getMapPoolFactor());
    bool success = mapLoader.load(m_map, fileName, cfg.getLevel(),
                                  app.getViewportWidth(),
                                  app.getViewportHeight());
    if (!success)
    {
        return false;
    }

    LOG_INFO("Done loading map");

    return true;
}

int GameScreen::update(float delta)
{
    bool process = !m_msgBox.isVisible() &&
                   (m_state == GAME_STATE_RUNNING ||
                    m_state == GAME_STATE_POST_WIN);
    if (!process)
    {
        return 0;
    }

    Player *player = m_map.getPlayer();
    if (!player)
    {
        return 0;
    }

    player->update(delta, *this);
    player->setFlag(GAME_OBJ_FLAG_UPDATED);

    m_map.updateViewport();
    updateMissiles(delta);
    updateRobots(delta);
    updateEffects(delta);
    updateDissolveObjects(delta);

    clearDeadObjects();

    if (player->testFlag(GAME_OBJ_FLAG_DEAD))
    {
        m_state = GAME_STATE_END;
        m_msgBox.setMsg("Game Over");
        m_msgBox.setButtonVisible(BUTTON_RESUME, false);
        m_msgBox.setVisible(true);
    }
    else
    {
        bool showVictory = !player->testFlag(GAME_OBJ_FLAG_DISSOLVE) &&
                           m_state == GAME_STATE_RUNNING &&
                           m_map.getAIRobotCount() <= 0;
        if (showVictory)
        {
            m_state = GAME_STATE_POST_WIN;
            m_msgBox.setMsg("You are victorious");
            m_msgBox.setButtonVisible(BUTTON_RESUME, true);
            m_msgBox.setVisible(true);
        }
    }

    return 0;
}

void GameScreen::present()
{
    SimpleShaderProgram &simpleShader = SimpleShaderProgram::getInstance();
    const App &app = App::getInstance();

    simpleShader.use();
    simpleShader.setViewportSize(app.getViewportSize());
    simpleShader.setViewportOrigin(m_map.getViewportPos());

    static const GameObjectType LAYER_ORDER[] = {
        GAME_OBJ_TYPE_TILE,
        GAME_OBJ_TYPE_GOODIE,
        GAME_OBJ_TYPE_MISSILE,
        GAME_OBJ_TYPE_ROBOT
    };
    static const int NUM_LAYERS = sizeof(LAYER_ORDER) / sizeof(GameObjectType);
    static const int DONT_DRAW_FLAGS = GAME_OBJ_FLAG_DRAWN | GAME_OBJ_FLAG_DEAD;

    int startRow, endRow, startCol, endCol;

    m_map.getViewportRegion(startRow, endRow, startCol, endCol);
    m_map.clearFlagsInRect(startRow, endRow,
                           startCol, endCol, GAME_OBJ_FLAG_DRAWN);

    for (int i = 0; i < NUM_LAYERS; ++i)
    {
        for (int r = startRow; r <= endRow; ++r)
        {
            for (int c = startCol; c <= endCol; ++c)
            {
                LinkedList<GameObjectItem> &cell = m_map.getMapCell(r, c);
                GameObjectItem *item, * next;
                for (item = cell.getFirst(); item; item = next)
                {
                    next = static_cast<GameObjectItem*>(item->getNext());

                    GameObject *obj = item->getObj();
                    bool dontProcess = obj->getType() != LAYER_ORDER[i] ||
                                       obj->testFlag(DONT_DRAW_FLAGS);
                    if (dontProcess)
                    {
                        continue;
                    }

                    obj->present();
                    obj->setFlag(GAME_OBJ_FLAG_DRAWN);
                }
            }
        }
    }

    m_map.present();
    presentEffects();
    presentOverlay();
}

int GameScreen::processInput(const InputEvent &e)
{
    if (m_msgBox.isVisible())
    {
        return m_msgBox.processInput(e);
    }

    const int DONT_PROCESS_FLAG = GAME_OBJ_FLAG_DEAD | GAME_OBJ_FLAG_DISSOLVE;
    Player *player = m_map.getPlayer();

    if (!player || player->testFlag(DONT_PROCESS_FLAG)) {
        return 0;
    }

    switch (e.m_type)
    {
        case InputEvent::ET_MOUSE_MOVE:
            return handleMouseMove(e.m_mouseMoveEvent);
        case InputEvent::ET_MOUSE_BUTTON:
            return handleMouseButton(e.m_mouseButtonEvent);
        case InputEvent::ET_KEY:
            return handleKey(e.m_keyEvent);
        default:
            LOG_WARN("Unknown input type %d", static_cast<int>(e.m_type));
    }
    return 0;
}

bool GameScreen::updateRobots(float delta)
{
    const int DONT_UPDATE_FLAG = GAME_OBJ_FLAG_UPDATED |
                                 GAME_OBJ_FLAG_DEAD |
                                 GAME_OBJ_FLAG_DISSOLVE;
    int startRow, endRow, startCol, endCol;

    m_map.getViewportRegion(startRow, endRow, startCol, endCol);
    m_map.clearFlagsInRect(startRow, endRow, startCol, endCol,
                           GAME_OBJ_FLAG_UPDATED);

    for (int r = startRow; r <= endRow; ++r)
    {
        for (int c = startCol; c <= endCol; ++c)
        {
            LinkedList<GameObjectItem> &cell = m_map.getMapCell(r, c);
            GameObjectItem *next = nullptr;
            for (GameObjectItem *item = cell.getFirst(); item; item = next)
            {
                next = static_cast<GameObjectItem*>(item->getNext());
                GameObject *obj = item->getObj();

                bool dontUpdate =
                         obj->getType() != GAME_OBJ_TYPE_ROBOT ||
                         obj->testFlag(DONT_UPDATE_FLAG) ||
                         obj == static_cast<GameObject*>(m_map.getPlayer());
                if (dontUpdate)
                {
                    continue;
                }

                Robot *robot = static_cast<Robot*>(obj);
                robot->update(delta, *this);
                robot->setFlag(GAME_OBJ_FLAG_UPDATED);
            }
        }
    }

    return true;
}

bool GameScreen::updateMissiles(float delta)
{
    Missile *next = nullptr;
    Missile *missile = m_gameObjManager.getFirstActiveMissile();
    for (; missile; missile = next)
    {
        next = static_cast<Missile*>(missile->getNext());
        missile->update(delta, *this);
    }

    return true;
}

void GameScreen::updateEffects(float delta)
{
    ParticleEffect *next = nullptr;
    ParticleEffect *effect = m_gameObjManager.getFirstParticleEffect();
    for (; effect; effect = next)
    {
        next = static_cast<ParticleEffect*>(effect->getNext());
        effect->update(delta, *this);
    }
}

void GameScreen::updateDissolveObjects(float delta)
{
    GameObject *next = nullptr;
    GameObject *obj = m_gameObjManager.getFirstDissolveObject();

    for(; obj; obj = next)
    {
        next = static_cast<GameObject*>(obj->getNext());
        obj->update(delta, *this);
    }
}

void GameScreen::presentEffects()
{
    ParticleShaderProgram &program = ParticleShaderProgram::getInstance();
    const App &app = App::getInstance();

    program.use();
    program.setViewportSize(app.getViewportSize());
    program.setViewportOrigin(m_map.getViewportPos());

    ParticleEffect *effect = m_gameObjManager.getFirstParticleEffect();
    ParticleEffect *next;
    for (; effect; effect = next)
    {
        next = static_cast<ParticleEffect*>(effect->getNext());
        effect->present();
    }
}

void GameScreen::presentOverlay()
{
    SimpleShaderProgram &program = SimpleShaderProgram::getInstance();
    const App &app = App::getInstance();

    program.use();
    program.setViewportSize(app.getViewportSize());
    program.setViewportOrigin(m_dashboardOrigin);

    m_dashboard.draw();
    m_msgBox.present();
}

int GameScreen::handleMouseMove(const MouseMoveEvent &e)
{
    Player *player = m_map.getPlayer();

    int btnState = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);
    if (btnState == GLFW_PRESS)
    {
        float destX = m_map.getWorldX(e.m_x);
        float destY = m_map.getWorldY(e.m_y);

        player->setDest(destX, destY);
        player->setMovingEnabled(true);
    }
    else
    {
        if (!player->isMoving())
        {
            float destX = m_map.getWorldX(e.m_x);
            float destY = m_map.getWorldY(e.m_y);
            float directionX, directionY;

            calculateDirection(directionX, directionY,
                               player->getPosX(), player->getPosY(),
                               destX, destY);
            player->setDirection(directionX, directionY);
        }
    }

    return 0;
}

int GameScreen::handleMouseButton(const MouseButtonEvent &e)
{
    Player *player = m_map.getPlayer();
    TimePoint now = Clock::now();

    if (e.m_button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (e.m_action == GLFW_PRESS)
        {
            float destX = m_map.getWorldX(e.m_x);
            float destY = m_map.getWorldY(e.m_y);

            player->setDest(destX, destY);
            player->setMovingEnabled(true);
        }
        else if(e.m_action == GLFW_RELEASE)
        {
            player->unsetDest();
            player->setMovingEnabled(false);
        }
    }
    else if (e.m_button == GLFW_MOUSE_BUTTON_LEFT)
    {
        player->applySkill(*this, now, 0);
    }

    return 0;
}

int GameScreen::handleKey(const KeyEvent &e)
{
    Player *player = m_map.getPlayer();
    TimePoint now = Clock::now();
    switch (e.m_key)
    {
        case GLFW_KEY_A:
        {
            player->applySkill(*this, now, 0);
            break;
        }
        case GLFW_KEY_S:
        {
            player->applySkill(*this, now, 1);
            break;
        }
        case GLFW_KEY_D:
        {
            player->applySkill(*this, now, 2);
            break;
        }
        case GLFW_KEY_ESCAPE:
        {
            bool process = m_state == GAME_STATE_RUNNING ||
                           m_state == GAME_STATE_POST_WIN;
            if (process)
            {
                m_msgBox.setMsg("What's next?");
                m_msgBox.setButtonVisible(BUTTON_RESUME, true);
                m_msgBox.setVisible(true);
            }
            break;
        }
    }

    return 0;
}

void GameScreen::clearDeadObjects()
{
    GameObject *obj = m_gameObjManager.getFirstDeadObject();
    for (; obj; obj = static_cast<GameObject*>(obj->getNext()))
    {
        m_map.removeObject(obj);
    }

    m_gameObjManager.clearDeadObjects();
}

int GameScreen::switchToStart()
{
    ScreenManager &screenMgr = ScreenManager::getInstance();
    screenMgr.switchScreen(Screen::SCREEN_START);
    return 1;
}

int GameScreen::exit()
{
    return switchToStart();
}

int GameScreen::resume()
{
    m_msgBox.setVisible(false);
    return 1;
}

} // end of namespace bot
