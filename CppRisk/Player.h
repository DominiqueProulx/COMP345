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


// ------------------ CLASS PLAYER ------------------

class Player {
public:
	static int playerCount;


//Constructor
Player();
Player(const std::string& color, const std::vector<Territory*>& initialTerritories, Deck* deck);
//Copy Constructor
Player(const Player& other); 

//Destrcutor 
	~Player();
	

	 //getters and setters
	const int* getID() const;
	const std::string* getColor() const;
	const std::vector<Territory*>* getTerritories() const;
	Hand* getHand() const;
	OrdersList* getOrdersList() const;

	void setColor(const std::string& color);
	void setHand(Hand& hand);

	//Steam insertion operator
	friend std::ostream& operator<<(std::ostream& os, const Player& player);

	 //Assignment Operator
	Player& operator= (const Player& otherplayer);

	// Methods
	std::vector<Territory*> toDefend();
	std::vector<Territory*> toAttack();
	void issueOrder();

private:
	const int* playerID;
	const std::string* playerColor;
	std::vector<Territory*>* territoriesOwned;
	Hand* playerHand;
	OrdersList* orderslist;
	Deck* deck;
};

#endif