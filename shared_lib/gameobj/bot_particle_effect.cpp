#include "opengl/bot_color.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_particle_shader_program.h"
#include "gameobj/bot_particle_effect.h"
#include "screen/bot_game_screen.h"

namespace bot {

ParticleEffect::ParticleEffect()
    : m_duration(0.0f)
{
}

bool ParticleEffect::init(const ParticleEffectTemplate* t, float x, float y)
{
    if (!GameObject::init(t, x, y))
    {
        return false;
    }

    m_startTime = Clock::now();
    m_duration = 0.0f;

    return true;
}

void ParticleEffect::present()
{
    ParticleShaderProgram& program = ParticleShaderProgram::getInstance();
    const ParticleEffectTemplate* t = getTemplate();

    program.setRef(m_pos);
    program.setAcceleration(t->getAcceleration());
    program.setInitSpeed(t->getInitSpeed());
    program.setColor(t->getColor()->getColor());
    program.setParticleSize(t->getParticleSize());
    program.setCurTime(m_duration);
    program.setUseTex(true);
    program.setTexture(t->getTexture()->textureId());
    program.bindData(t->getVertexArray());

    glDrawArrays(GL_POINTS, 0, t->getNumParticles());
}

void ParticleEffect::update(float delta, GameScreen& screen)
{
    if (screen.getMap().isOutsideViewport(this)) {
        onDeath(screen);
        return;
    }

    m_duration = elapsedTimeMs(m_startTime);
    if (m_duration >= getTemplate()->getDuration())
    {
        onDeath(screen);
    }
}

void ParticleEffect::onDeath(GameScreen& screen)
{
    GameObjectManager& gameObjMgr = screen.getGameObjManager();
    gameObjMgr.sendToDeathQueue(this);
}

} // end of namespace bot

