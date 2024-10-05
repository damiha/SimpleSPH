
#ifndef ROUND_LINE_H
#define ROUND_LINE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include <cmath>
#include "Globals.h"

class RoundLine{

public:
    sf::Vector2f startPos, endPos;
    float radius;
    float squaredRadius;
    
    float scalingFactorForWorld;

    bool isCircle = false;

    RoundLine(sf::Vector2f pos, float radius, float scalingFactorForWorld);
    RoundLine(sf::Vector2f startPos, sf::Vector2f endPos, float radius, float scalingFactorForWorld);

    void draw(sf::RenderWindow& window);

    void intersect(Particle& particle);

    sf::Vector2f getProjected(sf::Vector2f& position);

    RoundedLineCollision intersectWithMouse(sf::Vector2f mousPosition);
};

#endif