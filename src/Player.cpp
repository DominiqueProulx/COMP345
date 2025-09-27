// -------------------------------
// COMP 345 - Fall 2025
// Dominique Proulx - 40177566
// File: Player.cpp
// -------------------------------

//Implementation for Player class

//TODO: Impletement toDefend(), toAttack(), issueOrder() methods
//TODO : Implement stream insertion operator
//TODO : Implement assignation operator
//TODO: Implement destructor
//TODO : Implement player Driver
//TODO : Write meaningful comments and cleanup any flags.

// Headers
#include "Player.h"
#include "DummyCards.h"
#include "DummyHand.h"
#include "DummyMap.h"
#include "DummyOrders.h"
#include "DummyOrderList.h"
#include "DummyTerritory.h"
#include <stdio.h>
#include <list>
#include <string>




//Setup the Dummy classes 
using DummyMap as Map; // Might not be useful since player has access to adjacent terriritories through its own territories 
using DummyTerritory as Territory;
using DummyHand as Hand;
using DummyOrders as Orders;
using DummyOrderList as OrderList;
using DummyCards as Cards;

//Struct  definintion to hold a pointer to a territory and the number of armies on it
struct TerritoriesWithArmies {
    Territory* territory;
    int armies;
};

// Define a class
//All data members of user-defined class type must be of pointer type
//All classes must implement a correct copy constructor, assignment operator, and stream insertion operator.

class Player {
private:
    // has a playerID 
    const int* playerID;
	const std::string* playerColor;
	
	//A list of territories owned by the player, each has a number of armies on it.  The list constains pointers to the TerritoriesWithArmies struct
    std::list<TerritoriesWithArmies*>* territories;
    // has a hand
	Hand* hand;
	// has a list of orders
	OrderList* orders;

public:
    static int playerCount;
    // Constructor
    // should accept a list of territories
    Player(int& id, std::string& color, std::list<TerritoriesWithArmies>& initialTerritories) 
		: playerID(id), playerColor(color), territories(initialTerritories) {
        hand = new Hand();
        orders = new OrderList();
    }
    // Copy constructor
	Player(const Player& other) {
       this->playerID = new int(*(other.playerID));
       this->playerColor = new std::string(*(other.playerColor));
       this->hand = new Hand(*(other.hand));
       this->orders = new OrderList(*(other.orders));

       this->territories = new std::list<TerritoriesWithArmies*>();
       // For every pointer to the strcut containing territories and armies, do a deep copies of territories and armies.
        for (auto territoriesWithArmies_ptr : *(other.territories)) {
            TerritoriesWithArmies* newTWA = new TerritoriesWithArmies;
            (*newTWA).armies = (*territoriesWithArmies_ptr).armies;
            (*newTWA).territory = new Territory(*((*territoriesWithArmies_ptr).territory)); // deep copy of the territory as well
            (*territories).push_back(newTWA);
        }
	}   
   // Methods
    //toDefend();
    
    //toAttack();
    
	//issueOrder();
        // To create an order, you must play a card
		// To play a card , it must ne in your hand
        // to have a card in your hand , you must draw it from the deck.
    //to issue order
         // deploy and advance are always available
		 // Check hand for airlift, blockade, bomb, negotiate cards
             // If they are in hand , offer the order
		     //  If order form a card is chosen , Hand.Card.play()  -- it will return and order and remove the card from hand
				 // Add the order to the player's list of orders
			 //  if deploy or advance is chosen , create the order and add it to the player's list of orders
};


int Player::playerCount = 0; // Initialize static member



