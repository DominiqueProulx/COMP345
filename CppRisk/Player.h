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
	std::queue<DummyOrders*>* orders;
	DummyOrderList() {
		orders = new std::queue<DummyOrders*>();
	}
	std::queue<DummyOrders*>* getQueue() { return orders; }
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
		std::cout << "Dummy Cards processing\n This returns an order";
		return new DummyOrders();
	}
	DummyOrders* play(std::string& cardName) {
		std::cout << "Dummy Cards processing\n This returns an order";
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
	OrderList* getOrdersList() const;

	void setColor(const std::string& color);

	// Methods
	std::vector<TerritoriesWithArmies*> toDefend();
	std::vector<Territory*> toAttack(const std::vector<Territory*>& possibleTerritories);
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