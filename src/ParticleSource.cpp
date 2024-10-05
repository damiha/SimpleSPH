
#include "../include/ParticleSource.h"

ParticleSource::ParticleSource(sf::Vector2f position, float sourceWidth, sf::Vector2f initialVelocity, float duration, float timeBetweenParticles) : 
    position(position), sourceWidth(sourceWidth), initialVelocity(initialVelocity), duration(duration), timeBetweenParticles(timeBetweenParticles) {

        timeSinceLastParticle = 0;
        timeSinceSourceCreation = 0;
        isActive = true;
}

bool ParticleSource::canAddNewParticle(float dt)
{
    timeSinceLastParticle += dt;
    timeSinceSourceCreation += dt;

    if(timeSinceSourceCreation > duration){
        isActive = false;
        printf("INFO: Source deactivated\n");
    }

    if(timeSinceLastParticle > timeBetweenParticles){
        timeSinceLastParticle = 0;
        return true;
    }

    return false;
}
void ParticleSource::initializeParticle(Particle &particle)
{
    particle.position = sf::Vector2f(position);
    particle.position += sf::Vector2f(randF(0, sourceWidth), 0);
    particle.velocity = sf::Vector2f(initialVelocity);
}