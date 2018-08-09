#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "Population.h"
#include "Rocket.h"
#include "DNA.h"

int main() {
	const float PI = 3.1415926535;

	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 600;

	const unsigned int FPS = 0; //The desired FPS. (The number of updates each second) or 0 for uncapped.
	float timescale = 1.f;

	srand(static_cast<unsigned int>(time(NULL)));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), ("Smart Rockets Genetic Algorithm"), sf::Style::Default, settings);
	window.setFramerateLimit(FPS);

	sf::Color clearColour = sf::Color(128, 128, 128);

	sf::Font ArialFont;
	ArialFont.loadFromFile("C:/Windows/Fonts/arial.ttf");

	sf::RectangleShape obstacle; // object for the rockets to avoid
	obstacle.setSize(sf::Vector2f(200, 20));
	obstacle.setOrigin(obstacle.getGlobalBounds().width / 2, obstacle.getGlobalBounds().height / 2);
	obstacle.setPosition(SCREEN_WIDTH / 2, 400);
	obstacle.setFillColor(sf::Color::White);
	obstacle.setOutlineColor(sf::Color::Black);
	obstacle.setOutlineThickness(-1.f);

	int generations = 0; // generation counter
	int lifeCounter = 0; // frame counter
	
	// controls
	sf::Vector2f target = sf::Vector2f(SCREEN_WIDTH / 2, 50); // goal to reach
	int lifespan = 400; // frames per generation
	int populationSize = 200; // agents per generation
	float mutationChance = 0.0002; // DNA mutation rate
	
	Population pop = Population(populationSize, lifespan, mutationChance, target, obstacle, window, lifeCounter);
	std::cout << "Hold space to speed up" << std::endl;

#pragma region Graphics
	sf::Text lifeCounterText;
	lifeCounterText.setFont(ArialFont);
	lifeCounterText.setFillColor(sf::Color::Black);

	sf::Text generationsText;
	generationsText.setFont(ArialFont);
	generationsText.setFillColor(sf::Color::Black);
	generationsText.setPosition(0, 30);

	sf::CircleShape targetCircle; // graphical representation only
	targetCircle.setRadius(16);
	targetCircle.setOrigin(targetCircle.getGlobalBounds().width / 2, targetCircle.getGlobalBounds().height / 2);
	targetCircle.setPosition(target);
	targetCircle.setFillColor(sf::Color::White);
	targetCircle.setOutlineColor(sf::Color::Black);
	targetCircle.setOutlineThickness(-1.f);
#pragma endregion

	bool paused = false;
	sf::Event ev;
	sf::Clock clock;
	float lastDeltaTime = 0.f;
	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();
		deltaTime *= timescale;
		float fps = 1.f / deltaTime;
		lastDeltaTime = deltaTime;

		//Handle events
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
				window.close();

			if (ev.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.close();
				}
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					window.setFramerateLimit(0); // unlimited framerate for faster generations
				}
			}
			if (ev.type == sf::Event::KeyReleased) {
				if (ev.key.code == sf::Keyboard::Space) {
					window.setFramerateLimit(60); // standard framerate thats easier to view the rockets
				}
			}
		}

		// Update stuff
		if (!paused) {
			pop.update(deltaTime);

			lifeCounter++;
			if (lifeCounter == lifespan) {
				std::tuple<float, std::shared_ptr<Rocket>> bestRocket = pop.naturalSelection(); // evaluates and retrieves best rocket
				
				if (std::get<1>(bestRocket)->completed) {
					std::cout << "Fastest rocket: " << std::get<1>(bestRocket)->completionTime << std::endl;
				} else {
					std::cout << "Best fitness: " << std::get<0>(bestRocket) << std::endl;
				}
				
				std::cout << ((float)pop.getCompleted() / (float)populationSize) * 100.f << "% completed" << std::endl;

				pop.selection();
				lifeCounter = 0;
				generations++;
			}
			generationsText.setString(std::to_string(generations));
			lifeCounterText.setString(std::to_string(lifeCounter));
		}

		// Draw stuff
		window.clear(clearColour);

		pop.draw();

		window.draw(targetCircle);
		window.draw(obstacle);

		window.draw(lifeCounterText);
		window.draw(generationsText);

		window.display();
	}

	return 0;
}