
#ifndef LINKED_CELL_CONTAINER_H
#define LINKED_CELL_CONTAINER_H

#include <vector>
#include "Particle.h"
#include <SFML/Graphics.hpp>

class LinkedCellContainer{

public:

    std::vector<std::vector<int>> linkedCells;
    float domainWidth;
    float domainHeight;
    float smoothingRadius;

    // must be at least as big as 'smoothingRadius'
    float cellLength;
    int nY, nX;
    int nCellsToCreate;

    void initLinkedCellContainer(float domainWidth, float domainHeight, float smoothingRadius);

    void buildLinkedCells(std::vector<Particle>& particles);

    int getLinearIndex(Particle& particle);

    std::vector<int>& getIndices(int x, int y);

    void resetLinkedCells();

    void drawOccupancy(sf::RenderWindow& window, float scalingForWorld);

};

#endif