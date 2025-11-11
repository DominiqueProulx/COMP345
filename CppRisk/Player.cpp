// -------------------------------
// COMP 345 - Fall 2025
// Risk Game Project
// Part 2 : Player 
// File: Player.cpp
// -------------------------------

#include "Player.h"
#include "Orders.h"  
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstddef>

// Initialize static member
int Player::playerCount = 0; 

// Constructors 
Player::Player() {
    Player::playerCount++;
    playerID = new int(playerCount);
    playerColor = new std::string("NoColor");
    territoriesOwned = new std::vector<Territory*>();
    playerHand = new Hand();
    orderslist = new OrdersList();
    deck = new Deck();
    
    // Assignment 2 additions
    reinforcementPool = new int(0);
    pendingDeployments = new int(0);
    conqueredTerritoryThisTurn = new bool(false);
    negotiatedPlayers = new std::set<Player*>();
    territoriesToDefend = nullptr;
    territoriesToAttack = nullptr;
}

Player::Player(const std::string& color, const std::vector<Territory*>& initialTerritories, Deck* deck) {
    Player::playerCount++;
    this->deck = deck;
    playerID = new int(playerCount);
    playerColor = new std::string(color);
    territoriesOwned = new std::vector<Territory*>();
    
    for (Territory* t : initialTerritories) {
        (*territoriesOwned).push_back(t);
    }
    
    playerHand = new Hand();
    orderslist = new OrdersList();
    
    // Assignment 2 additions
    reinforcementPool = new int(0);
    pendingDeployments = new int(0);
    conqueredTerritoryThisTurn = new bool(false);
    negotiatedPlayers = new std::set<Player*>();
    territoriesToDefend = nullptr;
    territoriesToAttack = nullptr;
}

// Copy constructor
Player::Player(const Player& other) {
    this->playerID = new int(*(other.playerID));
    this->playerColor = new std::string(*(other.playerColor));
    this->playerHand = new Hand(*(other.playerHand));
    this->orderslist = new OrdersList(*(other.orderslist));
    this->deck = other.deck;
    this->territoriesOwned = new std::vector<Territory*>();
    
    for (Territory* t : *(other.territoriesOwned)) {
        territoriesOwned->push_back(t);
    }
    
    // Assignment 2 additions
    this->reinforcementPool = new int(*other.reinforcementPool);
    this->pendingDeployments = new int(*other.pendingDeployments); 
    this->conqueredTerritoryThisTurn = new bool(*other.conqueredTerritoryThisTurn);
    this->negotiatedPlayers = new std::set<Player*>(*other.negotiatedPlayers);
}

// Destructor
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
    
    // Assignment 2 additions
    delete reinforcementPool;
    delete pendingDeployments;
    delete conqueredTerritoryThisTurn;
    delete negotiatedPlayers;
    delete territoriesToDefend;
    delete territoriesToAttack;
}

// Getters
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
int Player::getPendingDeployments() const {
    return *pendingDeployments;
}


int Player::getReinforcementPool() const {
    return *reinforcementPool;
}



// Setters
void Player::setColor(const std::string& color) {
    delete this->playerColor; 
    this->playerColor = new std::string(color);
}

void Player::setHand(Hand& hand) {
    delete this->playerHand;
    this->playerHand = &hand;
}

void Player::setPendingDeployments(int armies) {
    *pendingDeployments = armies;
}

void Player::addPendingDeployment(int armies) {
    *pendingDeployments += armies;
}

void Player::clearPendingDeployments() {
    *pendingDeployments = 0;
}
void Player::setReinforcementPool(int armies) {
    *reinforcementPool = armies;
}
void Player::setConqueredThisTurn(bool conquered) {
    *conqueredTerritoryThisTurn = conquered;
}

// Assignment 2 methods

void Player::addToReinforcementPool(int armies) {
    *reinforcementPool += armies;
}
bool Player::ownsTerritory(Territory* territory) const {
    for (Territory* t : *territoriesOwned) {
        if (t == territory) {
            return true;
        }
    }
    return false;
}

void Player::addTerritory(Territory* territory) {
    if (!ownsTerritory(territory)) {
        territoriesOwned->push_back(territory);
        territory->setOwner(this);
    }
}

void Player::removeTerritory(Territory* territory) {
    auto it = std::find(territoriesOwned->begin(), territoriesOwned->end(), territory);
    if (it != territoriesOwned->end()) {
        territoriesOwned->erase(it);
    }
}

bool Player::hasConqueredThisTurn() const {
    return *conqueredTerritoryThisTurn;
}


void Player::removeExecutedOrder() {
    orderslist->remove(0);

}



void Player::addNegotiatedPlayer(Player* player) {
    if (player && player != this) {
        negotiatedPlayers->insert(player);
    }
}

void Player::clearNegotiations() {
    negotiatedPlayers->clear();
}

bool Player::hasNegotiationWith(Player* player) const {
    return negotiatedPlayers->find(player) != negotiatedPlayers->end();
}

std::string Player::getName() const {
    return *playerColor;  // Using color as name for now
}
// equals overloading
bool Player::operator==(const Player& other) const {
    return playerID == other.playerID;
}
bool Player::operator!=(const Player& other) const {
    return !(*this == other);
}


// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "\nPlayer ID : " << *(player.playerID)
       << "\nPlayer Color : " << *(player.playerColor)
       << "\nPlayer Territories: " << std::endl;

    for (Territory* t : *(player.territoriesOwned)) {
        std::cout << *t << std::endl;
    }
    return os;
}

// Assignment Operator
Player& Player::operator=(const Player& otherPlayer) {
    if (this == &otherPlayer) { return *this; }

    delete playerID;
    delete playerColor;
    delete playerHand; 
    delete orderslist; 
    delete territoriesOwned;
    delete reinforcementPool;
    delete pendingDeployments;
    delete conqueredTerritoryThisTurn;
    delete negotiatedPlayers;

    this->playerID = new int(*(otherPlayer.playerID));
    this->playerColor = new std::string(*(otherPlayer.playerColor));
    this->playerHand = new Hand(*(otherPlayer.playerHand)); 
    this->orderslist = new OrdersList(*(otherPlayer.orderslist));
    this->territoriesOwned = new std::vector<Territory*>();
    
    for (Territory* t : *(otherPlayer.territoriesOwned)) {
        territoriesOwned->push_back(t);
    }
    
    this->deck = otherPlayer.deck;
    
    // Assignment 2 additions
    this->reinforcementPool = new int(*otherPlayer.reinforcementPool);
    this->pendingDeployments = new int(*otherPlayer.pendingDeployments);
    this->conqueredTerritoryThisTurn = new bool(*otherPlayer.conqueredTerritoryThisTurn);
    this->negotiatedPlayers = new std::set<Player*>(*otherPlayer.negotiatedPlayers);
    
    return *this;
}


// toDefend()
// Player selects the territories they wish to defend this turn, in priority order.
// Returns a vector of Territory pointers representing the defend list.
std::vector<Territory*>* Player::toDefend() {
    int numberOfTerritories = (*territoriesOwned).size();

    // If player has no territories, allow to exit
    if (numberOfTerritories == 0) {
        std::cout << "You currently have no territories to defend. Returning to main menu..." << std::endl;
        return nullptr; 
    }

	std::vector<Territory*>* territoriesToDefendThisturn = new std::vector<Territory*>();// Create a new vector to hold territories to defend
    bool doneAdding = false;

    std::cout << "\nPlease add territories to defend in priority order." << std::endl;
    std::cout << "(Enter 0 to finish or -1 to cancel)\n" << std::endl;

    // Display owned territories
    for (int i = 0; i < numberOfTerritories; i++) {
        std::cout << i + 1 << ". " << (*territoriesOwned)[i]->getName() << " - " << (*territoriesOwned)[i]->getNumberOfArmies() << std::endl;
    }

    //Capture user input
    while (!doneAdding) {
        std::cout << "\nEnter the number of the territory to add to the defend list: ";
        int choice;
        std::cin >> choice;

		// Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
		//returns the empty list if user cancels, allows a way out if there are no territories to defend. 
        if (choice == -1) {
            std::cout << "You cancelled territory selection. Returning..." << std::endl;
            return territoriesToDefendThisturn;
           
        }
        //marks the end of the selection
        if (choice == 0) {
            if (territoriesToDefendThisturn->empty()) {
                std::cout << "You must add at least one territory before finishing, or type -1 to cancel." << std::endl;
            }
            else {
                doneAdding = true;
            }
        }
        else if (choice < 1 || choice > numberOfTerritories) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
			//add the chosen territory to the defend list if not already added
            Territory* territoryToAdd = (*territoriesOwned)[choice - 1];
            if (std::find(territoriesToDefendThisturn->begin(), territoriesToDefendThisturn->end(), territoryToAdd)
                == territoriesToDefendThisturn->end()) {
                territoriesToDefendThisturn->push_back(territoryToAdd);
                std::cout << territoryToAdd->getName() << " added to defend list." << std::endl;
            }
            else {
                std::cout << "Territory already in defend list. Please choose another." << std::endl;
            }
        }
    }

    return territoriesToDefendThisturn;
}

//choose from territories to defend
// Handles user selection from the defend list
Territory* Player::choseFromToDefend() {

    // Check if the defend list is empty or null
    if (!territoriesToDefend || territoriesToDefend->empty()) {
        std::cout << "You have no territories to defend. Returning..." << std::endl;
        return nullptr;
    }

	//list all territories to defend
    std::cout << "\n----- Territories to Defend -----" << std::endl;
    int i = 1;
    for (Territory* t : *territoriesToDefend) {
        std::cout << i << ". " << t->getName() << ": " << t->getNumberOfArmies() << " armies." << std::endl;
        i++;
    }

    std::cout << "\nEnter the number of the territory from your defend list."
        << "\n(Enter 0 to cancel)\n" << std::endl;

    int choice;
    bool validInput = false;
    while (!validInput) {
        std::cin >> choice;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

		// Allow user to cancel selection , this way they are not stuck if the list is empty.
        if (choice == 0 ) {
            std::cout << "Selection cancelled. Returning..." << std::endl;
            return nullptr;
        }

        // Validate choice range
        if (choice < 1 || choice >(int)territoriesToDefend->size()) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }
	//Return the selected territory
    Territory* targetTerritory = (*territoriesToDefend)[choice - 1];
    return targetTerritory;
}

//choose from territories to attack
// Handles user selection from the attack list
Territory* Player::choseFromToAttack() {

    // Check if the attack list is empty or null
    if (!territoriesToAttack || territoriesToAttack->empty()) {
        std::cout << "You have no territories to attack. Returning..." << std::endl;
        return nullptr;
    }

    std::cout << "\n----- Territories to Attack -----" << std::endl;
    int index = 1;
    for (Territory* t : *territoriesToAttack) {
        std::cout << index << ". " << t->getName() << " : " << t->getNumberOfArmies() << " armies." << std::endl;
        index++;
    }

    std::cout << "\nEnter the number of the territory you want from your to attack list."
        << "\n(Enter 0 to cancel)\n" << std::endl;

    int choice;
    bool validInput = false;
    while (!validInput) {
        std::cin >> choice;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        // Allow user to cancel selection , this way they are not stuck if the list is empty.
        if (choice == 0 ) {
            std::cout << "Selection cancelled. Returning..." << std::endl;
            return nullptr;
        }

        // Validate choice range
        if (choice < 1 || choice >(int)territoriesToAttack->size()) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

	Territory* targetTerritory = (*territoriesToAttack)[choice - 1]; // Return the selected territory
    return targetTerritory;
}

//choose from adjacent territories
// Allows the player to choose a territory adjacent to a given territory for attack or defense
Territory* Player::choseFromAdjacent(Territory* ownTerritory, const std::string& option) {
    std::vector<Territory*> adjTerritories;
    int index = 1;
    std::cout << "Choose a territory: " << std::endl;

	// List adjacent territories based on the option (attack or defend)
	// For attack, list adjacent territories from the attack list
    if (option == "attack") {
        for (Territory* t : *territoriesToAttack) {
            if (t->isAdjacentTo(ownTerritory)) {
                adjTerritories.push_back(t);
                std::cout << index << ". " << t->getName()
                    << " : " << t->getNumberOfArmies() << " armies." << std::endl;
                index++;
            }
        }
    }
	// For defend, list adjacent territories owned by the player
    else if (option == "defend") {
        for (Territory* t : *territoriesOwned) {
            if (t->isAdjacentTo(ownTerritory)) {
                adjTerritories.push_back(t);
                std::cout << index << ". " << t->getName()
                    << " : " << t->getNumberOfArmies() << " armies." << std::endl;
                index++;
            }
        }
    }

    if (adjTerritories.empty()) {
        std::cout << "No adjacent territories available to " << option << ". Returning..." << std::endl;
        return nullptr;
    }

    std::cout << "\nEnter the number of the territory (0 cancel): ";

    int choice;
    bool validInput = false;
    while (!validInput) {
        std::cin >> choice;

		// Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

		// Allow user to cancel selection
        if (choice == 0 ) {
            std::cout << "Selection cancelled. Returning..." << std::endl;
            return nullptr;
        }
		// Validate choice range
        if (choice < 1 || choice >(int)adjTerritories.size()) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

	return adjTerritories[choice - 1]; // Return the selected adjacent territory
}


// toAttack()
// Player selects the territories they wish to attack this turn, in priority order.
// Returns a vector of Territory pointers representing the attack list.
std::vector<Territory*>* Player::toAttack() {
    std::vector<Territory*> possibleTerritories;

    // Add all adjacent territories that are not owned by the player
    for (Territory* t : *territoriesOwned) {
        const std::vector<Territory*>& adjacentTerritories = t->getAdjacentTerritories();
        for (Territory* adjTerritory : adjacentTerritories) {
            if (std::find(possibleTerritories.begin(), possibleTerritories.end(), adjTerritory) == possibleTerritories.end()
                && (*(adjTerritory->getOwner()) != *this)) {
                possibleTerritories.push_back(adjTerritory);
            }
        }
    }

    int numberOfPossibleTerritories = possibleTerritories.size();
    std::vector<Territory*>* territoriesToAttackThisturn = new std::vector<Territory*>();

    if (numberOfPossibleTerritories == 0) {
        std::cout << "Player has no territories to attack." << std::endl;
        return territoriesToAttackThisturn;
    }
    std::cout << "Please add territories to attack in priority (enter 0 to finish or cancel):" << std::endl;

	// Display possible territories to attack with the number of armies present on it
    for (int i = 0; i < numberOfPossibleTerritories; i++) {
        std::cout << i + 1 << ". " << possibleTerritories[i]->getName()
            << " - " << possibleTerritories[i]->getNumberOfArmies() << " armies." << std::endl;
    }

    bool doneAdding = false;
    while (!doneAdding) {
        std::cout << "Enter the number of the territory to add to attack list (0 to finish/cancel): ";
        int choice;
        std::cin >> choice;

		// Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

		// Handle finish/cancel input
        if (choice == 0) {
            if (territoriesToAttackThisturn->empty()) {
                std::cout << "No territories selected for attack. Returning empty list." << std::endl;
            }
            doneAdding = true;
        }
		// Validate choice range
        else if (choice < 1 || choice > numberOfPossibleTerritories) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
			// Add the chosen territory to the attack list if not already added
            Territory* territoryToAdd = possibleTerritories[choice - 1];
            if (std::find(territoriesToAttackThisturn->begin(), territoriesToAttackThisturn->end(), territoryToAdd) == territoriesToAttackThisturn->end()) {
                territoriesToAttackThisturn->push_back(territoryToAdd);
                std::cout << territoryToAdd->getName() << " added to attack list." << std::endl;
            }
            else {
                std::cout << "Territory already in attack list. Please choose another." << std::endl;
            }
        }
    }

	return territoriesToAttackThisturn;// Return the attack list
}

// Draw backup cards to ensure the player has at least 2 cards in hand at the beginning of the turn
void Player::drawBackUpCards() {
    Hand* playersHand = getHand();
	int cardcount = playersHand->getSize();
    for (int i = cardcount; i < 2; i++) {
        Card* drawnCard = deck->draw(playersHand);
        if (drawnCard) {
            std::cout << "Player " << getName() << " drew a card." << std::endl;
        }
        else {
            std::cout << "Deck is empty. No more cards to draw." << std::endl;
            break;
        }
	}

}

// Reset defend and attack lists at the beginnign of the turn 
void Player::resetDefendAndAttack() {
    if (territoriesToDefend) {
        delete territoriesToDefend;
        territoriesToDefend = nullptr;
    }
    if (territoriesToAttack) {
        delete territoriesToAttack;
        territoriesToAttack = nullptr;
    }
}

// issueAdvanceOrder()
// Handles the issuance of an Advance order by the player.
// Returns a pointer to the created Advance order
Order* Player::issueAdvanceOrder() {
	std::cout << "You want to advance armies to defend your own territories or to attack a neighboring territory,\n press\"d\"for defend and \"a\" for attack. press 0 to cancel." << std::endl;
	bool validInput = false;
	char choice;
	while (!validInput) {
		std::cin >> choice;

		//Handles defend choice
		if (choice == 'd' || choice == 'D') {
            Territory* fromTerritory = nullptr;
            Territory* toTerritory = nullptr;

			// Loop until a valid territory with armies is chosen to move armies from
			while (fromTerritory == nullptr || fromTerritory->getNumberOfArmies() < 1) {
				// chose the territory to move armies from
				std::cout << "chose the territory you want to move armies from: " << std::endl;
				fromTerritory = choseFromToDefend();
                if(fromTerritory == nullptr) {
                    std::cout << "Advance order cancelled." << std::endl;
                    return nullptr;
				}
				// Check if there are armies available to move
				if (fromTerritory->getNumberOfArmies() < 1) {
					std::cout << "No armies available to move from " << fromTerritory->getName() << " Please chose another Territory or press 0 to skip your turn \n" << std::endl;
                    int choice;
                    std::cin >> choice;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        continue;
                    }
                    if(choice == '0') {
                        std::cout << "Advance order cancelled." << std::endl;
						return nullptr; // Exit if user chooses to cancel
					}

				}
			}
            
			//Chose the territory to move armies to
			std::cout << "chose the territory you want to move armies to: " << std::endl;
			toTerritory = choseFromAdjacent(fromTerritory,"defend");


			int armiestoMove;
			bool validArmiesInput = false;
			// Loop until a valid number of armies is entered
			while (!validArmiesInput) {
				int maxnumber = fromTerritory->getNumberOfArmies();
				std::cout << "How many armies do you wish to move? max: " << maxnumber << std::endl;
				std::cin >> armiestoMove;
				if (armiestoMove < 1 || armiestoMove > maxnumber) {
					std::cout << "Invalid number of armies. Please try again." << std::endl;
				}
				else {
					validArmiesInput = true;
				}
				// Create the Advance order using the order factory
				std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
				Order* advanceOrder = orderFactory(Player::OrderType::DEPLOY, fromTerritory, toTerritory, nullptr, armiestoMove);
				
				validInput = true;
                return advanceOrder;
			}
		}
		//Handles attack choice
		else if (choice == 'a' || choice == 'A') {
			// chose the territory to move armies from
            Territory* fromTerritory = nullptr;
            Territory* toTerritory = nullptr;
			while (fromTerritory == nullptr || fromTerritory->getNumberOfArmies() < 1) {

				std::cout << "Choose the territory you want to move armies from: " << std::endl;
				fromTerritory = choseFromToDefend();
				if (fromTerritory == nullptr) { return nullptr; } // Exit if user chooses to cancel
				if (fromTerritory->getNumberOfArmies() < 1) {
					std::cout << "No armies available to move from " << fromTerritory->getName() << " Please choose another Territory " << std::endl;
                    int choice;
                    std::cin >> choice;
					// Handle invalid input types (non-numeric)
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        continue;
                    }
					// Exit if user chooses to cancel
                    if (choice == '0') {
                        std::cout << "Advance order cancelled." << std::endl;
                        return nullptr;
                    }
				}
			}

			//list territories to attack          
			std::cout << "chose the territory you want to move armies to: " << std::endl;
			 toTerritory = choseFromAdjacent(fromTerritory, "attack");
			int armiestoMove;
			bool validArmiesInput = false;
			while (!validArmiesInput) {
				std::cout << "How many armies do you wish to move? " << std::endl;
				std::cin >> armiestoMove;

				// Handle invalid input types (non-numeric)
                if (std::cin.fail()) {
                    // Input was not an integer
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    continue;
                }
				// Validate number of armies to move
				if (armiestoMove < 1 || armiestoMove > fromTerritory->getNumberOfArmies()) {
					std::cout << "Invalid number of armies. Please try again." << std::endl;
				}
				else {
					validArmiesInput = true;
				}
				// Create the Advance order using the order factory
				std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
				Order* advanceOrder = orderFactory(Player::OrderType::ADVANCE, fromTerritory, toTerritory, nullptr, armiestoMove);
				
				validInput = true;
                return advanceOrder;
			}
		}
        else if (choice == '0') {
			std::cout << "Advance order cancelled." << std::endl; // Exit if user chooses to cancel
            validInput = true;
		}
		else {
			std::cout << "Invalid choice. Please try again." << std::endl; 
		}
	}
    return nullptr;
}

// issueDeployOrder()
// Handles the issuance of a Deploy order by the player.
// Returns a pointer to the created Deploy order
Order* Player::issueDeployOrder() {

    std::cout << "\nWhich territories would you like to deploy armies to? " << std::endl;
    int i = 1;
    
	Territory* targetTerritory = choseFromToDefend();
    if (targetTerritory == nullptr) {
        std::cout << "Deploy order cancelled." << std::endl;
        return nullptr;
    }
    int armiesToDeploy;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Enter number of armies to deploy to " << targetTerritory->getName()
            << " (max " << (*reinforcementPool - *pendingDeployments) << "): ";
        std::cin >> armiesToDeploy;

		// Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            // Input was not an integer
            std::cin.clear(); // clear the failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

		// Validate number of armies to deploy to the chosen territory
        if (armiesToDeploy < 1 || armiesToDeploy > (*reinforcementPool - *pendingDeployments)) {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

    // Create Deploy order using the order factory
    Order* deployOrder = orderFactory(Player::OrderType::DEPLOY,nullptr, targetTerritory,nullptr, armiesToDeploy);
    
	// Update pending deployments to match the commited armies
	*pendingDeployments += armiesToDeploy;
    std::cout << armiesToDeploy << " armies deployed to " << targetTerritory->getName() << ". Remaining reinforcement pool: " << (*reinforcementPool - *pendingDeployments) << std::endl;
	return deployOrder;
}
// issueBombOrder()
// Handles the issuance of a Bomb order by the player.
// Returns a pointer to the created Bomb order
Order* Player::issueBombOrder() {
    // choose target territory
	std::cout << "choose the territory you want to bomb: " << std::endl;
    Territory* targetTerritory = choseFromToAttack();
    if (targetTerritory == nullptr) {
        std::cout << "Bomb order cancelled." << std::endl;
        return nullptr;
    }
	// Create Bomb order using the order factory
    std::cout << "Creating a Bomb order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order* bombOrder = orderFactory(Player::OrderType::BOMB, nullptr, targetTerritory, nullptr, 0);
    return bombOrder;
}

// issueBlockadeOrder()
// Handles the issuance of a Blockade order by the player.
//// Returns a pointer to the created Blockade order
Order* Player::issueBlockadeOrder() {
    // choose target territory
    std::cout << "choose the territory you want to blockade: " << std::endl;
    Territory* targetTerritory = choseFromToDefend();
    if (targetTerritory == nullptr) {
        std::cout << "Blockade order cancelled." << std::endl;
        return nullptr;
    }
	// Create Blockade order using the order factory
	std::cout << "Creating a Blockade order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
	Order* blockadeOrder = orderFactory(Player::OrderType::BLOCKADE, nullptr, targetTerritory, new Player(), 0); //FLAG : need to specify neutral player after integration
	return blockadeOrder;
}

// issueAirliftOrder()
// Handles the issuance of an Airlift order by the player.
// Returns a pointer to the created Airlift order
Order* Player::issueAirliftOrder() {
    Territory* fromTerritory;
    Territory* toTerritory;
   
    do {
        // choose the origin territory
        std::cout << "Choose the territory you want to airlift armies from: " << std::endl;
        fromTerritory = choseFromToDefend();
        if (fromTerritory == nullptr) {
            std::cout << "Airlift order cancelled." << std::endl;
            return nullptr;
        }

        // choose target territory
        std::cout << "Choose the territory you want to airlift armies to: " << std::endl;
        toTerritory = choseFromToDefend();
        if (toTerritory == nullptr) {
            std::cout << "Airlift order cancelled." << std::endl;
            return nullptr;
        }
        if (fromTerritory == toTerritory) {
            std::cout << "Please try again, the territories cannot be the same." << std::endl;
        }
	} while (fromTerritory == toTerritory); //FLAG : this compares the pointers but the == operator is not overloaded for Territory 

    //chose number of armies
	int armiesToAirlift;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Enter number of armies to airlift from " << fromTerritory->getName()
            << " to " << toTerritory->getName() << " (max " << fromTerritory->getNumberOfArmies() << "): ";
        std::cin >> armiesToAirlift;

		// Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        if (armiesToAirlift < 1 || armiesToAirlift > fromTerritory->getNumberOfArmies()) {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

	// Create Airlift order using the order factory
    std::cout << "Creating an Airlift order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order* airliftOrder = orderFactory(Player::OrderType::AIRLIFT, fromTerritory, toTerritory, nullptr, armiesToAirlift);
	return airliftOrder;

}

// issueNegotiateOrder()
// Handles the issuance of a Negotiate order by the player.
// Returns a pointer to the created Negotiate order
Order* Player::issueNegotiateOrder() {
	//choose the player to negotiate with
    std::cout << "Choose the territory of the player you want to negotiate with: " << std::endl;
    //chose the owner of a territory 
	Territory* targetTerritory = choseFromToAttack();
    if (targetTerritory == nullptr) {
        std::cout << "Negociate order cancelled." << std::endl;
        return nullptr;
    }
	// Get the owner of the chosen territory - Negocation is between players not territories
	Player* owner = targetTerritory->getOwner();
	std::cout << "Creating a Negotiate order with player owning " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
	// Create Negotiate order using the order factory
	Order* negotiateOrder = orderFactory(Player::OrderType::NEGOTIATE, nullptr, nullptr, owner, 0);
	return negotiateOrder;
}
// orderFactory()
// Factory method to create different types of orders based on the specified type
// Returns a pointer to the created Order
Order* Player::orderFactory(Player::OrderType type,
    Territory* sourceTerritory,
    Territory* targetTerritory ,
    Player* targetOrNeutral ,
    int armies) {

    switch (type) {
    case OrderType::ADVANCE:
        std::cout << "Creating Advance Order via Factory Method" << std::endl;
        return new Advance(this, sourceTerritory, targetTerritory, armies);
    case OrderType::DEPLOY:
		std::cout << "Creating Deploy Order via Factory Method" << std::endl;
        return new Deploy(this, targetTerritory, armies);
    case OrderType::BLOCKADE:
		std::cout << "Creating Blockade Order via Factory Method" << std::endl;
        return new Blockade(this, targetTerritory, targetOrNeutral);
    case OrderType::AIRLIFT:
		std::cout << "Creating Airlift Order via Factory Method" << std::endl;
        return new Airlift(this, sourceTerritory, targetTerritory, armies);
    case OrderType::BOMB:
		std::cout << "Creating Bomb Order via Factory Method" << std::endl;
        return new Bomb(this, targetTerritory);
    case OrderType::NEGOTIATE:
		std::cout << "Creating Negotiate Order via Factory Method" << std::endl;
        return new Negotiate(this,  targetOrNeutral );
    default:
        return nullptr;
    }
}



// issueOrder()
// Main method for issuing orders during the player's turn
// First, toDefend and ToAttack lists are set if not already done for the turn
// Calls the appropriate order issuance methods based on player input
// Only deploy orders casn be called until the reinforcement pool is all pendingDeployment 
// All orders are created using the order factory
// Adds the created orders to the player's order list
void Player::issueOrder() {
    
    // ToDefend and ToAttack list are chosen only once per issueOrderPhase and are cleared at the beginning of a new issueOrderPhase turn
    if (territoriesToDefend == nullptr || territoriesToAttack == nullptr) {
        territoriesToDefend = new std::vector<Territory*>();
        territoriesToAttack = new std::vector<Territory*>();

        std::cout << "\nPlease specify which of your territories to defend : " << std::endl;
        if (territoriesToDefend != nullptr) {
            delete territoriesToDefend;
        }
        territoriesToDefend = toDefend();
       
        if (territoriesToAttack != nullptr) {
            delete territoriesToAttack;
        }
        std::cout << "\nPlease specify which territories you want to attack : " << std::endl;
        territoriesToAttack = toAttack();
    }

    std::cout << "\nHere are the possible orders you can issue" << std::endl;
    // Only deploy order is available if the reinforcement pool is not empty
    if ( ( *reinforcementPool - *pendingDeployments) != 0) {
        std::cout << "\nYour reinforcement pool is not empty, please Deploy your armies first" << std::endl;
        std::cout << ( *reinforcementPool - *pendingDeployments) << " armies need to be deployed." << std::endl;
        // Deploy order to assign reinforcements to owned territories
        Order* deployOrder =  issueDeployOrder();
        if (deployOrder != nullptr) { (*orderslist).add(deployOrder); }
      
    } else {
       
		// Other orders are available once the reinforcement pool is empty
        std::cout << "\nHere are the possible orders" << std::endl;
        std::cout << "0. End turn" << std::endl; 
        std::cout << "1. Advance" << std::endl;
        std::cout << "From the cards available in your hand : \n" << std::endl;

        for (int i = 0; i < (*playerHand).getSize(); i++) {
            std::cout << (i + 2) << ". " << playerHand->getCard(i)->getName() << std::endl;
        }

        bool validInput = false;
        int choice;
        while (!validInput) {
            std::cout << "Please choose an order number" << std::endl;
            std::cin >> choice;
			// Handle invalid input types (non-numeric)
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number." << std::endl;
                continue;
            }

            if (choice < 0 || choice > (1 + (*playerHand).getSize())) {
                std::cout << "Invalid choice. Please try again." << std::endl;
            } else {
                validInput = true;
            }
        }
        if (choice == 0 ) {

            return;
        }
        else if (choice == 1) {
			// Advance order (to defend or attack)
           Order* advanceOrder =  issueAdvanceOrder();
           if (advanceOrder != nullptr) { (*orderslist).add(advanceOrder); }
           else { std::cout << "There was a problem with the advance order, it was not added to the orderList" << std::endl; };
		}
		else {
			// Issue Order from hand
			std::string cardName = playerHand->getCard(choice - 2)->getName();
			std::cout << "Playing the card" << cardName << ", it will create the order and add it to the orders list" << std::endl;
			Order* cardOrder = playerHand->getCard(choice - 2)->play(*playerHand, *deck, this);
			if (cardOrder != nullptr) { (*orderslist).add(cardOrder); }
            else { std::cout << "There was a problem with the card order, it was not added to the orderList" << std::endl; };
		}
	}
}

// getNextOrderToExecute()
// Retrieves the next order to be executed from the player's order list
// Returns a pointer to the next Order, or nullptr if no orders are available
Order* Player::getNextOrderToExecute(){
    if (orderslist->size() == 0) {
        std::cout << "No orders to execute." << std::endl;
        return nullptr;
    }
    else {
        std::cout << "Getting next order to execute." << std::endl;
        Order* nextOrder = orderslist->getOrder(0);
        return nextOrder;
	}
	
}

