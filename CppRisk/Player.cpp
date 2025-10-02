// -------------------------------
// COMP 345 - Fall 2025
// Dominique Proulx - 40177566
// File: Player.cpp
// -------------------------------

//Implementation for Player class

//Change territories to a vector.
//TODO: Impletement toDefend(), toAttack(), issueOrder() methods
//TODO : Implement stream insertion operator
//TODO : Implement assignation operator
//TODO : Implement player Driver
//TODO : Write meaningful comments and cleanup any flags.
//TODO: Check that the constructor is okay and accepts a list of territories
//TODO overload tostring 

// Headers
#include "Player.h"
#include <iostream>
#include <cstdlib>

// Initialize static member
int Player::playerCount = 0; 


// Define a class
//All data members of user-defined class type must be of pointer type
//All classes must implement a correct copy constructor, assignment operator, and stream insertion operator.






    // Constructor
    // should accept a list of territories
    
    Player::Player(const std::string& color, const std::vector<TerritoriesWithArmies*>& initialTerritories)  {
        Player::playerCount++;
        this->playerID = new int(playerCount);
        this->playerColor = new std::string(color);
        this->territories = new std::vector<TerritoriesWithArmies*>();
		// Copy every pointer to the struct containing territories and armies, do a deep copies of territories too.
		// This is assuming that territories do not have a copy constructor defined.
        for (TerritoriesWithArmies* twa : initialTerritories) {
            TerritoriesWithArmies* newTWA = new TerritoriesWithArmies;
            (*newTWA).armies = (*twa).armies;
            (*newTWA).territory = new Territory(*((*twa).territory));
			(*territories).push_back(newTWA);
        }

        hand = new Hand();
        orders = new OrderList();
		
    }
    // Copy constructor
	Player::Player(const Player& other) {
       this->playerID = new int(*(other.playerID));
       this->playerColor = new std::string(*(other.playerColor));
       this->hand = new Hand(*(other.hand));
       this->orders = new OrderList(*(other.orders));

       this->territories = new std::vector<TerritoriesWithArmies*>();
       // For every pointer to the strcut containing territories and armies, do a deep copies of territories and armies.
        for (TerritoriesWithArmies* territoriesWithArmies_ptr : *(other.territories)) {
            TerritoriesWithArmies* newTWA = new TerritoriesWithArmies;
            (*newTWA).armies = (*territoriesWithArmies_ptr).armies;
            (*newTWA).territory = new Territory(*((*territoriesWithArmies_ptr).territory)); // deep copy of the territory as well
            (*territories).push_back(newTWA);
        }
	}   

    //Destructor
    Player::~Player() {
        delete playerID;
        delete playerColor;
        delete hand;
        delete orders;

        // Delete all territoriesWithArmies and their territory
        for (TerritoriesWithArmies* twa : *territories) {
            delete (*twa).territory;
            delete twa;
        }
        delete territories;

        playerCount--;
    }

    //getters
    const int* Player::getID() const {
        return playerID;
    }

    const std::string* Player::getColor() const {
        return playerColor;
    }

    const std::vector<TerritoriesWithArmies*>* Player::getTerritories() const {
        return territories;
    }

    Hand* Player::getHand() const {
        return hand;
    }

    OrderList* Player::getOrders() const {
        return orders;
    }
    // setters
    void Player::setColor(const std::string& color) {
		delete this->playerColor; 
        this->playerColor = new std::string(color);
    }
    
   
    
    
   // Methods
    
    
    //toDefend();
        // Returns a vector of pointers to TerritoriesWithArmies struct
        // The territories returned are a subset of the player's territories
		// The vector can be empty if the player has no territories -- But that would mean that the player has lost the game anyway
        // The vector can contain all the player's territories
        // The number of territories to defend is randomly chosen between 0 and the number of territories the player owns
        // The territories to defend are randomly chosen from the player's territories
    std::vector<TerritoriesWithArmies*> Player::toDefend(){
     int numberOfTerritories = (*territories).size();
     std::vector<TerritoriesWithArmies*> territoriesToDefend = std::vector<TerritoriesWithArmies*>();
     std::vector<int> territoriesAdded; // to keep track of already added territories

     if(numberOfTerritories == 0) {
         std::cout << "Player has no territories to defend." << std::endl;
		 return territoriesToDefend; //return the empty vector
	 }
	 //using the random generator to select how many territories will be added. (from 0 to numberOfTerritories -1)
     int randomNumberOfTerritories = std::rand() % numberOfTerritories; 
 
     //assign the territories that get added to the vecor randomly
     while (territoriesToDefend.size() < randomNumberOfTerritories) {
         int randomTerritory = std::rand() % numberOfTerritories;

		 // Check that the territory was not already added
         if (std::find(territoriesAdded.begin(), territoriesAdded.end(), randomTerritory) == territoriesAdded.end()) {
             territoriesAdded.push_back(randomTerritory);

             TerritoriesWithArmies* twaToAdd = new TerritoriesWithArmies;
             (*twaToAdd).armies = (*((*territories)[randomTerritory])).armies;
             (*twaToAdd).territory = new Territory(*((*((*territories)[randomTerritory])).territory));
             territoriesToDefend.push_back(twaToAdd);
         }
     }
	 return territoriesToDefend;
    }
    
    //ToAttack();
        // Returns a vector of pointers to Territories struct
		// The territories are randomly chosen from the vector of territories passed as argument
        // The argument  serves as a stub to model what the player could get from the map
        // The vector can be empty if the player has no territories to attack
        // The vector can contain all the territories passed as argument
		// The number of territories to attack is randomly chosen between 0 and the number of territories passed as argument
    std::vector<Territory*> Player::toAttack(const std::vector<Territory*>& possibleTerritories) {
        int numberOfPossibleTerritories = possibleTerritories.size();

        std::vector<Territory*> territoriesToAttack;
        std::vector<int> territoriesAdded; // to keep track of already added territories

        if (numberOfPossibleTerritories == 0) {
            std::cout << "Player has no territories to attack." << std::endl;
			return territoriesToAttack; //return the empty vector
        }

        //using the random generator to select how many territories will be added. (from 0 to numberOfTerritories -1)
        int randomNumberOfTerritories = std::rand() % numberOfPossibleTerritories;


        //assign the territories that get added to the vecor randomly
        while (territoriesToAttack.size() < randomNumberOfTerritories) {
            int randomTerritory = std::rand() % numberOfPossibleTerritories;

            // Check that the territory was not already added
            if (std::find(territoriesAdded.begin(), territoriesAdded.end(), randomTerritory) == territoriesAdded.end()) {
                territoriesAdded.push_back(randomTerritory);
                
                Territory* territoryToAdd = new Territory(*possibleTerritories[randomTerritory]);

                
                territoriesToAttack.push_back(territoryToAdd);
            }
        }
        return territoriesToAttack;
    }

    

    
	//issueOrder();
        // To create an order, you must play a card
		// To play a card , it must ne in your hand
        // to have a card in your hand , you must draw it from the deck.
    //to issue order
         // deploy and advance are always available
		 // Check hand for airlift, blockade, bomb, negotiate cards ( Hand.getcard.type)
             // If they are in hand , offer the order 
		     //  If order form a card is chosen , Hand.getCard.play()  -- it will return and order and remove the card from hand
				 // Add the order to the player's list of orders
			 //  if deploy or advance is chosen , create the order and add it to the player's list of orders






