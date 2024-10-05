#include "../include/Particle.h"

Particle::Particle(sf::Vector2f position, sf::Vector2f velocity) : position(position), velocity(velocity){
    pressure = 1.0;
    density = 1.0;

    pressureForce = sf::Vector2f(0,0);
    viscosityForce = sf::Vector2f(0,0);
    force = sf::Vector2f(0,0); 
}