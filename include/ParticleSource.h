
#ifndef PARTICLE_SOURCE_H
#define PARTICLE_SOURCE_H

#include <SFML/System/Vector2.hpp>
#include "Particle.h"
#include "Globals.h"

class ParticleSource {

public:

    sf::Vector2f position;
    sf::Vector2f initialVelocity;

    float sourceWidth;
    float duration;
    float timeBetweenParticles;

    float timeSinceLastParticle;
    float timeSinceSourceCreation;
    bool isActive;

    ParticleSource(sf::Vector2f position, float sourceWidth, sf::Vector2f initialVelocity, float duration, float timeBetweenParticles);

    bool canAddNewParticle(float dt);

    void initializeParticle(Particle& particle);
};

#endif