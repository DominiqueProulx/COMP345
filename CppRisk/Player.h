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
#include <set>
#include "Map.h"
#include "Cards.h"

// Forward declarations to break circular dependency
class OrdersList;
class Order;


// ------------------ CLASS PLAYER ------------------

class Player {
public:
    static int playerCount;

    
    enum class OrderType {
        ADVANCE,
        DEPLOY,
        BOMB,
        BLOCKADE,
        AIRLIFT,
        NEGOTIATE
    };

    // Constructor
    Player();
    Player(const std::string& color, const std::vector<Territory*>& initialTerritories, Deck* deck);
    // Copy Constructor
    Player(const Player& other); 

    // Destructor 
    ~Player();

    // Getters and setters
    const int* getID() const;
    const std::string* getColor() const;
    const std::vector<Territory*>* getTerritories() const;
    Hand* getHand() const;
    OrdersList* getOrdersList() const;
    void setPendingDeployments(int armies);

    void setColor(const std::string& color);
    void setHand(Hand& hand);

    // Assignment 2 additions for order execution
    bool ownsTerritory(Territory* territory) const;
    void addTerritory(Territory* territory);
    void removeTerritory(Territory* territory);
    int getReinforcementPool() const;
    void setReinforcementPool(int armies);
	void addToReinforcementPool(int armies);
    bool hasConqueredThisTurn() const;
    void setConqueredThisTurn(bool conquered);
    void addNegotiatedPlayer(Player* player);
    void clearNegotiations();
    bool hasNegotiationWith(Player* player) const;
    std::string getName() const;
	void drawBackUpCards();

    Order* getNextOrderToExecute();
    void removeExecutedOrder();
    void resetDefendAndAttack();
    Territory* choseFromToDefend();
    Territory* choseFromToAttack();
    Territory* choseFromAdjacent(Territory* ownTerritory , const std::string& option);
    Order* issueAdvanceOrder();
    Order* issueDeployOrder();
    Order* issueBombOrder();
    Order* issueBlockadeOrder();
    Order* issueAirliftOrder();
    Order* issueNegotiateOrder();
    Order* orderFactory(
        Player::OrderType type,
		Territory* sourceTerritory = nullptr,
		Territory* targetTerritory = nullptr,
		Player* targetOrNeutral = nullptr,
		int armies = 0
    );


    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

	// Equals operator
    bool operator==(const Player& other) const;
    bool operator!=(const Player& other) const;
    // Assignment Operator
    Player& operator= (const Player& otherplayer);

    // Methods
    std::vector<Territory*>* toDefend();
    std::vector<Territory*>* toAttack();
    void issueOrder();

  

private:
    const int* playerID;
    const std::string* playerColor;
    std::vector<Territory*>* territoriesOwned;
    Hand* playerHand;
    OrdersList* orderslist;
    Deck* deck;

    
    // Assignment 2 additions
    int* reinforcementPool;
    int* pendingDeployments;
    bool* conqueredTerritoryThisTurn;
    std::set<Player*>* negotiatedPlayers;
    std::vector<Territory*>* territoriesToDefend;
    std::vector<Territory*>* territoriesToAttack;

	

};

#endif