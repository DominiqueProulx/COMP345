#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <iostream>


// --------DUMMY CLASSES --------
//to Stub Implementation from other team members for now


class DummyOrders {
public:
	void DummyOrders::validate() { std::cout << "Dummy Order validate\n"; };
	void DummyOrders::execute() { std::cout << "Dummy Order Execute\n"; };

};

class DummyOrderList {
public:
	void DummyOrderList::move() { std::cout << "Dummy Orderlist move\n"; }
	void DummyOrderList::removee() { std::cout << "Dummy Orderlist remove\n"; }
	void DummyOrderList::execute() { std::cout << "Dummy Orderlist execute\n"; }
	void DummyOrderList::validate() { std::cout << "Dummy Orderlist validate\n"; }
};


class DummyCards {

private:
	std::string type;
public:
	DummyCards(std::string t) : type(t) {}
	DummyOrders* DummyCards::play() {
		std::cout << "Dummy Cards processing\n This returns an order";
		return new DummyOrders();
	}
	const std::string getType() { return type; }

};

class DummyHand {
public:
	std::vector<DummyCards*> cards;
};



class DummyTerritory {
public:
	std::string* name;
	DummyTerritory::DummyTerritory(const std::string& territoryName) {
		name = new std::string(territoryName);
	}
	DummyTerritory(const DummyTerritory& territoryToCopy) {
		name = new std::string(*territoryToCopy.name);
	}
	 
};

//Setup the Dummy classes 
using Hand = DummyHand;
using OrderList = DummyOrderList;
using Territory = DummyTerritory;
using Orders =  DummyOrders;
using Cards = DummyCards;

// ------------------ Struct to keep track of the armies ------------------

//Struct  definintion to hold a pointer to a territory and the number of armies on it
struct TerritoriesWithArmies {
	Territory* territory;
	int armies;
};

// ------------------ CLASS PLAYER ------------------

class Player {
public:
	static int playerCount;


	//Constructors
	Player(const std::string& color, const std::vector<TerritoriesWithArmies*>& initialTerritories);
	Player(const Player& other); //Copy Constructor

	//Destrcutor 
	~Player();

	//Assignment Operator
	//Player& operator=(const Player& other);

	 // Stream insertion operator (declare as friend)
	//friend std::ostream& operator<<(std::ostream& os, const Player& player);

	 //getters and setters
	const int* getID() const;
	const std::string* getColor() const;
	const std::vector<TerritoriesWithArmies*>* getTerritories() const;
	Hand* getHand() const;
	OrderList* getOrders() const;

	void setColor(const std::string& color);

	// Methods
	std::vector<TerritoriesWithArmies*> Player::toDefend();
	std::vector<Territory*> Player::toAttack(const std::vector<Territory*>& possibleTerritories);
	// //issueOrder();

private:
	const int* playerID;
	const std::string* playerColor;
	//A vector of territories owned by the player, each has a number of armies on it.  The vector constains pointers to the TerritoriesWithArmies struct
	std::vector<TerritoriesWithArmies*>* territories;
	Hand* hand;
	OrderList* orders;
};

#endif