
#ifndef SIMULATION_H
#define SIMULATION_H

#include "ParticleSource.h"
#include "Particle.h"
#include "Globals.h"
#include "RoundLine.h"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

class Simulation {

public:
    std::vector<Particle> particles;
    std::vector<ParticleSource> particleSources;

    std::vector<RoundLine> roundLines;

    float domainWidth = 80;
    float domainHeight = 160;

    float scalingFactorForWorld = WINDOW_WIDTH / domainWidth;

    float smoothingRadius = 5;

    float smoothingRadiusSquared = smoothingRadius * smoothingRadius;

    // mass of a particle
    float M = 1.0;

    // for collisions with boundaries
    float dampingFactor = 0.9;

    float isentropicExponent = 40.0;

    float baseDensity = 3.;

    float dynamicViscosity = 1.;

    float brownianAmplitude = 1;

    float densityNormalization = 315 * M / (64 * PI * std::pow(smoothingRadius, 9));

    float pressureForceNormalization = (-45 * M) / (PI * std::pow(smoothingRadius, 6));

    float viscosityNormalization = (45 * dynamicViscosity * M) / (PI * std::pow(smoothingRadius, 6));

    Simulation();

    void addSource(ParticleSource& source);
    void addLine(RoundLine& roundLine);

    void step(float deltaTime);
    void drawParticles(sf::RenderWindow& window);

    void drawLines(sf::RenderWindow& window);
};

#endif