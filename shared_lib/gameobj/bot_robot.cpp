#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "structure/bot_linked_list.h"
#include "gameutil/bot_game_object_item.h"
#include "gameutil/bot_collide.h"
#include "gameutil/bot_game_lib.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_goodie.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_player.h"
#include "skill/bot_skill.h"
#include "screen/bot_game_screen.h"

namespace bot {

Robot::Robot()
    : m_side(SIDE_UNKNOWN)
    , m_mask()
    , m_hasDest(false)
    , m_destX(0.0f)
    , m_destY(0.0f)
{
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
    m_mask.init(255, 255, 255, 255);
}

Robot::~Robot()
{
    unsigned int count = numSkills();
    for (unsigned int i = 0; i < count; ++i)
    {
        delete m_skills[i];
    }
}

bool Robot::init(const RobotTemplate* t, Side side,
                 int hpLevel, int hpRestoreLevel,
                 int armorLevel, int armorRepairLevel,
                 int powerLevel, int powerRestoreLevel,
                 int weaponLevel, int moverLevel,
                 const std::vector<int>& skillLevels,
                 float x, float y,
                 float directionX, float directionY)
{
    bool ret = initBasic(t, side, hpLevel, hpRestoreLevel,
                         armorLevel, armorRepairLevel,
                         powerLevel, powerRestoreLevel,
                         weaponLevel, moverLevel,
                         x, y, directionX, directionY);
    if (!ret)
    {
        return false;
    }

    if (!initSkills(skillLevels))
    {
        return false;
    }

    return true;
}

bool Robot::init(const RobotTemplate* t, Side side,
                 int hpLevel, int hpRestoreLevel,
                 int armorLevel, int armorRepairLevel,
                 int powerLevel, int powerRestoreLevel,
                 int weaponLevel, int moverLevel, int skillLevel,
                 float x, float y, float directionX, float directionY)
{
    bool ret = initBasic(t, side, hpLevel, hpRestoreLevel,
                         armorLevel, armorRepairLevel,
                         powerLevel, powerRestoreLevel,
                         weaponLevel, moverLevel,
                         x, y, directionX, directionY);
    if (!ret)
    {
        return false;
    }

    if (!initSkills(skillLevel))
    {
        return false;
    }

    return true;
}

bool Robot::initBasic(const RobotTemplate* t, Side side,
                      int hpLevel, int hpRestoreLevel,
                      int armorLevel, int armorRepairLevel,
                      int powerLevel, int powerRestoreLevel,
                      int weaponLevel, int moverLevel,
                      float x, float y, float directionX, float directionY)
{
    bool ret = GameObject::init(t, x, y);
    if (!ret)
    {
        return false;
    }

    m_direction[0] = directionX;
    m_direction[1] = directionY;

    ret = m_base.init(t->getBaseTemplate(), this, hpLevel, hpRestoreLevel,
                      armorLevel, armorRepairLevel, powerLevel,
                      powerRestoreLevel);
    if (!ret)
    {
        return false;
    }

    ret = m_weapon.init(t->getWeaponTemplate(), this, weaponLevel);
    if (!ret)
    {
        return false;
    }

    ret = m_mover.init(t->getMoverTemplate(), this, moverLevel);
    if (!ret)
    {
        return false;
    }

    if (side != SIDE_AI && side != SIDE_PLAYER)
    {
        LOG_ERROR("Invalid side %d", static_cast<int>(side));
        return false;
    }

    m_side = side;

    return true;
}

bool Robot::initSkills(const std::vector<int>& skillLevels)
{
    const RobotTemplate* t = getTemplate();
    unsigned int numSkills = t->numSkills();

    if (numSkills == 0)
    {
        LOG_ERROR("skills is empty");
        return false;
    }

    if (numSkills != skillLevels.size())
    {
        LOG_ERROR("skillLevels is invalid");
        return false;
    }

    m_skills.resize(numSkills);
    for (unsigned int i = 0; i < numSkills; ++i)
    {
        m_skills[i] = Skill::create(t->getSkillTemplate(i), this,
                                    skillLevels[i]);

        if (!m_skills[i])
        {
            LOG_ERROR("Failed to create Skill: idx=%d", i);
            return false;
        }
    }

    return true;
}

bool Robot::initSkills(int skillLevel)
{
    const RobotTemplate* t = getTemplate();
    unsigned int numSkills = t->numSkills();

    if (numSkills == 0)
    {
        LOG_ERROR("skills is empty");
        return false;
    }

    if (skillLevel < 0)
    {
        LOG_ERROR("Invalid skillLevel %d", skillLevel);
        return false;
    }

    m_skills.resize(numSkills);
    for (unsigned int i = 0; i < numSkills; ++i)
    {
        m_skills[i] = Skill::create(t->getSkillTemplate(i), this,
                                    skillLevel);

        if (!m_skills[i])
        {
            LOG_ERROR("Failed to create Skill: idx=%d", i);
            return false;
        }
    }

    return true;
   
}

void Robot::present()
{
    m_weapon.present();
    m_base.present();
    m_mover.present();
}

void Robot::shiftPos(float deltaX, float deltaY)
{
    m_pos[0] += deltaX;
    m_pos[1] += deltaY;
    m_base.shiftWeaponMoverPos(deltaX, deltaY);
    m_weapon.shiftFirePoints(deltaX, deltaY);
}

void Robot::setPos(float x, float y)
{
    shiftPos(x - m_pos[0], y - m_pos[1]);
}

void Robot::setDirection(float directionX, float directionY)
{
    m_direction[0] = directionX;
    m_direction[1] = directionY;
    m_base.resetWeaponMoverPos();
    m_weapon.resetFirePoints();
}

bool Robot::addHP(float delta)
{
    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return false;
    }

    if (testFlag(GAME_OBJ_FLAG_INDESTRUCTABLE) && delta < 0.0f)
    {
        return true;
    }

    m_base.setHP(m_base.getHP() + delta);

    return m_base.getHP() > 0.0f;
}

void Robot::refillHP()
{
    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return;
    }

    m_base.refillHP();
}

void Robot::processCollisions(LinkedList<GameObjectItem>& collideObjs,
                              GameScreen& screen)
{
    GameObjectItem* item;
    const int DEAD_FLAG = GAME_OBJ_FLAG_DEAD | GAME_OBJ_FLAG_DISSOLVE;

    for (item = collideObjs.getFirst(); item; item = item->getNext())
    {
        GameObject* o = item->getObj();

        if (o->testFlag(DEAD_FLAG))
        {
            continue;
        }

        switch (o->getType())
        {
            case GAME_OBJ_TYPE_MISSILE:
            {
                Missile* missile = static_cast<Missile*>(o);
                missile->onHit(screen, *this);
                break;
            }
            case GAME_OBJ_TYPE_GOODIE:
            {
                if (m_side == SIDE_PLAYER)
                {
                    Player* player = static_cast<Player*>(this);
                    Goodie* goodie = static_cast<Goodie*>(o);
                    player->consumeGoodie(goodie, screen);
                }
                break;
            }
            default:
            {
                LOG_WARN("Unexcepted game object type %d for collision",
                         static_cast<int>(item->getObj()->getType()));
            }
        }
    }
}

bool Robot::updateMover(float delta, GameScreen& gameScreen)
{
    return m_mover.update(gameScreen, delta);
}

void Robot::updateBase()
{
    m_base.update();
}

bool Robot::updateMask()
{
    const GameConfig& cfg = GameLib::getInstance().getGameConfig();
    float alpha = 1.0f - elapsedTimeMs(m_deathTime) / cfg.getDissolveTimeMS();
    bool maskVisible = alpha > 0.0f;

    if (maskVisible)
    {
        m_mask.setAlpha(alpha);
    }
    else
    {
        m_mask.setAlpha(0.0f);
    }

    return maskVisible;
}

void Robot::setDest(float destX, float destY)
{
    m_hasDest = true;
    m_destX = destX;
    m_destY = destY;

    float directionX, directionY;

    calculateDirection(directionX, directionY, m_pos[0], m_pos[1],
                       m_destX, m_destY);
    setDirection(directionX, directionY);
}

void Robot::unsetDest()
{
    m_hasDest = false;
    m_destX = 0.0f;
    m_destY = 0.0f;
}

bool Robot::applySkill(GameScreen& screen, const TimePoint& t, unsigned int idx)
{
    if (idx >= numSkills())
    {
        LOG_WARN("Skill index %d out of range", idx);
        return false;
    }

    Skill* skill = getSkill(idx);
    if (!skill->available(t))
    {
        return false;
    }

    skill->apply(screen, t);

    return true;
}

bool Robot::scaleSkillCooldown(float cooldownMultiplier)
{
    unsigned int count = numSkills();
    for (unsigned int i = 0; i < count; ++i)
    {
        if (!m_skills[i]->setCooldownMultiplier(cooldownMultiplier))
        {
            LOG_ERROR("setCooldownMultiplier failed for skill idx=%d", i);
            return false;
        }
    }

    return true;
}

} // end of namespace bot

