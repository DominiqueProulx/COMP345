#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <queue>
#include <iostream>


// --------DUMMY CLASSES --------
//to Stub Implementation from other team members for now

class DummyOrders {
public:
	std::string type;
	DummyOrders() {
		type = "deploy";
	}
	DummyOrders(std::string t) : type(t) {}
	void validate() { std::cout << "Dummy Order validate\n"; };
	void execute() { std::cout << "Dummy Order Execute\n"; };

	friend std::ostream& operator<<(std::ostream& os, const DummyOrders& order) {
		os << "Order: " << order.type;
		return os;
		
	}
};

class DummyOrderList {
public:
	std::vector<DummyOrders*>* orders;
	DummyOrderList() {
		orders = new std::vector<DummyOrders*>();
	}
	std::vector<DummyOrders*>* getOrderList() { return orders; }
	void move() { std::cout << "Dummy Orderlist move\n"; }
	void remove() { std::cout << "Dummy Orderlist remove\n"; }
	void execute() { std::cout << "Dummy Orderlist execute\n"; }
	void validate() { std::cout << "Dummy Orderlist validate\n"; }
};

class DummyCards {

private:
	std::string name;
public:
	DummyCards() {
		name = "deploy";
	}
	DummyCards(std::string t) : name(t) {}
	DummyOrders* play() {
		std::cout << "Dummy Cards processing. This returns an order\n";
		return new DummyOrders();
	}
	DummyOrders* play(std::string& cardName) {
		std::cout << "Dummy Cards processing. This returns an order\n";
		return new DummyOrders(cardName);
	}
	const std::string getName() const{ return name; }

};

class DummyHand {
public:
	std::vector<DummyCards*>* hand;
	DummyHand() {
		hand = new std::vector<DummyCards*>();
	}
	void addCard(DummyCards* card) 
		{
			(*hand).push_back(card);
		}
	
};

class DummyTerritory {
public:
	std::string* name;
	DummyTerritory(const std::string& territoryName) {
		name = new std::string(territoryName);
	}
	DummyTerritory(const DummyTerritory& territoryToCopy) {
		name = new std::string(*territoryToCopy.name);
	}
	std::string getName() const { return *name; }
	std::vector<DummyTerritory*> getAdjacentTerritories() {
		DummyTerritory* adjTerritory = new DummyTerritory("adjTerritory");
		DummyTerritory* adjTerritory2 = new DummyTerritory("adjTerritory");
		std::vector<DummyTerritory*> adjTerritories;
		adjTerritories.push_back(adjTerritory);
		adjTerritories.push_back(adjTerritory2);
		return adjTerritories;
		
	}
};

//Setup the Dummy classes 
using Hand = DummyHand;
using OrderList = DummyOrderList;
using Territory = DummyTerritory;
using Orders =  DummyOrders;
using Cards = DummyCards;

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
Player(const std::string& color, const std::vector<TerritoriesWithArmies*>& initialTerritories);
//Copy Constructor
Player(const Player& other); 

//Destrcutor 
	~Player();
	

	 //getters and setters
	const int* getID() const;
	const std::string* getColor() const;
	const std::vector<TerritoriesWithArmies*>* getTerritories() const;
	Hand* getHand() const;
	OrderList* getOrdersList() const;

	void setColor(const std::string& color);

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
	OrderList* orderlist;
};

#endif