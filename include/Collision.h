
#ifndef COLLISION_H
#define COLLISION_H

#include "SFML/System/Vector2.hpp"

struct Collision {
    
    bool collisionOccurred;
    sf::Vector2f normal;
    float penetrationDepth;
};

#endif