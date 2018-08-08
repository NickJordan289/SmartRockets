// C++ SFML Smart Rockets Genetic Algorithm
// This is a C++ port of code from Daniel Shiffman's SmartRockets video
// https://www.youtube.com/watch?v=bGz7mv2vD6g
// https://github.com/CodingTrain/website/tree/9f0f94dd0495840ab037ac172c4575dba9dafd62/CodingChallenges/CC_029_SmartRockets

#pragma once
#include <SFML/Graphics.hpp>
#include "DNA.h"
#include "ExtraFuncs.h"

class Rocket {
public:
	sf::RectangleShape r;
	
	sf::Vector2f pos = sf::Vector2f(0.f, 0.f);
	sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
	sf::Vector2f acc = sf::Vector2f(0.f, 0.f);

	int& countRef;
	float fitness;
	bool completed = false;
	bool crashed = false;
	int completionTime;
	int lifespan;
	DNA dna;

	sf::RenderWindow& rWindRef;

	Rocket(int lifespan, float mutationChance, sf::RenderWindow& rWind, int& count) : rWindRef(rWind), countRef(count) {
		this->lifespan = lifespan;
		
		dna = DNA(lifespan, mutationChance);

		pos = sf::Vector2f(rWindRef.getSize().x/2.f, rWindRef.getSize().y);
		
		r.setSize(sf::Vector2f(50.f, 10.f));
		r.setOrigin(r.getGlobalBounds().width / 2.f, r.getGlobalBounds().height / 2.f);
		r.setPosition(pos);
		r.setFillColor(sf::Color(255, 255, 255, 100));
		r.setOutlineColor(sf::Color::Black);
		r.setOutlineThickness(0.5f);
	}

	// mutationChance and lifespan are taken from the existing dna object
	Rocket(DNA dna, sf::RenderWindow& rWind, int& count) : Rocket(dna.mutationChance, dna.genes.size(), rWind, count) {
		this->dna = dna;
	}

	~Rocket() {
		
	}

	void applyForce(sf::Vector2f force) {
		acc += force;
	}

	void update(sf::Vector2f target, sf::RectangleShape obstacle, float dt) {
		float d = dist(pos, target);
		if (!completed && d < 10.f) {
			completed = true;
			completionTime = countRef;
			pos = target; // set to location for max fitness
		}

		if (pos.x > obstacle.getGlobalBounds().left && pos.x < obstacle.getGlobalBounds().left + obstacle.getGlobalBounds().width && pos.y > obstacle.getGlobalBounds().top && pos.y < obstacle.getGlobalBounds().top + obstacle.getGlobalBounds().height) {
			crashed = true;
		}

		if (pos.x > rWindRef.getSize().x || pos.x < 0) {
			crashed = true;
		}

		if (pos.y > rWindRef.getSize().y || pos.y < 0) {
			crashed = true;
		}

		if (countRef < dna.genes.size()) {
			applyForce(dna.genes[countRef]);
		}
		
		if (!completed && !crashed) {
			velocity += acc;
			pos += velocity;
			acc = sf::Vector2f(0.f, 0.f);
		}
	}

	void calcFitness(sf::Vector2f target) {
		float d = dist(pos, target);

		// remaps the range
		fitness = map(d, 0, rWindRef.getSize().x, rWindRef.getSize().x, 0);
	
		if (completed) {
			fitness *= pow((float)dna.genes.size() / (float)completionTime, 2); // completion time multiplier exponential
			fitness *= 10; // very likely to be picked for successful rockets
		}

		if (crashed) {
			fitness = 1;
		}

		fitness = pow(fitness, 2); // fitness becomes exponential
	}

	void draw() {
		
		r.setPosition(pos);

		sf::Vector2f normalizedVelocity = normalize(velocity);
		double angleInRadians = std::atan2(normalizedVelocity.y, normalizedVelocity.x);
		double angleInDegrees = (angleInRadians / 3.1415926535) * 180.0;

		r.setRotation(angleInDegrees);

		rWindRef.draw(r);
	}
};