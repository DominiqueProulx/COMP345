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
#include "PlayerStrategies.h"

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
    
    reinforcementPool = new int(0);
    pendingDeployments = new int(0);
    conqueredTerritoryThisTurn = new bool(false);
    negotiatedPlayers = new std::set<Player*>();
    territoriesToDefend = new std::vector<Territory*>();
    territoriesToAttack = new std::vector<Territory*>();

	strategy = std::make_unique<HumanPlayerStrategy>(this); //default strategy
}
Player::Player(Deck* &deck) {
    Player::playerCount++;
    playerID = new int(playerCount);
    playerColor = new std::string("NoColor");
    territoriesOwned = new std::vector<Territory*>();
    playerHand = new Hand();
    orderslist = new OrdersList();
    this->deck = deck;

       // Assignment 2 additions
    reinforcementPool = new int(0);
    pendingDeployments = new int(0);
    conqueredTerritoryThisTurn = new bool(false);
    negotiatedPlayers = new std::set<Player*>();
	territoriesToDefend = new std::vector<Territory*>();
    territoriesToAttack = new std::vector<Territory*>();

    strategy = std::make_unique<HumanPlayerStrategy>(this); //default strategy
}

Player::Player(Deck*& deck, std::unique_ptr<PlayerStrategies> initialStartegy) {
    Player::playerCount++;
    playerID = new int(playerCount);
    playerColor = new std::string("NoColor");
    territoriesOwned = new std::vector<Territory*>();
    playerHand = new Hand();
    orderslist = new OrdersList();
    this->deck = deck;

    // Assignment 2 additions
    reinforcementPool = new int(0);
    pendingDeployments = new int(0);
    conqueredTerritoryThisTurn = new bool(false);
    negotiatedPlayers = new std::set<Player*>();
    territoriesToDefend = new std::vector<Territory*>();
    territoriesToAttack = new std::vector<Territory*>();

    strategy = std::move(initialStartegy); 
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
    territoriesToDefend = new std::vector<Territory*>();
    territoriesToAttack = new std::vector<Territory*>();
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
Deck* Player::getDeck() const {
    return deck;
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

const std::vector<Territory*>* Player::getTerritoriesToDefend() const {
	return territoriesToDefend;
}
const std::vector<Territory*>* Player::getTerritoriesToAttack() const {
	return territoriesToAttack;
}

PlayerStrategies* Player::getStrategy() const{
    return strategy.get();
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
void Player::setTerritoriesToDefend(const std::vector<Territory*>& territories) {
    if (!territoriesToDefend) {  // allocate if nullptr
        territoriesToDefend = new std::vector<Territory*>();
    }
    territoriesToDefend->clear();
    territoriesToDefend->insert(territoriesToDefend->end(), territories.begin(), territories.end());

}
void Player::setTerritoriesToAttack(const std::vector<Territory*>& territories) {
    if (!territoriesToAttack) {  // allocate if nullptr
        territoriesToAttack = new std::vector<Territory*>();
    }
    territoriesToAttack->clear();
    territoriesToAttack->insert(territoriesToAttack->end(), territories.begin(), territories.end());
}
void Player::setStrategy(std::unique_ptr<PlayerStrategies> newStrategy) {
    strategy = std::move(newStrategy);
}


//Methods
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
void Player::addOrderToOrderlist(Order* order) {
    orderslist->add(order);
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
    this->reinforcementPool = new int(*otherPlayer.reinforcementPool);
    this->pendingDeployments = new int(*otherPlayer.pendingDeployments);
    this->conqueredTerritoryThisTurn = new bool(*otherPlayer.conqueredTerritoryThisTurn);
    this->negotiatedPlayers = new std::set<Player*>(*otherPlayer.negotiatedPlayers);
    
    return *this;
}

void Player::onTerritoryAttacked(Territory* territory, Player* attacker)
{
    
    /*  Neutral becomes Aggressive when attacked  */
    if (dynamic_cast<NeutralPlayerStrategy*>(strategy.get()))
    {
        std::cout << "[Neutral] Player " << getName()
                << " was attacked becoming Aggressive!\n";
        setStrategy(std::make_unique<AggressivePlayerStrategy>(this));
    }
}

//// toDefend()
//// Player selects the territories they wish to defend this turn, in priority order.
//// Returns a vector of Territory pointers representing the defend list.
std::vector<Territory*> Player::toDefend() {
    return strategy->toDefend();
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
std::vector<Territory*> Player::toAttack() {
	return strategy->toAttack();// Copy / Return the attack list
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
       // delete territoriesToDefend;
      //  territoriesToDefend = nullptr;
        territoriesToDefend->clear();

    }
    if (territoriesToAttack) {
       /* delete territoriesToAttack;
        territoriesToAttack = nullptr;*/
		territoriesToAttack->clear();
    }
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
      // Delete old vectors if they exist
    if (territoriesToDefend != nullptr) {
        territoriesToDefend->clear();
    }
    if (territoriesToAttack != nullptr) {
        territoriesToAttack->clear();
    }
	
	strategy->issueOrder();
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

