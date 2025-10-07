// -------------------------------
// COMP 345 - Fall 2025
// Risk Game Project
// Part 2 : Player 
// File: Player.cpp
// -------------------------------

//Implementation for Player class

// Headers
#include "Player.h"
#include <iostream>
#include <cstdlib>

// Initialize static member
int Player::playerCount = 0; 

 // Constructors 
Player::Player() {
    Player::playerCount++;
    playerID = new int(playerCount);
    playerColor = new std::string("NoColor");
 	territoriesOwned = new std::vector<Territory*>(); // initialized with null pointers.
    playerHand = new Hand();
    orderslist = new OrdersList();
	deck = new Deck();
}

Player::Player(const std::string& color, const std::vector<Territory*>& initialTerritories,Deck* deck) {
    Player::playerCount++;
    this->deck = deck; //shared amongs all players.
    playerID = new int(playerCount);
    playerColor = new std::string(color);
	territoriesOwned = new std::vector<Territory*>(); // initialized with null pointers.
   // You want the pointers of Player's territories to point to the same territories as map. 
    for (Territory* t : initialTerritories) {
		(*territoriesOwned).push_back(t);
    }
    playerHand = new Hand();
    orderslist = new OrdersList();
}

// Copy constructor
Player::Player(const Player& other) {
       this->playerID = new int(*(other.playerID));
       this->playerColor = new std::string(*(other.playerColor));
       this->playerHand = new Hand(*(other.playerHand)); //Calling Hand copy constrcutor
	   this->orderslist = new OrdersList(*(other.orderslist)); //Calling OrderList copy constrcutor
       this->deck = other.deck; // all players must play with the same deck of cards.
	   this->territoriesOwned = new std::vector<Territory*>();
       for (Territory* t : *(other.territoriesOwned)) {
		   territoriesOwned->push_back(t);
       }
	}   

//Destructor
Player::~Player() {
        delete playerID;
        playerID = nullptr;
        delete playerColor;
        playerColor = nullptr;
        delete playerHand;
        playerHand = nullptr;
        delete orderslist;
        orderslist = nullptr;
        delete territoriesOwned;
        territoriesOwned = nullptr;
    }

//getters
const int* Player::getID() const {
        return playerID;
    }

const std::string* Player::getColor() const {
        return playerColor;
    }

const std::vector<Territory*>* Player::getTerritories() const {
        return territoriesOwned;
    }

Hand* Player::getHand() const {
        return playerHand;
    }

OrdersList* Player::getOrdersList() const {
        return orderslist;
    }
//setters
void Player::setColor(const std::string& color) {
		delete this->playerColor; 
        this->playerColor = new std::string(color);
    }
void Player::setHand(Hand& hand) {
    delete this->playerHand;
	this->playerHand = &hand;
}
 
//Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& player)
    {
        os << "\nPlayer ID : " << *(player.playerID)
            << "\nPlayer Color : " << *(player.playerColor)
            << "\nPlayer Territories: " << std::endl;

        for (Territory* t : *(player.territoriesOwned)) {
            std::cout << *t << std::endl;
        }
        return os;
    }
    

//Assignment Operator
Player& Player::operator= (const Player& otherPlayer) {
        if (this == &otherPlayer) { return *this; }

    delete playerID;
    delete playerColor;
	delete playerHand; 
    delete orderslist; 
    delete territoriesOwned; 

    this->playerID = new int(*(otherPlayer.playerID));
    this->playerColor = new std::string(*(otherPlayer.playerColor));
    this->playerHand = new Hand(*(otherPlayer.playerHand)); 
    this->orderslist = new OrdersList(*(otherPlayer.orderslist));
    this->territoriesOwned = new std::vector<Territory*>();
	for (Territory* t : *(otherPlayer.territoriesOwned)) { //Shallow copies since territories are owned by map
        territoriesOwned->push_back(t);
    }
	this->deck = otherPlayer.deck; // all players must play with the same deck of cards.
    
    return *this;
}
    
// ------------------ ToDefend() ------------------
//toDefend();
 // Returns a vector of pointers to TerritoriesWithArmies struct
 // The territories returned are a subset of the player's territories
 // The vector can be empty if the player has no territories -- But that would mean that the player has lost the game anyway
 // The vector can contain all the player's territories
 // The number of territories to defend is randomly chosen between 0 and the number of territories the player owns
 // The territories to defend are randomly chosen from the player's territories
std::vector<Territory*> Player::toDefend(){
     int numberOfTerritories = (*territoriesOwned).size();
     std::vector<Territory*> territoriesToDefend;

     if(numberOfTerritories == 0) {
         std::cout << "Player has no territories to defend." << std::endl;
		 return territoriesToDefend; //return the empty vector
	 }
	 //using the random generator to select how many territories will be added. (from 0 to numberOfTerritories -1)
     int randomNumberOfTerritories = std::rand() % numberOfTerritories; 
 
     //assign the territories that get added to the vector randomly
     while (territoriesToDefend.size() < randomNumberOfTerritories) {
         int randomTerritoryIndex = std::rand() % numberOfTerritories;
         Territory* territoryToAdd = territoriesOwned->at(randomTerritoryIndex);
         if (std::find(territoriesToDefend.begin(), territoriesToDefend.end(), territoryToAdd) == territoriesToDefend.end()) {
             territoriesToDefend.push_back(territoryToAdd);
         }

		
     }
	 return territoriesToDefend;
    }
    
// ------------------ ToAttack() TO KEEP AFTER MERGING------------------
//ToAttack();
    // Returns a vector of pointers to a vector containing Territories pointers
    // Creates a collection of territories from adjacent territories to the player's territories
    // The territories are randomly chosen from this collection
    // The vector can be empty if the player has no territories to attack
    // The vector can contain all the territories passed as argument
	// The number of territories to attack is randomly chosen between 0 and the number of territories that could be attacked
std::vector<Territory*> Player::toAttack() {
    std::vector<Territory*> possibleTerritories;

	//construct the vector of possible territories to attack
      for (Territory* t : *territoriesOwned) {
            std::vector<Territory*> adjacentTerritories = t->getAdjacentTerritories();
			//Add adjacent territories to the possible territories vector if not already present
        for (Territory* adjTerritory : adjacentTerritories) {
            if (std::find(possibleTerritories.begin(), possibleTerritories.end(), adjTerritory) == possibleTerritories.end()) {
                possibleTerritories.push_back(adjTerritory); 
            }
        }
	}
	//Build the collection of randomly chosen territories to attack
    int numberOfPossibleTerritories = possibleTerritories.size();
    std::vector<Territory*> territoriesToAttack;

    if (numberOfPossibleTerritories == 0) {
        std::cout << "Player has no territories to attack." << std::endl;
        return territoriesToAttack; //return the empty vector
    }

    //using the random generator to select how many territories will be added. (from 0 to numberOfTerritories -1)
    int randomNumberOfTerritories = std::rand() % numberOfPossibleTerritories;

    //assign the territories that get added to the vector randomly
    while (territoriesToAttack.size() < randomNumberOfTerritories) {

        int randomTerritoryIndex = std::rand() % numberOfPossibleTerritories;
        // Check that the territory was not already added, if not, add it
        Territory* territoryToAdd = possibleTerritories.at(randomTerritoryIndex);
        if (std::find(territoriesToAttack.begin(), territoriesToAttack.end(), territoryToAdd) == territoriesToAttack.end()) {
          
            territoriesToAttack.push_back(territoryToAdd);
        }
    }
    return territoriesToAttack;
}


// ------------------ issueOrder() ------------------
	// This method allows the user to issue an order . it adds the order to the player's order list
    // Deploy and advance are always available as orders
	// Players can also issue orders based on the cards they have in their hand
void Player::issueOrder() {

        std::cout << "\nHere are the possible orders" << std::endl;
        std::cout << "1. Deploy" << std::endl;
        std::cout << "2. Advance" << std::endl;
        std::cout << "From the cards available in your hand : \n" << std::endl;
        

        for (int i = 0; i < (*playerHand).getSize(); i++) {
            std::cout << (i+3) <<". " << playerHand->getCard(i)->getName() << std::endl;
        }
       
       
        bool validInput = false;
        int choice;
        while (!validInput)
        {
            std::cout << "Please chose an order number" << std::endl;
            
            std::cin >> choice;
            if (choice < 1 || choice > 2 + (*playerHand).getSize()) {
                std::cout << "Invalid choice. Please try again." << std::endl;
            };
            validInput = true;
        }

        if(choice == 1){
            std::cout << " Creating a Deploy order and adding it to the orders list" << std::endl;
            // Create a deploy order and add it to the player's list of orders
            Order* deployOrder = new Deploy();
            (*orderslist).add(deployOrder);
           
        }
        else if(choice == 2 ){
            std::cout << " Creating a Advance order and adding it to the orders list" << std::endl;
            // Create a advance order and add it to the player's list of orders
            Order* advanceOrder = new Advance();
            (*orderslist).add(advanceOrder);
            
        }
			
        else {
            std::string cardName = playerHand->getCard(choice - 3)->getName();
            std::cout << " Creating an " << cardName  << " order and adding it to the orders list" << std::endl;
            // the card pay() returns an Order
            Order* cardOrder = playerHand->getCard(choice - 3)->play(*playerHand, *deck);
			// add the order to the player's list of orders 
			(*orderslist).add(cardOrder);
        }          
    }

        






