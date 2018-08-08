//C++ SFML Smart Rockets Genetic Algorithm
//This is a C++ port of code from Daniel Shiffman's SmartRockets video
//https://www.youtube.com/watch?v=bGz7mv2vD6g
//https://github.com/CodingTrain/website/tree/9f0f94dd0495840ab037ac172c4575dba9dafd62/CodingChallenges/CC_029_SmartRockets

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ExtraFuncs.h"

class DNA {
public:
	std::vector<sf::Vector2f> genes;
	float mag = 0.2;
	float mutationChance; // for harder courses you may need to adjust this

	DNA() {

	}

	DNA(int lifespan, float mutationChance) {
		this->mutationChance = mutationChance;
		for (int i = 0; i < lifespan; i++) {
			genes.push_back(randomVector());
		}
	}

	DNA(std::vector<sf::Vector2f> genes, float mutationChance) {
		this->genes = genes;
		this->mutationChance = mutationChance;
	}

	sf::Vector2f randomVector() {
		return sf::Vector2f(RandomDouble(-1*mag, 1*mag), RandomDouble(-1*mag, 1*mag));
	}

	DNA crossover(DNA partner) {
		std::vector<sf::Vector2f> newGenes;
		for (int i = 0; i < genes.size(); i++) {
			int mid = rNum(genes.size());
			if (i > mid) {
				newGenes.push_back(genes[i]);
			} else {
				newGenes.push_back(partner.genes[i]);
			}
		}
		return DNA(newGenes, mutationChance);
	}

	void mutation() {
		for (int i = 0; i < genes.size(); i++) {
			if (RandomDouble(0, 1) < mutationChance) {
				genes[i] = randomVector();
			}
		}
	}
};