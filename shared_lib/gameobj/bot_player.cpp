#include <cstdio>
#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "screen/bot_game_screen.h"
#include "gametemplate/bot_player_template.h"
#include "gameutil/bot_game_lib.h"
#include "gameobj/bot_goodie.h"
#include "gameobj/bot_dashboard.h"
#include "gameobj/bot_player.h"
#include "app/bot_app.h"

namespace bot {

Player::Player()
    : m_firstActiveEffect(nullptr)
    , m_firstFreeEffect(nullptr)
    , m_activeEffectCount(0)
    , m_experience(0)
    , m_experienceMultiplier(1.0f)
    , m_gold(0)
    , m_dashboard(nullptr)
{
}

Player::~Player()
{
}

bool Player::init(const PlayerTemplate* playerTemplate, float x, float y,
                  float directionX, float directionY, Dashboard* dashboard)
{
    bool ret = Robot::init(playerTemplate, SIDE_PLAYER,
                           playerTemplate->getHPLevel(),
                           playerTemplate->getHPRestoreLevel(),
                           playerTemplate->getArmorLevel(),
                           playerTemplate->getArmorRepairLevel(),
                           playerTemplate->getPowerLevel(),
                           playerTemplate->getPowerRestoreLevel(),
                           playerTemplate->getWeaponLevel(),
                           playerTemplate->getMissileLevel(),
                           playerTemplate->getMoverLevel(),
                           x, y, directionX, directionY);

    if (!ret)
    {
        return false;
    }

    initEffects();

    m_experience = 0;
    m_experienceMultiplier = 1.0f;

    m_gold = 0;
    resetGoldStr();

    m_dashboard = dashboard;

    return true;
}

void Player::present()
{
    Robot::present();
}

void Player::update(float delta, GameScreen& screen)
{
    if (!testFlag(GAME_OBJ_FLAG_DISSOLVE))
    {
        updateEffects();
        updateMover(delta, screen);

        if (testFlag(GAME_OBJ_FLAG_DEAD))
        {
            return;
        }

        updateWeapon(screen);
        updateBase();

        if (m_dashboard)
        {
            m_dashboard->setProgressBarRatio(Dashboard::BAR_ARMOR,
                                             m_base.getArmorRatio());
            m_dashboard->setProgressBarRatio(Dashboard::BAR_POWER,
                                             m_base.getPowerRatio());
        }
    }
    else
    {
        if (!updateMask())
        {
            setFlag(GAME_OBJ_FLAG_DEAD);
        }
    }
}

void Player::initEffects()
{
    m_effects.resize(GOODIE_EFFECT_COUNT);
    for (int i = 0; i < GOODIE_EFFECT_COUNT - 1; ++i)
    {
        m_effects[i].setNext(&m_effects[i + 1]);
    }
    m_firstActiveEffect = nullptr;
    m_firstFreeEffect = &m_effects[0];
    m_activeEffectCount = 0;
}

void Player::consumeGoodie(Goodie* goodie, GameScreen& screen)
{
    if (isInstantaneousGoodie(goodie->getGoodieType()))
    {
        applyInstantaneousEffect(goodie);
    }
    else
    {
        applyNonInstantaneousEffect(goodie);
    }

    screen.getGameObjManager().sendToDeathQueue(goodie);
}

void Player::applyInstantaneousEffect(Goodie* goodie)
{
    switch (goodie->getGoodieType())
    {
    case GOODIE_GOLD:
        m_gold++;
        resetGoldStr();
        break;
    case GOODIE_HEALTH:
        refillHP();
        break;
    default:
        LOG_ERROR("Goodie type %d is NOT instantaneous!",
                  static_cast<int>(goodie->getGoodieType()));
        break;
    }
}

void Player::applyNonInstantaneousEffect(Goodie* goodie)
{
    if (!addEffect(goodie))
    {
        return;
    }

    switch (goodie->getGoodieType())
    {
        case GOODIE_INDESTRUCTABLE:
        {
            setFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
            break;
        }
        case GOODIE_DOUBLE_EXPERIENCE:
        {
            m_experienceMultiplier = 2.0f;
            break;
        }
        case GOODIE_QUICK_MOVER:
        {
            m_mover.setSpeedMultiplier(2.0f);
            break;
        }
        case GOODIE_QUICK_SHOOTER:
        {
            m_weapon.setFireDurationMultiplier(0.5f);
            break;
        }
        case GOODIE_DOUBLE_DAMAGE:
        {
            m_weapon.setDamageMultiplier(2.0f);
            break;
        }
        default:
        {
            LOG_ERROR("Invalid non-instantaneous goodie type %d",
                      static_cast<int>(goodie->getGoodieType()));
            break;
        }
    }
}

bool Player::addEffect(Goodie* goodie)
{
    for (GoodieEffect* t = m_firstActiveEffect; t; t = t->getNext())
    {
        if (t->getType() == goodie->getGoodieType())
        {
            t->start();
            return true;
        }
    }

    if (!m_firstFreeEffect)
    {
        LOG_WARN("Active effect list is full");
        return false;
    }

    GoodieEffect* next = m_firstFreeEffect->getNext();
    m_firstFreeEffect->setTemplate(goodie->getTemplate());
    m_firstFreeEffect->setNext(m_firstActiveEffect);
    m_firstActiveEffect = m_firstFreeEffect;
    m_firstFreeEffect = next;
    ++m_activeEffectCount;

    m_firstActiveEffect->start();
    resetEffectPos();

    return true;
}

void Player::expireEffect(GoodieEffect* effect)
{
    switch (effect->getType())
    {
        case GOODIE_INDESTRUCTABLE:
        {
            clearFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
            break;
        }
        case GOODIE_DOUBLE_EXPERIENCE:
        {
            m_experienceMultiplier = 1.0f;
            break;
        }
        case GOODIE_QUICK_MOVER:
        {
            m_mover.setSpeedMultiplier(1.0f);
            break;
        }
        case GOODIE_QUICK_SHOOTER:
        {
            m_weapon.setFireDurationMultiplier(1.0f);
            break;
        }
        case GOODIE_DOUBLE_DAMAGE:
        {
            m_weapon.setDamageMultiplier(1.0f);
            break;
        }
        default:
        {
            LOG_ERROR("Invalid non-instantaneous goodie type %d",
                      static_cast<int>(effect->getType()));
            break;
        }
    }
}

void Player::updateEffects()
{
    GoodieEffect* cur, * prev, * next;
    TimePoint now = Clock::now();

    prev = nullptr;
    for (cur = m_firstActiveEffect; cur; cur = next)
    {
        next = cur->getNext();

        if (cur->update(now))
        {
            expireEffect(cur);

            if (prev)
            {
                prev->setNext(next);
            }
            else
            {
                m_firstActiveEffect = next;
            }

            cur->setNext(m_firstFreeEffect);
            m_firstFreeEffect = cur;

            --m_activeEffectCount;

            resetEffectPos();
        }
        else
        {
            prev = cur;
        }
    }
}

void Player::resetGoldStr()
{
    snprintf(m_goldStr, sizeof(m_goldStr), "%d", m_gold);
}

void Player::resetEffectPos()
{
    if (m_activeEffectCount == 0)
    {
        return;
    }

    const DashboardConfig& cfg = GameLib::getInstance().getDashboardConfig();
    float viewportWidth = App::getInstance().getViewportWidth();
    float totalWidth = 0.0f;
    GoodieEffect* effect;

    for (effect = m_firstActiveEffect; effect; effect = effect->getNext())
    {
        totalWidth += effect->getWidth();
    }
    totalWidth += (m_activeEffectCount - 1) * cfg.getEffectSpacing();

    float x = (viewportWidth - totalWidth) / 2.0f;
    float y = cfg.getEffectMargin();

    for(effect = m_firstActiveEffect; effect; effect = effect->getNext())
    {
        effect->setPos(x, y);
        x += effect->getWidth() + cfg.getEffectSpacing();
    }
}

void Player::onDeath(GameScreen& screen)
{
    setFlag(GAME_OBJ_FLAG_DISSOLVE);
    m_deathTime = Clock::now();
}

} // end of namespace bot
