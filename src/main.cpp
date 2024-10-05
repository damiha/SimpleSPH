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

    bool mousePressed = false;
    sf::Vector2f mousePosition;
    sf::Vector2f prevMousePosition;

    Simulation sim = Simulation();

    ParticleSource topSource = ParticleSource(sf::Vector2f(35, 10), 10, sf::Vector2f(0, 3), 20, 0.1);

    RoundLine topLineLeft = RoundLine(sf::Vector2f(5, 60), sf::Vector2f(40, 80), 2.5, sim.scalingFactorForWorld);
    RoundLine topLineRight = RoundLine(sf::Vector2f(65, 90), sf::Vector2f(35, 110), 2.5, sim.scalingFactorForWorld);
    RoundLine midCircle = RoundLine(sf::Vector2f(35, 150), 6, sim.scalingFactorForWorld);


    sim.addSource(topSource);
    sim.addLine(topLineLeft);
    sim.addLine(topLineRight);

    sim.addLine(midCircle);

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

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                mousePressed = true;
                mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                // to prevent jumping
                prevMousePosition = sf::Vector2f(mousePosition);
            }
            
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                mousePressed = false;
                mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
            
            if (event.type == sf::Event::MouseMoved){
                mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            }
        
        }

        sf::Time currTime = clock.getElapsedTime();
        int microsecondsBetweenFrames = currTime.asMicroseconds() - prevTime.asMicroseconds();
        prevTime = currTime;

        float deltaTime = microsecondsBetweenFrames / (1000000.0f);
        int fps = (int)(1.0 / deltaTime);

        // move the obstacles BEFORE the simulation runs
        if(mousePressed){

            sf::Vector2f mouseDeltaInWorldCoords = (1.0f / sim.scalingFactorForWorld) * (mousePosition - prevMousePosition);

            for(RoundLine& line : sim.roundLines){

                RoundedLineCollision collision = line.intersectWithMouse(mousePosition);

                if(collision == RoundedLineCollision::BOTH || collision == RoundedLineCollision::START){
                    line.startPos += mouseDeltaInWorldCoords;
                }

                if(collision == RoundedLineCollision::BOTH || collision == RoundedLineCollision::END){
                    line.endPos += mouseDeltaInWorldCoords;
                }
            }
        }

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
        sim.drawLines(window);

        infoText.setString("FPS: " + std::to_string(fps) + "\nparticles: " + std::to_string((int)sim.particles.size()));
        window.draw(infoText);

        window.display();

        // last updates
        prevMousePosition = sf::Vector2f(mousePosition);
    }

    return 0;
}