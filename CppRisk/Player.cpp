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
    territories = new std::vector<TerritoriesWithArmies*>(); // initialized with null pointers. 
    playerHand = new Hand();
    orderslist = new OrdersList();
}

Player::Player(const std::string& color, const std::vector<TerritoriesWithArmies*>& initialTerritories) {
    Player::playerCount++;
    playerID = new int(playerCount);
    playerColor = new std::string(color);
    territories = new std::vector<TerritoriesWithArmies*>(); // initialized with null pointers. 
    // You want the pointers of Player to point to the same territories as map territories. 
 
    for (TerritoriesWithArmies* twa : initialTerritories) {
        TerritoriesWithArmies* newTWA = new TerritoriesWithArmies;
        (*newTWA).armies = (*twa).armies;
        (*newTWA).territory = (*twa).territory;
        (*territories).push_back(newTWA);
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

       this->territories = new std::vector<TerritoriesWithArmies*>();
       // For every pointer to the struct containing territories and armies, do deep copies of territories and armies.
        for (TerritoriesWithArmies* territoriesWithArmies_ptr : *(other.territories)) {
            TerritoriesWithArmies* newTWA = new TerritoriesWithArmies;
            (*newTWA).armies = (*territoriesWithArmies_ptr).armies;
            (*newTWA).territory = new Territory(*((*territoriesWithArmies_ptr).territory));
            (*territories).push_back(newTWA);
        }
	}   

//Destructor
Player::~Player() {
        delete playerID;
        delete playerColor;
        delete playerHand;
        delete orderslist;

       // Delete all territoriesWithArmies and their territory
        for (TerritoriesWithArmies* twa : *territories) {
            delete (*twa).territory;
            delete twa;
        }
        delete territories;
        
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
        return playerHand;
    }

OrdersList* Player::getOrdersList() const {
        return orderslist;
    }
//setter
void Player::setColor(const std::string& color) {
		delete this->playerColor; 
        this->playerColor = new std::string(color);
    }
    
 
//Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& player)
    {
        os << "\nPlayer ID : " << *(player.playerID)
            << "\nPlayer Color : " << *(player.playerColor)
            << "\nPlayer Territories: " << std::endl;
        const std::vector<TerritoriesWithArmies*>* twaVector = player.territories;
       if (twaVector != nullptr) {
        for (int i = 0; i < (*twaVector).size(); i++) {
            TerritoriesWithArmies& twa = *((*twaVector)[i]);
            if ( twa.territory != nullptr) {
                os << (*twa.territory).getName()  << std::endl;
            }
        }
    }
        return os;
    }
    

//Assignment Operator
Player& Player::operator= (const Player& otherPlayer) {
        if (this == &otherPlayer) { return *this; }

    delete playerID;
    delete playerColor;
	delete playerHand; //calling Hand destructor
    delete orderslist; //calling Orderlist destructor
    delete territories;  //calling Territories destructor

    this->playerID = new int(*(otherPlayer.playerID));
    this->playerColor = new std::string(*(otherPlayer.playerColor));
    this->playerHand = new Hand(*(otherPlayer.playerHand)); //calling Hand copy constructor
    this->orderslist = new OrdersList(*(otherPlayer.orderslist)); //calling orderList copy constructor

    this->territories = new std::vector<TerritoriesWithArmies*>(); // default constructor assigns null pointers. 
    // For every pointer to the struct containing territories and armies, do deep copies of territories and armies.
    for (TerritoriesWithArmies* territoriesWithArmies_ptr : *(otherPlayer.territories)) {
        TerritoriesWithArmies* newTWA = new TerritoriesWithArmies;
        (*newTWA).armies = (*territoriesWithArmies_ptr).armies;
        (*newTWA).territory = new Territory(*((*territoriesWithArmies_ptr).territory)); 
        (*territories).push_back(newTWA);
    }
    
    }
    
// ------------------ ToDefend() ------------------
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
    for (TerritoriesWithArmies* twa : *territories) {  
        Territory* playerTerritory = (*twa).territory;
        std::vector<Territory*> adjacentTerritories = playerTerritory->getAdjacentTerritories();
       	
		//Create the collection of possible territories to attack
        for (Territory* adjTerritory : adjacentTerritories) {
            if (std::find(possibleTerritories.begin(), possibleTerritories.end(), adjTerritory) == possibleTerritories.end()) {
                possibleTerritories.push_back(adjTerritory); 
            }
        }
	}
	//Build the collection of randomly chosen territories to attack
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


// ------------------ issueOrder() ------------------
	// This method allows the user to issue an order . it adds the order to the player's order list
    // Deploy and advance are always available as orders
	// Players can also issue orders based on the cards they have in their hand
void Player::issueOrder() {

        std::cout << "Here are the possible orders" << std::endl;
        std::cout << "1. Deploy" << std::endl;
        std::cout << "2. Advance" << std::endl;
        std::cout << "From the cards available in your hand" << std::endl;
        int i = 3;
        for (Cards* c : (*((*playerHand).hand))) {
            
            std::cout << i << "  " << (*c).getName() << std::endl;
            i++;
        }
        bool validInput = false;
        int choice;
        while (!validInput)
        {
            std::cout << "Please chose an order number" << std::endl;
            
            std::cin >> choice;
            if (choice < 1 || choice > 2 + (*(*playerHand).hand).size()) {
                std::cout << "Invalid choice. Please try again." << std::endl;
            };
            validInput = true;
        }

        if(choice == 1){
            std::cout << " Creationg a Deploy order and adding it to the orders list" << std::endl;
            // Create a deploy order and add it to the player's list of orders
            Order* deployOrder = new Deploy();
            (*orderslist).add(deployOrder);
           
        }
        else if(choice == 2 ){
            std::cout << " Creationg a Advance order and adding it to the orders list" << std::endl;
            // Create a advance order and add it to the player's list of orders
            Order* advanceOrder = new Advance();
            (*orderslist).add(advanceOrder);
            
        }
			
        else {
            std::string cardName = (*(*(*playerHand).hand)[choice-3]).getName();
            std::cout << " Creationg an " << cardName  << " order and adding it to the orders list" << std::endl;
            // the card pay() returns an Order
            Order* cardOrder = (*((*(*playerHand).hand)[choice-3])).play(cardName);
			// add the order to the player's list of orders 
			(*orderslist).add(cardOrder);
        }          
    }

        






