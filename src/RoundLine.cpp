#include "../include/RoundLine.h"

RoundLine::RoundLine(sf::Vector2f startPos, sf::Vector2f endPos, float radius, float scalingFactorForWorld) :
 startPos(startPos), endPos(endPos), radius(radius), scalingFactorForWorld(scalingFactorForWorld)
{
    
}

RoundLine::RoundLine(sf::Vector2f pos, float radius, float scalingFactorForWorld) : RoundLine(pos, pos, radius, scalingFactorForWorld)
{
    
}

void RoundLine::draw(sf::RenderWindow &window)
{
    // Calculate scaled radius and positions
    float scaledRadius = radius * scalingFactorForWorld;
    sf::Vector2f scaledStart = startPos * scalingFactorForWorld;
    sf::Vector2f scaledEnd = endPos * scalingFactorForWorld;

    // Create the line
    sf::Vector2f direction = scaledEnd - scaledStart;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::RectangleShape line(sf::Vector2f(length, 2 * scaledRadius));
    line.setOrigin(0, scaledRadius);
    line.setPosition(scaledStart);
    line.setRotation(std::atan2(direction.y, direction.x) * 180 / M_PI);
    line.setOutlineColor(sf::Color::White);
    line.setOutlineThickness(1);
    line.setFillColor(sf::Color::Transparent);

    // Create start and end circles
    sf::CircleShape startCircle(scaledRadius);
    sf::CircleShape endCircle(scaledRadius);
    
    startCircle.setOrigin(scaledRadius, scaledRadius);
    endCircle.setOrigin(scaledRadius, scaledRadius);
    
    startCircle.setPosition(scaledStart);
    endCircle.setPosition(scaledEnd);
    
    startCircle.setFillColor(sf::Color::Transparent);
    endCircle.setFillColor(sf::Color::Transparent);
    
    startCircle.setOutlineThickness(1);
    endCircle.setOutlineThickness(1);
    
    startCircle.setOutlineColor(sf::Color::White);
    endCircle.setOutlineColor(sf::Color::White);

    // Draw all elements
    window.draw(line);
    window.draw(startCircle);
    window.draw(endCircle);
}