
#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <SFML/System/Vector2.hpp>

class Particle {

public:
    sf::Vector2f position;
    sf::Vector2f velocity;

    // for the Navier-Stokes equations
    sf::Vector2f pressureForce;
    sf::Vector2f viscosityForce;

    sf::Vector2f force;
    
    // density and (static) pressure at that particle
    float density;
    float pressure;

    Particle(sf::Vector2f position, sf::Vector2f velocity);
};

#endif