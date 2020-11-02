#include <functional>
#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "misc/bot_math_utils.h"
#include "input/bot_input_event.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_game_map_loader.h"
#include "gameutil/bot_game_lib.h"
#include "screen/bot_game_screen.h"
#include "screen/bot_screen_manager.h"
#include "app/bot_app_config.h"

using namespace rapidjson;

namespace bot {

GameScreen::GameScreen()
    : m_state(GAME_STATE_INIT)
    , m_visibleMsgBoxIdx(-1)
{
}

GameScreen::~GameScreen()
{
}

bool GameScreen::init()
{
    LOG_INFO("Initializing GameScreen");

    const GameLib& lib = GameLib::getInstance();
    const App& app = App::getInstance();
    const AppConfig& cfg = AppConfig::getInstance();
    float viewportWidth = app.getViewportWidth();
    float viewportHeight = app.getViewportHeight();

    m_gameObjManager.init(&m_map, cfg, &m_lib);

    initMessageBoxes(lib.getMessageBoxConfig(), lib.getButtonConfig());

    if (!loadMap(cfg.getMapFile(), cfg))
    {
        LOG_ERROR("Failed to load map from %s", cfg.getMapFile().c_str());
        return false;
    }

    m_dashboard.init(m_map.getPlayer(), &lib.getDashboardConfig());

    LOG_INFO("Done loading dashboard");

    m_dashboardOrigin[0] = viewportWidth / 2.0f;
    m_dashboardOrigin[1] = viewportHeight / 2.0f;

    m_state = GAME_STATE_RUNNING;

    return true;
}

void GameScreen::initMessageBoxes(const MessageBoxConfig& msgBoxCfg,
                                  const ButtonConfig& buttonCfg)
{
/*    m_msgBoxes.resize(MSGBOX_COUNT);

    Button::ActionFunc exitFunc = std::bind(GameScreen::exitGame, this);
    Button::ActionFunc resumeFunc = std::bind(GameScreen::resumeGame, this);
    Button::ActionFunc restartFunc = std::bind(GameScreen::restartGame, this);

    MessageBox& escMsgBox = m_msgBoxes[MSGBOX_ESCAPE_GAME];
    std::vector<std::string> buttons1 = {
        "Exit", "Resume", "Restart"
    };

    escMsgBox.init(msgBoxCfg, buttonCfg, textSys,
                   "Exit game?", buttons1);
    escMsgBox.setAction(0, exitFunc);
    escMsgBox.setAction(1, resumeFunc);
    escMsgBox.setAction(2, restartFunc);

    MessageBox& victoryMsgBox = m_msgBoxes[MSGBOX_VICTORY];

    victoryMsgBox.init(msgBoxCfg, buttonCfg, textSys,
                       "You are victorious", buttons1);
    victoryMsgBox.setAction(0, exitFunc);
    victoryMsgBox.setAction(1, resumeFunc);
    victoryMsgBox.setAction(2, restartFunc);

    MessageBox& defeatMsgBox = m_msgBoxes[MSGBOX_DEFEAT];
    std::vector<std::string> buttons2 = {
        "Exit", "Restart"
    };

    defeatMsgBox.init(msgBoxCfg, buttonCfg, textSys,
                      "You are defeated", buttons2);
    defeatMsgBox.setAction(0, exitFunc);
    defeatMsgBox.setAction(1, restartFunc);
*/
}

bool GameScreen::loadMap(const std::string& fileName)
{
    LOG_INFO("Loading map from %s", fileName.c_str());

    const App& app = App::getInstance();
    const AppConfig& cfg = AppConfig::getInstance();

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
    if (m_state != GAME_STATE_RUNNING)
    {
        return 0;
    }

    Player* player = m_map.getPlayer();
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

    bool end = player->testFlag(GAME_OBJ_FLAG_DEAD) ||
               m_gameObjManager.getAIRobotCount() <= 0;

    if (end)
    {
        std::string msg = player->testFlag(GAME_OBJ_FLAG_DEAD) ?
                                        "Game Over" : "You are victorious";

        m_state = GAME_STATE_END;
        m_map.setPlayer(nullptr);
/*        m_msgBoxVisible = true;
        m_msgBox.init(&m_lib->getMessageBoxConfig(), &m_lib->getButtonConfig(),
                      m_graphics->getTextSystem(), MessageBox::OPTION_OK, msg);
        m_msgBox.setOKAction(std::bind(&GameScreen::switchToStart, this));
*/
    }

    clearDeadObjects();

    return 0;
}

void GameScreen::present()
{
    SimpleShaderProgram& simpleShader = SimpleShaderProgram::getInstance();
    const App& app = App::getInstance();

    simpleShader.use();
    simpleShader.setViewportSize(app.getViewportSize());
    simpleShaderProgram.setViewportOrigin(m_map.getViewportPos());

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
                LinkedList<GameObjectItem>& cell = m_map.getMapCell(r, c);
                GameObjectItem* item, * next;
                for (item = cell.getFirst(); item; item = next)
                {
                    next = static_cast<GameObjectItem*>(item->getNext());

                    GameObject* obj = item->getObj();
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

int GameScreen::processInput(const InputEvent& e)
{
    if (m_msgBoxVisible)
    {
        return m_msgBox.processInput(e);
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
    const int DONT_UPDATE_FLAG = GAME_OBJ_FLAG_UPDATED | GAME_OBJ_FLAG_DEAD;
    int startRow, endRow, startCol, endCol;

    m_map.getViewportRegion(startRow, endRow, startCol, endCol);
    m_map.clearFlagsInRect(startRow, endRow, startCol, endCol,
                           GAME_OBJ_FLAG_UPDATED);

    for (int r = startRow; r <= endRow; ++r)
    {
        for (int c = startCol; c <= endCol; ++c)
        {
            LinkedList<GameObjectItem>& cell = m_map.getMapCell(r, c);
            GameObjectItem* next = nullptr;
            for (GameObjectItem* item = cell.getFirst(); item; item = next)
            {
                next = static_cast<GameObjectItem*>(item->getNext());
                GameObject* obj = item->getObj();

                bool dontUpdate =
                         obj->getType() != GAME_OBJ_TYPE_ROBOT ||
                         obj->testFlag(DONT_UPDATE_FLAG) ||
                         obj == static_cast<GameObject*>(m_map.getPlayer());
                if (dontUpdate)
                {
                    continue;
                }

                Robot* robot = static_cast<Robot*>(obj);
                robot->update(delta, *this);
                robot->setFlag(GAME_OBJ_FLAG_UPDATED);
            }
        }
    }

    return true;
}

bool GameScreen::updateMissiles(float delta)
{
    Missile* next = nullptr;
    Missile* missile = m_gameObjManager.getFirstActiveMissile();
    for (; missile; missile = next)
    {
        next = static_cast<Missile*>(missile->getNext());
        missile->update(delta, *this);
    }

    return true;
}

void GameScreen::updateEffects(float delta)
{
    ParticleEffect* next = nullptr;
    ParticleEffect* effect = m_gameObjManager.getFirstParticleEffect();
    for (; effect; effect = next)
    {
        next = static_cast<ParticleEffect*>(effect->getNext());
        effect->update(delta, *this);
    }
}

void GameScreen::presentEffects()
{
    ParticleShaderProgram& program = ParticleShaderProgram::getInstance();
    const App& app = App::getInstance();

    program.use();
    program.setViewportSize(app.getViewportSize());
    program.setViewportOrigin(m_map.getViewportPos());

    ParticleEffect* effect = m_gameObjManager.getFirstParticleEffect();
    ParticleEffect* next;
    for (; effect; effect = next)
    {
        next = static_cast<ParticleEffect*>(effect->getNext());
        effect->present();
    }
}

void GameScreen::presentOverlay()
{
    SimpleShaderProgram& program = SimpleShaderProgram::getInstance();
    const App& app = App::getInstance();

    program.use();
    program.setViewportSize(app.getViewportSize());
    program.setViewportOrigin(m_dashboardOrigin);

    m_dashboard.draw();
/*
    if (m_msgBoxVisible)
    {
        m_msgBox.show(*m_graphics);
    }*/
}

int GameScreen::handleMouseMove(const MouseMoveEvent& e)
{
    Player* player = m_map.getPlayer();
    if (!player || player->testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return 0;
    }

    float destX = m_map.getWorldX(e.m_x);
    float destY = m_map.getWorldY(e.m_y);
    float directionX, directionY;
    calculateDirection(directionX, directionY,
                       player->getPosX(), player->getPosY(),
                       destX, destY);
    player->setDirection(directionX, directionY);
    return 0;
}

int GameScreen::handleMouseButton(const MouseButtonEvent& e)
{
    Player* player = m_map.getPlayer();
    if (!player || player->testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return 0;
    }

    if (e.m_button == GLFW_MOUSE_BUTTON_LEFT)
    {
        player->setShootingEnabled(e.m_action == GLFW_PRESS);
    }

    return 0;
}

int GameScreen::handleKey(const KeyEvent& e)
{
    Player* player = m_map.getPlayer();
    if (!player || player->testFlag(GAME_OBJ_FLAG_DEAD)) {
        return 0;
    }

    switch (e.m_key)
    {
        /*case GLFW_KEY_F:
            return handleFireKey(e.m_action);*/
        case GLFW_KEY_W:
            player->setMovingEnabled(true);
            break;
        case GLFW_KEY_S:
            player->setMovingEnabled(false);
            break;
    }

    return 0;
}

void GameScreen::clearDeadObjects()
{
    GameObject* obj = m_gameObjManager.getFirstDeadObject();
    for (; obj; obj = static_cast<GameObject*>(obj->getNext()))
    {
        m_map.removeObject(obj);
    }

    m_gameObjManager.clearDeadObjects();
}

int GameScreen::switchToStart()
{
    m_screenManager->switchScreen(Screen::SCREEN_START);
    return 1;
}

} // end of namespace bot
