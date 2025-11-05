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

// Setters
void Player::setColor(const std::string& color) {
    delete this->playerColor; 
    this->playerColor = new std::string(color);
}

void Player::setHand(Hand& hand) {
    delete this->playerHand;
    this->playerHand = &hand;
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

int Player::getReinforcementPool() const {
    return *reinforcementPool;
}

void Player::setReinforcementPool(int armies) {
    *reinforcementPool = armies;
}

bool Player::hasConqueredThisTurn() const {
    return *conqueredTerritoryThisTurn;
}

void Player::setConqueredThisTurn(bool conquered) {
    *conqueredTerritoryThisTurn = conquered;
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
    this->conqueredTerritoryThisTurn = new bool(*otherPlayer.conqueredTerritoryThisTurn);
    this->negotiatedPlayers = new std::set<Player*>(*otherPlayer.negotiatedPlayers);
    
    return *this;
}

// toDefend()
std::vector<Territory*>* Player::toDefend() {
    int numberOfTerritories = (*territoriesOwned).size();
	std::vector<Territory*>* territoriesToDefendThisturn = new std::vector<Territory*>();
  

    if (numberOfTerritories == 0) {
        std::cout << "Player has no territories to defend." << std::endl;
        return territoriesToDefendThisturn;
    }
        bool doneAdding = false;
		
    std::cout << "Please add territories to defend in priority" << std::endl;
    for (int i = 0; i < numberOfTerritories; i++) {
        std::cout << i + 1 << ". " << (*territoriesOwned)[i]->getName() << std::endl;
    }
    while (!doneAdding) {
        std::cout << "Enter the number of the territory to add to defend list (0 to finish): ";
        int choice;
        std::cin >> choice;
        if (choice == 0) {
            doneAdding = true;
        }
        else if (choice < 1 || choice > numberOfTerritories) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            Territory* territoryToAdd = (*territoriesOwned)[choice - 1];
            if (std::find(territoriesToDefendThisturn->begin(), territoriesToDefendThisturn->end(), territoryToAdd) == territoriesToDefendThisturn->end()) {
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

// toAttack()
std::vector<Territory*>* Player::toAttack() {
    std::vector<Territory*> possibleTerritories;

    //add all adjacent territories 
    for (Territory* t : *territoriesOwned) {
        const std::vector<Territory*>& adjacentTerritories = t->getAdjacentTerritories();
        for (Territory* adjTerritory : adjacentTerritories) {
            if (std::find(possibleTerritories.begin(), possibleTerritories.end(), adjTerritory) == possibleTerritories.end()) {
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

	bool doneAdding = false;
   
        std::cout << "Please add territories to attack in priority" << std::endl;
        for (int i = 0; i < numberOfPossibleTerritories; i++) {
            std::cout << i + 1 << ". " << possibleTerritories[i]->getName() << std::endl;
		}
        while (!doneAdding) {
            std::cout << "Enter the number of the territory to add to attack list (0 to finish): ";
            int choice;
            std::cin >> choice;
            if (choice == 0) {
                doneAdding = true;
            }
            else if (choice < 1 || choice > numberOfPossibleTerritories) {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
            else {
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

    return territoriesToAttackThisturn;
}

void Player::resetDefendAndAttack() {
    territoriesToDefend->clear();
    territoriesToAttack->clear();
}
// issueOrder()
void Player::issueOrder() {

    //todo: modify so only 1 order can be issues per issueOrder call 

    //ToDefend and ToAttakc list are chosen only once issueOrderPhase and are cleared at the beginning of a new issueOrderPhase
    if(territoriesToDefend == nullptr || territoriesToAttack == nullptr) {
        territoriesToDefend = new std::vector<Territory*>();
        territoriesToAttack = new std::vector<Territory*>();
	
    std::cout << "\nPlease specify which of your territories to defend : " << std::endl;
    static std::vector<Territory*>* territoriesToDefend = toDefend();
	std::cout << "\nPlease specify which territories you want to attack : " << std::endl;
	static std::vector<Territory*>* territoriesToAttack = toAttack();
    }

    std::cout << "\nHere are the possible orders your can issue" << std::endl;
    
    while(reinforcementPool != 0){
        std::cout << "Your reinforcement pool is not empty , please Deploy your armies first" << std::endl;
        std::cout << reinforcementPool << " armies need to be deployed." << std::endl;

        std::cout << "Which territories would you like to deploy armies to? " << std::endl;
        for (Territory* t : *territoriesToDefend) {
            std::cout << t << ". " << t->getName() << std::endl;
        }

        int choice;
		bool validInput = false;
        while (!validInput) {
            std::cout << "Please enter the number of the territory to deploy to: ";
            std::cin >> choice;
            if (choice < 1 || choice > territoriesToDefend->size()) {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
            else { validInput = true; }
        }
        Territory* targetTerritory = (*territoriesToDefend)[choice - 1];
        int armiesToDeploy;
        validInput = false;
        while (!validInput) {
            std::cout << "Enter number of armies to deploy to " << targetTerritory->getName() << " (max " << *reinforcementPool << "): ";
            std::cin >> armiesToDeploy;
            if (armiesToDeploy < 1 || armiesToDeploy > *reinforcementPool) {
                std::cout << "Invalid number of armies. Please try again." << std::endl;
            }
            else { validInput = true; }
        }
        // Create Deploy order
        Order* deployOrder = new Deploy(this, targetTerritory, armiesToDeploy);
        (*orderslist).add(deployOrder);
        *reinforcementPool -= armiesToDeploy;
		std::cout << armiesToDeploy << " armies deployed to " << targetTerritory->getName() << ". Remaining reinforcement pool: " << *reinforcementPool << std::endl;
    }


//TODO finish issue Order
               // if reinforcement pool not empty 
		        	// can only issue deploy order
            //Advance order 
                // 1. To defend ( from owned to owned)
				// 2. To attack  ( from owned to not owned)
            //issue Order from hand
/*			
    std::cout << "\nHere are the possible orders" << std::endl;
    std::cout << "1. Deploy" << std::endl;
    std::cout << "2. Advance" << std::endl;
    std::cout << "From the cards available in your hand : \n" << std::endl;

    for (int i = 0; i < (*playerHand).getSize(); i++) {
        std::cout << (i+3) <<". " << playerHand->getCard(i)->getName() << std::endl;
    }

    bool validInput = false;
    int choice;
    while (!validInput) {
        std::cout << "Please chose an order number" << std::endl;
        
        std::cin >> choice;
        if (choice < 1 || choice > (2 + (*playerHand).getSize())) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else { validInput = true; }
    }

    if (choice == 1) {
        std::cout << " Creating a Deploy order and adding it to the orders list" << std::endl;
        Order* deployOrder = new Deploy();
        (*orderslist).add(deployOrder);
    }
    else if (choice == 2) {
        std::cout << " Creating a Advance order and adding it to the orders list" << std::endl;
        Order* advanceOrder = new Advance();
        (*orderslist).add(advanceOrder);
    }
    else {
        std::string cardName = playerHand->getCard(choice - 3)->getName();
        std::cout << " Creating an " << cardName  << " order and adding it to the orders list" << std::endl;
        Order* cardOrder = playerHand->getCard(choice - 3)->play(*playerHand, *deck);
        (*orderslist).add(cardOrder);
    }  */        
}

Order* Player::getNextOrderToExecute(){
    if (orderslist->size() == 0) {
        std::cout << "No orders to execute." << std::endl;
        return nullptr;
    }
    else {
        std::cout << "Getting next order to execute." << std::endl;
        Order* nextOrder = orderslist->getOrder(0);
        orderslist->remove(0); //TODO: ensure that it is my resposnability to remove this
        return nextOrder;
	}
	
}