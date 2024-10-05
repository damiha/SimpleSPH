#include "../include/Simulation.h"
#include "Simulation.h"

Simulation::Simulation(){
    
    float widthFactor = WINDOW_WIDTH / domainWidth;
    float heightFactor = WINDOW_HEIGHT / domainHeight;

    if(abs(widthFactor - heightFactor) > EPSILON){
        printf("domain ratio and window ratio don't match");
    }

    container.initLinkedCellContainer(domainWidth, domainHeight, smoothingRadius);
}

void Simulation::drawParticles(sf::RenderWindow& window){

    sf::CircleShape shape(smoothingRadius * scalingFactorForWorld);

    sf::Vector2f offset(-smoothingRadius, -smoothingRadius);
    offset *= scalingFactorForWorld;

    for(Particle& p : particles){
        
        shape.setPosition(scalingFactorForWorld * p.position + offset);        
        shape.setFillColor(sf::Color(0, 0, 255, 255));
        window.draw(shape);
    }
}

void Simulation::drawLines(sf::RenderWindow &window)
{
    for(RoundLine& roundLine : roundLines){
        roundLine.draw(window);
    }
}

void Simulation::drawGridOccupancy(sf::RenderWindow& window){
    container.drawOccupancy(window, scalingFactorForWorld);
}
void Simulation::addSource(ParticleSource &source)
{
    particleSources.emplace_back(source);
}

void Simulation::addLine(RoundLine &roundLine)
{
    roundLines.emplace_back(roundLine);
}

void Simulation::step(float dt){

    
    // add new particles if necessary
    for(ParticleSource& source : particleSources){

        if(source.isActive && source.canAddNewParticle(dt)){
           
           particles.push_back(Particle(sf::Vector2f(0,0), sf::Vector2f(0,0)));
           source.initializeParticle(particles[particles.size() - 1]);
        }
    }

    container.buildLinkedCells(particles);

    // initialization
    for(Particle& p : particles){
        p.force = sf::Vector2f(0, 1.0);
        p.density = EPSILON;
    }

    // density and pressure calculation
    for(int y = 1; y <= container.nY; y++){
        for(int x = 1; x <= container.nX; x++){

            std::vector<int>& indicesA = container.getIndices(x, y);

            if(indicesA.empty()){
                continue;
            }

            //printf("#particles in linked cell: %d\n", indicesA.size());

            for(int k = -1; k <= 1; k++){
                for(int l = -1; l <= 1; l++){

                    std::vector<int>& indicesB = container.getIndices(x + l, y + k);

                    for(int i = 0; i < indicesA.size(); i++){

                        Particle& particleI = particles[indicesA[i]];

                        for(int j = 0; j < indicesB.size(); j++){
                            Particle& particleJ = particles[indicesB[j]];

                            sf::Vector2f distanceVector = particleI.position - particleJ.position;
                            float squaredDistance = lengthSq(distanceVector);

                            if(squaredDistance > smoothingRadiusSquared){
                                continue;
                            }

                            float squaredRadiusMinusSquaredDistance = smoothingRadiusSquared - squaredDistance;
                            
                            particleI.density += (
                                squaredRadiusMinusSquaredDistance * 
                                squaredRadiusMinusSquaredDistance * 
                                squaredRadiusMinusSquaredDistance
                            );
                        }
                    }
                }
            }
        }
    }

    // normalization & initialization
    for(Particle& p : particles){
        p.density *= densityNormalization;
        p.pressure = isentropicExponent * (p.density - baseDensity);

        p.pressureForce = sf::Vector2f(0, 0);
        p.viscosityForce = sf::Vector2f(0, 0);
    }

    // pairwise force calculation
    for(int y = 1; y <= container.nY; y++){
        for(int x = 1; x <= container.nX; x++){

            std::vector<int>& indicesA = container.getIndices(x, y);

            if(indicesA.empty()){
                continue;
            }

            //printf("#particles in linked cell: %d\n", indicesA.size());

            for(int k = -1; k <= 1; k++){
                for(int l = -1; l <= 1; l++){

                    std::vector<int>& indicesB = container.getIndices(x + l, y + k);

                    for(int i = 0; i < indicesA.size(); i++){

                        Particle& particleI = particles[indicesA[i]];

                        for(int j = 0; j < indicesB.size(); j++){

                            // skip, otherwise division by zero
                            if(indicesB[j] == indicesA[i]){
                                continue;
                            }

                            Particle& particleJ = particles[indicesB[j]];

                            sf::Vector2f distanceVector = particleI.position - particleJ.position;
                            float dIJ = std::sqrt(lengthSq(distanceVector)) + EPSILON;

                            if(dIJ >= smoothingRadius){
                                continue;
                            }
                            
                            particleI.pressureForce += 
                                (-(particleJ.position - particleI.position) / dIJ) *
                                (particleI.pressure + particleJ.pressure) / (2 * particleJ.density) *
                                (smoothingRadius - dIJ) * (smoothingRadius - dIJ);

                            particleI.viscosityForce += ((particleJ.velocity - particleI.velocity) / particleJ.density) * (smoothingRadius - dIJ);
                        }
                    }
                }
            }
        }
    }

    // normalization + force summation + integration
    for(Particle& p : particles){

        p.pressureForce = pressureForceNormalization * p.pressureForce;
        p.viscosityForce = viscosityNormalization * p.viscosityForce;
        
        warnIfAbnormal("pressure force", p.pressureForce);
        p.force += p.pressureForce;

        warnIfAbnormal("viscosity force", p.viscosityForce);
        p.force += p.viscosityForce;

        // prevents clustering and makes more realistic
        p.force += sf::Vector2f(randN(brownianAmplitude), randN(brownianAmplitude));

        // integration
        p.velocity += dt * (1.0f / p.density) * p.force;
        p.position += dt * p.velocity;
    }

    // enforce boundary conditions
    for(Particle& p : particles){
        
        // left
        if(p.position.x - smoothingRadius < 0){
            p.position.x = smoothingRadius;
            p.velocity.x *= -dampingFactor;
        }

        // right
        if(p.position.x + smoothingRadius > domainWidth){
            p.position.x = domainWidth - smoothingRadius;
            p.velocity.x *= -dampingFactor;
        }

        // bottom
        if(p.position.y + smoothingRadius > domainHeight){
            p.position.y = domainHeight - smoothingRadius;
            p.velocity.y *= -dampingFactor;
        }

        // top
        if(p.position.y - smoothingRadius < 0){
            p.position.y = smoothingRadius;
            p.velocity.y *= -dampingFactor;
        }

        // enforce boundary conditions enforced by obstacles
        for(RoundLine& line : roundLines){
            
            Collision collision = line.intersect(p, smoothingRadius);

            if(collision.collisionOccurred){
                
                p.position += (collision.normal * collision.penetrationDepth);

                sf::Vector2f normalVelocity = dot(collision.normal, p.velocity) * collision.normal;

                //sf::Vector2f tangentVelocity = p.velocity - normalVelocity;
                // velocity := tangentVelocity - 2 * normalVelocity
                p.velocity -= ((1.f + dampingFactor) * normalVelocity);
            }
        }
    }
}