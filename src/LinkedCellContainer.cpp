#include "../include/LinkedCellContainer.h"
#include "LinkedCellContainer.h"

void LinkedCellContainer::initLinkedCellContainer(float domainWidth, float domainHeight, float smoothingRadius)
{   

    this->domainHeight = domainHeight;
    this->domainWidth = domainWidth;
    this->smoothingRadius = smoothingRadius;

    cellLength = 3.f * smoothingRadius;

    nX = (int)((domainWidth / cellLength) + 1);
    nY = (int)((domainHeight / cellLength) + 1);

    // two extra cells, one on top and one on the bottom
    nCellsToCreate = (nX + 2) * (nY + 2);

    linkedCells.resize(nCellsToCreate);

    for(int i = 0; i < nCellsToCreate; i++){
        linkedCells[i] = std::vector<int>();
    }

    printf("INFO: created %d empty linked cells\n", nCellsToCreate);
}

void LinkedCellContainer::buildLinkedCells(std::vector<Particle> &particles)
{
    resetLinkedCells();

    for(int i = 0; i < particles.size(); i++){
        
        int linearIndex = getLinearIndex(particles[i]); 
        linkedCells[linearIndex].push_back(i);
    }
}

int LinkedCellContainer::getLinearIndex(Particle &particle)
{
    int cellX = (int)(particle.position.x / cellLength) + 1;
    int cellY = (int)(particle.position.y / cellLength) + 1;

    int linearIndex = cellY * (nX + 2) + cellX;

    return linearIndex;
}

std::vector<int> &LinkedCellContainer::getIndices(int x, int y)
{
    int linearIndex = y * (nX + 2) + x;
    return linkedCells[linearIndex];
}

void LinkedCellContainer::resetLinkedCells()
{
    for(int i = 0; i < nCellsToCreate; i++){
        if(!linkedCells[i].empty()){
            linkedCells[i].clear();
        }
    }

    //printf("INFO: reset linked cells\n");
}

void LinkedCellContainer::drawOccupancy(sf::RenderWindow &window, float scalingForWorld)
{   
    // TODO: don't hardcode this
    int minInCell = 10000;
    int maxInCell = -1;

    // to find lower and upper end of the color spectrum
    for(std::vector<int>& cell : linkedCells){
        
        int cellSize = cell.size();

        maxInCell = maxInCell > cellSize ? maxInCell : cellSize;
        minInCell = minInCell < cellSize ? minInCell : cellSize;
    }

    float squareLength = cellLength * scalingForWorld;
    sf::RectangleShape shape(sf::Vector2f(squareLength, squareLength));

    for(int y = 1; y <= nY; y++){
        for(int x = 1; x <= nX; x++){

            sf::Vector2f squarePos((x - 1) * squareLength, (y - 1) * squareLength);

            float occupancy = (getIndices(x, y).size() * 1.0f - minInCell) / (maxInCell - minInCell);

            shape.setPosition(squarePos);
            shape.setFillColor(sf::Color(255, 0, 0, (int)(255 * occupancy)));
            window.draw(shape);
        }
    }
}
