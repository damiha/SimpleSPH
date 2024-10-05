#include <SFML/Graphics.hpp>
#include "../include/Particle.h"
#include "../include/Simulation.h"
#include "../include/Globals.h"
#include "../include/ParticleSource.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SimpleSPH");

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(12);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 10);

    sf::Shader waterShader;
    if (!waterShader.loadFromFile("./src/water.frag", sf::Shader::Fragment)) {
        std::cerr << "Error loading shader\n";
        return 1;
    }

    ParticleSource topSource = ParticleSource(sf::Vector2f(35, 10), 10, sf::Vector2f(0, 3), 20, 0.1);

    Simulation sim = Simulation();
    sim.addSource(topSource);

    sf::Clock clock;
    sf::Time prevTime = clock.getElapsedTime();

    // create a texture and a sprite to apply the shader to
    sf::Texture waterTexture;
    waterTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::Sprite waterSprite(waterTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time currTime = clock.getElapsedTime();
        int microsecondsBetweenFrames = currTime.asMicroseconds() - prevTime.asMicroseconds();
        prevTime = currTime;

        float deltaTime = microsecondsBetweenFrames / (1000000.0f);
        int fps = (int)(1.0 / deltaTime);

        sim.step(deltaTime);

        // set info for the water shader
        std::vector<sf::Vector2f> pixelPositions;

        for(Particle& p : sim.particles){
            sf::Vector2f pixelPos = sim.scalingFactorForWorld * p.position;
            pixelPos.y = WINDOW_HEIGHT - pixelPos.y;
            pixelPositions.push_back(std::move(pixelPos));
        }

        int pointCount = std::min(MAX_POINTS_FOR_SHADER, static_cast<int>(sim.particles.size()));

        waterShader.setUniform("pointCount", pointCount);
        waterShader.setUniform("kernelRadiusInPixels", (int)(sim.smoothingRadius * sim.scalingFactorForWorld));
        waterShader.setUniformArray("pixelPositions", pixelPositions.data(), pixelPositions.size());

        window.clear();
        
        //sim.drawParticles(window);
        window.draw(waterSprite, &waterShader);

        infoText.setString("FPS: " + std::to_string(fps) + "\nparticles: " + std::to_string((int)sim.particles.size()));
        window.draw(infoText);

        window.display();
    }

    return 0;
}