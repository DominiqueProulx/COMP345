#ifndef ORDERS_H
#define ORDERS_H

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <memory>
#include "LoggingObserver.h"

// Include actual implementations from teammates
#include "Map.h"
#include "Player.h"

// Note: Territory and Player classes are now from Map.h and Player.h
// We removed the placeholder classes that were causing conflicts

// -------- ORDER BASE CLASS --------
// Abstract base class for all order types
class Order : public Subject, public ILoggable {
public:
	// Constructors and destructor
	Order(const std::string& orderType);
	Order(const Order& other);
	virtual ~Order();

	// Assignment operator
	Order& operator=(const Order& other);

	// Pure virtual methods - must be implemented by subclasses
	virtual bool validate() = 0;
	virtual void execute() = 0;

	// Virtual clone method for deep copying in OrdersList
	virtual Order* clone() const = 0;

	// logging function
	std::string stringToLog() const override;

	// Getters
	std::string getType() const;
	std::string getEffect() const;
	bool isExecuted() const;

	// Stream insertion operator
	friend std::ostream& operator<<(std::ostream& os, const Order& order);

protected:
    std::string* orderType;
    std::string* effect;
    bool* executed;
};

// -------- ORDER SUBCLASSES WITH GAME PARAMETERS --------

// Deploy order: deploy armies from reinforcement pool to owned territory
class Deploy : public Order {
public:
    Deploy();  // Default constructor for Assignment 1 compatibility
    Deploy(Player* player, Territory* target, int numArmies);
    Deploy(const Deploy& other);
    ~Deploy();
    Deploy& operator=(const Deploy& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    
private:
    Player* player;
    Territory* targetTerritory;
    int* armiesToDeploy;
};

// Advance order: move/attack with armies from source to target
class Advance : public Order {
public:
    Advance();  // Default constructor for Assignment 1 compatibility
    Advance(Player* player, Territory* source, Territory* target, int numArmies);
    Advance(const Advance& other);
    ~Advance();
    Advance& operator=(const Advance& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    
private:
    Player* player;
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int* armiesToMove;
    
    void simulateBattle(int attackers, int defenders);
};

// Bomb order: destroy half armies on enemy adjacent territory
class Bomb : public Order {
public:
    Bomb();  // Default constructor for Assignment 1 compatibility
    Bomb(Player* player, Territory* target);
    Bomb(const Bomb& other);
    ~Bomb();
    Bomb& operator=(const Bomb& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    
private:
    Player* player;
    Territory* targetTerritory;
};

// Blockade order: double armies and transfer to Neutral player
class Blockade : public Order {
public:
    Blockade();  // Default constructor for Assignment 1 compatibility 
    Blockade(Player* player, Territory* target, Player* neutral);
    Blockade(const Blockade& other);
    ~Blockade();
    Blockade& operator=(const Blockade& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    
private:
    Player* player;
    Territory* targetTerritory;
    Player* neutralPlayer;  // Reference to Neutral player
};

// Airlift order: move armies between any owned territories
class Airlift : public Order {
public:
    Airlift();  // Default constructor for Assignment 1 compatibility 
    Airlift(Player* player, Territory* source, Territory* target, int numArmies);
    Airlift(const Airlift& other);
    ~Airlift();
    Airlift& operator=(const Airlift& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    
private:
    Player* player;
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int* armiesToMove;
};

// Negotiate order: prevent attacks between two players for remainder of turn
class Negotiate : public Order {
public:
    Negotiate();  // Default constructor for Assignment 1 compatibility 
    Negotiate(Player* player, Player* targetPlayer);
    Negotiate(const Negotiate& other);
    ~Negotiate();
    Negotiate& operator=(const Negotiate& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
    
private:
    Player* player;
    Player* targetPlayer;
};

// -------- ORDERSLIST CLASS --------
// Manages a collection of orders with operations to manipulate the list
class OrdersList : public Subject, public ILoggable {
public:
	// Constructors and destructor
	OrdersList();
	OrdersList(const OrdersList& other);
	~OrdersList();

	// Assignment operator
	OrdersList& operator=(const OrdersList& other);

	// Methods
	void add(Order* order);                    // Add order to end of list
	void remove(int index);                     // Remove order at index
	void move(int fromIndex, int toIndex);      // Move order from one position to another

	// logging function
	std::string stringToLog() const override;

	// Getters
	int size() const;
	Order* getOrder(int index) const;

	// Stream insertion operator
	friend std::ostream& operator<<(std::ostream& os, const OrdersList& list);

private:
    std::vector<Order*>* orders;
};

#endif // ORDERS_H