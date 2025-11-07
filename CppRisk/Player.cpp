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
std::vector<Territory*> Player::toDefend() {
    int numberOfTerritories = (*territoriesOwned).size();
    std::vector<Territory*> territoriesToDefend;

    if (numberOfTerritories == 0) {
        std::cout << "Player has no territories to defend." << std::endl;
        return territoriesToDefend;
    }
    
    int randomNumberOfTerritories = std::rand() % numberOfTerritories; 

    while (territoriesToDefend.size() < randomNumberOfTerritories) {
        int randomTerritoryIndex = std::rand() % numberOfTerritories;
        Territory* territoryToAdd = territoriesOwned->at(randomTerritoryIndex);
        if (std::find(territoriesToDefend.begin(), territoriesToDefend.end(), territoryToAdd) == territoriesToDefend.end()) {
            territoriesToDefend.push_back(territoryToAdd);
        }
    }
    return territoriesToDefend;
}

// toAttack()
std::vector<Territory*> Player::toAttack() {
    std::vector<Territory*> possibleTerritories;

    for (Territory* t : *territoriesOwned) {
        const std::vector<Territory*>& adjacentTerritories = t->getAdjacentTerritories();
        for (Territory* adjTerritory : adjacentTerritories) {
            if (std::find(possibleTerritories.begin(), possibleTerritories.end(), adjTerritory) == possibleTerritories.end()) {
                possibleTerritories.push_back(adjTerritory); 
            }
        }
    }
    
    int numberOfPossibleTerritories = possibleTerritories.size();
    std::vector<Territory*> territoriesToAttack;

    if (numberOfPossibleTerritories == 0) {
        std::cout << "Player has no territories to attack." << std::endl;
        return territoriesToAttack;
    }

    int randomNumberOfTerritories = std::rand() % numberOfPossibleTerritories;

    while (territoriesToAttack.size() < randomNumberOfTerritories) {
        int randomTerritoryIndex = std::rand() % numberOfPossibleTerritories;
        Territory* territoryToAdd = possibleTerritories.at(randomTerritoryIndex);
        if (std::find(territoriesToAttack.begin(), territoriesToAttack.end(), territoryToAdd) == territoriesToAttack.end()) {
            territoriesToAttack.push_back(territoryToAdd);
        }
    }
    return territoriesToAttack;
}

// issueOrder()
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
    }          
}