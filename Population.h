// C++ SFML Smart Rockets Genetic Algorithm
// This is a C++ port of code from Daniel Shiffman's SmartRockets video
// https://www.youtube.com/watch?v=bGz7mv2vD6g
// https://github.com/CodingTrain/website/tree/9f0f94dd0495840ab037ac172c4575dba9dafd62/CodingChallenges/CC_029_SmartRockets

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Rocket.h"
#include "ExtraFuncs.h"

class Population {
public:

	std::vector<Rocket*> rockets;
	std::vector<Rocket*> matingPool;
	int popSize;
	int& countRef;
	sf::Vector2f target;
	sf::RectangleShape obstacle;
	int lifespan;
	float mutationChance;

	int mut = 0;

	sf::RenderWindow& rWindRef;

	Population(int popSize, int lifespan, float mutationChance, sf::Vector2f target, sf::RectangleShape obstacle, sf::RenderWindow& rWind, int& count) : rWindRef(rWind), countRef(count) {
		this->popSize = popSize;
		this->lifespan = lifespan;
		this->mutationChance = mutationChance;
		this->target = target;
		this->obstacle = obstacle;

		for (int i = 0; i < popSize; i++)
		{
			rockets.push_back(new Rocket(lifespan, mutationChance, rWind, countRef));
		}
	}

	~Population() {
		//free_pointer_vector_memory(rockets);
	}

	std::tuple<float, Rocket*> evaluate() {
		float highestFitness = 0;
		Rocket* highestRocket = nullptr;
		for (Rocket* r : rockets) {
			r->calcFitness(target);
			if (r->fitness > highestFitness) {
				highestFitness = r->fitness;
				highestRocket = r;
			}
		}

		// normalizes fitness
		for (Rocket* r : rockets) {
			r->fitness /= highestFitness;
		}

		matingPool.clear();
		for (Rocket* r : rockets) {
			int n = r->fitness * 100;
			for (int i = 0; i < n; i++) {
				matingPool.push_back(r);
			}
		}
		return std::make_tuple(highestFitness, highestRocket);
	}

	int getCompleted() {
		int count = 0;
		for (auto r : rockets) {
			if (r->completed)
				count++;
		}
		return count;
	}

	void selection() {
		std::vector<Rocket*> newRockets;
		for (int i = 0; i < rockets.size(); i++) {
			int randomIndex = rNum(matingPool.size());
			DNA parentA = matingPool[randomIndex]->dna;
			randomIndex = rNum(matingPool.size());
			DNA parentB = matingPool[randomIndex]->dna;
			DNA child = parentA.crossover(parentB);

			child.mutation();

			newRockets.push_back(new Rocket(child, rWindRef, countRef));
		}

		rockets = newRockets;
	}

	void update(float dt) {
		for (Rocket* r : rockets) {
			r->update(target, obstacle, dt);
		}
	}

	void draw() {
		for (Rocket* r : rockets) {
			r->draw();
		}
	}
};