// COMP 345 - Fall 2025
// Risk Game Project
// Part 2 : Player 
// File: Player.h

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include "Map.h"
#include "Orders.h"
#include "Cards.h"



// ------------------ Struct to keep track of the armies ------------------

//User defined Strcut to hold a pointer to a territory and the number of armies on it
struct TerritoriesWithArmies {
	Territory* territory;
	int* armies;


// Constructor
TerritoriesWithArmies()
		: territory(nullptr), armies(nullptr) {
	}
TerritoriesWithArmies(Territory* terr, int ar) {
	territory = terr;
	armies = new int(ar);
};

//Copy Constructor
TerritoriesWithArmies(const TerritoriesWithArmies& otherTwa) {
	territory = new Territory(*(otherTwa.territory));
	armies = new int(*(otherTwa.armies));
};
// Destructor
~TerritoriesWithArmies() {
	delete territory;
	delete armies;
	territory = nullptr;
	armies = nullptr;
};
// Assignment operator
TerritoriesWithArmies& operator=(const TerritoriesWithArmies& otherTwa) {
	if (this != &otherTwa) {
		delete territory;
		delete armies;
		territory = new Territory(*(otherTwa.territory));
		armies = new int(*(otherTwa.armies));
	}
	return *this;
}};

// ------------------ CLASS PLAYER ------------------

class Player {
public:
	static int playerCount;


//Constructor
Player();
Player(const std::string& color, const std::vector<TerritoriesWithArmies*>& initialTerritories, Deck* deck);
//Copy Constructor
Player(const Player& other); 

//Destrcutor 
	~Player();
	

	 //getters and setters
	const int* getID() const;
	const std::string* getColor() const;
	const std::vector<TerritoriesWithArmies*>* getTerritories() const;
	Hand* getHand() const;
	OrdersList* getOrdersList() const;

	void setColor(const std::string& color);
	void setHand(Hand& hand);

	//Steam insertion operator
	friend std::ostream& operator<<(std::ostream& os, const Player& player);

	 //Assignment Operator
	Player& operator= (const Player& otherplayer);

	// Methods
	std::vector<TerritoriesWithArmies*> toDefend();
	//std::vector<Territory*> toAttack(const std::vector<Territory*>& possibleTerritories);
	std::vector<Territory*> toAttack();
	void issueOrder();

private:
	const int* playerID;
	const std::string* playerColor;
	//A vector of territories owned by the player, each has a number of armies on it.  The vector constains pointers to the TerritoriesWithArmies struct
	std::vector<TerritoriesWithArmies*>* territories;
	Hand* playerHand;
	OrdersList* orderslist;
	Deck* deck;
};

#endif