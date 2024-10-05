#include "../include/Simulation.h"
#include "Simulation.h"

Simulation::Simulation(){
    
    float widthFactor = WINDOW_WIDTH / domainWidth;
    float heightFactor = WINDOW_HEIGHT / domainHeight;

    if(abs(widthFactor - heightFactor) > EPSILON){
        printf("domain ratio and window ratio don't match");
    }
}

void Simulation::drawParticles(sf::RenderWindow& window){

    sf::CircleShape shape(smoothingRadius * scalingFactorForWorld);

    sf::Vector2f offset(-smoothingRadius, -smoothingRadius);
    offset *= scalingFactorForWorld;

    for(Particle& p : particles){

        //print("position", p.position);
        
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

    // density and pressure calculation
    for(int i = 0; i < particles.size(); i++){

        Particle& particleI = particles[i];
        particleI.density = EPSILON;

        for(int j = 0; j < particles.size(); j++){
            Particle& particleJ = particles[j];

            sf::Vector2f distanceVector = particleI.position - particleJ.position;
            float squaredDistance = lengthSq(distanceVector);

            if(squaredDistance > smoothingRadiusSquared){
                continue;
            }
            
            particleI.density += std::pow(smoothingRadiusSquared - squaredDistance, 3);
        }

        particleI.density *= densityNormalization;

        particleI.pressure = isentropicExponent * (particleI.density - baseDensity);

        //print("density", particleI.density);
    }

    // single force calculation
    for(Particle& p : particles){
        p.force = sf::Vector2f(0, 1.0);
    }

    // pairwise force calculation
    for(int i = 0; i < particles.size(); i++){

        Particle& particleI = particles[i];
        particleI.pressureForce = sf::Vector2f(0, 0);
        particleI.viscosityForce = sf::Vector2f(0, 0);

        for(int j = 0; j < particles.size(); j++){

            // skip, otherwise division by zero
            if(j == i){
                continue;
            }

            Particle& particleJ = particles[j];

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

        particleI.pressureForce = pressureForceNormalization * particleI.pressureForce;
        particleI.viscosityForce = viscosityNormalization * particleI.viscosityForce;

        //print("pressure", particleI.pressureForce);

        warnIfAbnormal("pressure force", particleI.pressureForce);
        particleI.force += particleI.pressureForce;

        warnIfAbnormal("viscosity force", particleI.viscosityForce);
        particleI.force += particleI.viscosityForce;

        // prevents clustering and makes more realistic
        particleI.force += sf::Vector2f(randN(brownianAmplitude), randN(brownianAmplitude));
    }

    // integration
    for(Particle& p : particles){

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
    }
}