
#ifndef PARTICLE_SOURCE_H
#define PARTICLE_SOURCE_H

#include <SFML/System/Vector2.hpp>
#include "Particle.h"
#include "Globals.h"

class ParticleSource {

public:

    float sourceWidth;
    float angularSpread;
    float duration;
    float timeBetweenParticles;

    float timeSinceLastParticle;
    float timeSinceSourceCreation;
    bool isActive;

    sf::Vector2f position;
    sf::Vector2f initialVelocity;

    ParticleSource(sf::Vector2f position, float sourceWidth, sf::Vector2f initialVelocity, float duration, float timeBetweenParticles);

    bool canAddNewParticle(float dt);

    void initializeParticle(Particle& particle);
};

#endif