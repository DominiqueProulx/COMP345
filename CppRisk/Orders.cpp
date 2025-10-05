// -------------------------------
// COMP 345 - Fall 2025
// Risk Game Project
// Part 3: Orders List
// File: Orders.cpp
// -------------------------------

#include "Orders.h"
#include <iostream>

// ============================================================================
// DUMMY CLASSES IMPLEMENTATION (Placeholders for integration)
// ============================================================================

// Territory placeholder implementation
Territory::Territory(const std::string& territoryName) {
    name = new std::string(territoryName);
    numberOfArmies = new int(0);
}

Territory::Territory(const Territory& other) {
    name = new std::string(*other.name);
    numberOfArmies = new int(*other.numberOfArmies);
}

Territory& Territory::operator=(const Territory& other) {
    if (this != &other) {
        delete name;
        delete numberOfArmies;
        name = new std::string(*other.name);
        numberOfArmies = new int(*other.numberOfArmies);
    }
    return *this;
}

Territory::~Territory() {
    delete name;
    delete numberOfArmies;
}

std::string Territory::getName() const {
    return *name;
}

int Territory::getNumberOfArmies() const {
    return *numberOfArmies;
}

void Territory::setNumberOfArmies(int armies) {
    *numberOfArmies = armies;
}

// Player placeholder implementation
Player::Player(const std::string& name) {
    playerName = new std::string(name);
}

Player::Player(const Player& other) {
    playerName = new std::string(*other.playerName);
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        delete playerName;
        playerName = new std::string(*other.playerName);
    }
    return *this;
}

Player::~Player() {
    delete playerName;
}

std::string Player::getName() const {
    return *playerName;
}

// ============================================================================
// ORDER BASE CLASS IMPLEMENTATION
// ============================================================================

// Constructor
Order::Order(const std::string& orderType) {
    this->orderType = new std::string(orderType);
    this->effect = nullptr;  // No effect until executed
    this->executed = new bool(false);
}

// Copy constructor
Order::Order(const Order& other) {
    orderType = new std::string(*other.orderType);
    effect = other.effect ? new std::string(*other.effect) : nullptr;
    executed = new bool(*other.executed);
}

// Destructor
Order::~Order() {
    delete orderType;
    delete effect;
    delete executed;
}

// Assignment operator
Order& Order::operator=(const Order& other) {
    if (this != &other) {
        delete orderType;
        delete effect;
        delete executed;
        
        orderType = new std::string(*other.orderType);
        effect = other.effect ? new std::string(*other.effect) : nullptr;
        executed = new bool(*other.executed);
    }
    return *this;
}

// Getters
std::string Order::getType() const {
    return *orderType;
}

std::string Order::getEffect() const {
    return effect ? *effect : "Not yet executed";
}

bool Order::isExecuted() const {
    return *executed;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "Order Type: " << *order.orderType;
    if (*order.executed && order.effect) {
        os << " | Effect: " << *order.effect;
    }
    return os;
}

// ============================================================================
// DEPLOY ORDER IMPLEMENTATION
// ============================================================================

Deploy::Deploy() : Order("Deploy") {}

Deploy::Deploy(const Deploy& other) : Order(other) {}

Deploy::~Deploy() {}

Deploy& Deploy::operator=(const Deploy& other) {
    Order::operator=(other);
    return *this;
}

bool Deploy::validate() {
    // For now, always valid (will be expanded during integration)
    // In full game: check if player owns territory, has armies to deploy
    std::cout << "Validating Deploy order..." << std::endl;
    return true;
}

void Deploy::execute() {
    if (validate()) {
        std::cout << "Executing Deploy order..." << std::endl;
        
        // Set effect description
        delete effect;
        effect = new std::string("Armies deployed to target territory");
        *executed = true;
        
        std::cout << "Deploy order executed successfully." << std::endl;
    } else {
        std::cout << "Deploy order is invalid and cannot be executed." << std::endl;
    }
}

Order* Deploy::clone() const {
    return new Deploy(*this);
}

// ============================================================================
// ADVANCE ORDER IMPLEMENTATION
// ============================================================================

Advance::Advance() : Order("Advance") {}

Advance::Advance(const Advance& other) : Order(other) {}

Advance::~Advance() {}

Advance& Advance::operator=(const Advance& other) {
    Order::operator=(other);
    return *this;
}

bool Advance::validate() {
    // For now, always valid
    // In full game: check if player owns source territory, territories are adjacent
    std::cout << "Validating Advance order..." << std::endl;
    return true;
}

void Advance::execute() {
    if (validate()) {
        std::cout << "Executing Advance order..." << std::endl;
        
        delete effect;
        effect = new std::string("Armies moved from source to target territory");
        *executed = true;
        
        std::cout << "Advance order executed successfully." << std::endl;
    } else {
        std::cout << "Advance order is invalid and cannot be executed." << std::endl;
    }
}

Order* Advance::clone() const {
    return new Advance(*this);
}

// ============================================================================
// BOMB ORDER IMPLEMENTATION
// ============================================================================

Bomb::Bomb() : Order("Bomb") {}

Bomb::Bomb(const Bomb& other) : Order(other) {}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& other) {
    Order::operator=(other);
    return *this;
}

bool Bomb::validate() {
    // For now, always valid
    // In full game: check if target is adjacent, not owned by player
    std::cout << "Validating Bomb order..." << std::endl;
    return true;
}

void Bomb::execute() {
    if (validate()) {
        std::cout << "Executing Bomb order..." << std::endl;
        
        delete effect;
        effect = new std::string("Half of the armies on target territory destroyed");
        *executed = true;
        
        std::cout << "Bomb order executed successfully." << std::endl;
    } else {
        std::cout << "Bomb order is invalid and cannot be executed." << std::endl;
    }
}

Order* Bomb::clone() const {
    return new Bomb(*this);
}

// ============================================================================
// BLOCKADE ORDER IMPLEMENTATION
// ============================================================================

Blockade::Blockade() : Order("Blockade") {}

Blockade::Blockade(const Blockade& other) : Order(other) {}

Blockade::~Blockade() {}

Blockade& Blockade::operator=(const Blockade& other) {
    Order::operator=(other);
    return *this;
}

bool Blockade::validate() {
    // For now, always valid
    // In full game: check if player owns the territory
    std::cout << "Validating Blockade order..." << std::endl;
    return true;
}

void Blockade::execute() {
    if (validate()) {
        std::cout << "Executing Blockade order..." << std::endl;
        
        delete effect;
        effect = new std::string("Territory armies tripled and territory made neutral");
        *executed = true;
        
        std::cout << "Blockade order executed successfully." << std::endl;
    } else {
        std::cout << "Blockade order is invalid and cannot be executed." << std::endl;
    }
}

Order* Blockade::clone() const {
    return new Blockade(*this);
}

// ============================================================================
// AIRLIFT ORDER IMPLEMENTATION
// ============================================================================

Airlift::Airlift() : Order("Airlift") {}

Airlift::Airlift(const Airlift& other) : Order(other) {}

Airlift::~Airlift() {}

Airlift& Airlift::operator=(const Airlift& other) {
    Order::operator=(other);
    return *this;
}

bool Airlift::validate() {
    // For now, always valid
    // In full game: check if player owns both territories
    std::cout << "Validating Airlift order..." << std::endl;
    return true;
}

void Airlift::execute() {
    if (validate()) {
        std::cout << "Executing Airlift order..." << std::endl;
        
        delete effect;
        effect = new std::string("Armies airlifted to target territory");
        *executed = true;
        
        std::cout << "Airlift order executed successfully." << std::endl;
    } else {
        std::cout << "Airlift order is invalid and cannot be executed." << std::endl;
    }
}

Order* Airlift::clone() const {
    return new Airlift(*this);
}

// ============================================================================
// NEGOTIATE ORDER IMPLEMENTATION
// ============================================================================

Negotiate::Negotiate() : Order("Negotiate") {}

Negotiate::Negotiate(const Negotiate& other) : Order(other) {}

Negotiate::~Negotiate() {}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    Order::operator=(other);
    return *this;
}

bool Negotiate::validate() {
    // For now, always valid
    // In full game: check if target player exists and is not self
    std::cout << "Validating Negotiate order..." << std::endl;
    return true;
}

void Negotiate::execute() {
    if (validate()) {
        std::cout << "Executing Negotiate order..." << std::endl;
        
        delete effect;
        effect = new std::string("Diplomacy established, preventing attacks this turn");
        *executed = true;
        
        std::cout << "Negotiate order executed successfully." << std::endl;
    } else {
        std::cout << "Negotiate order is invalid and cannot be executed." << std::endl;
    }
}

Order* Negotiate::clone() const {
    return new Negotiate(*this);
}

// ============================================================================
// ORDERSLIST CLASS IMPLEMENTATION
// ============================================================================

// Constructor
OrdersList::OrdersList() {
    orders = new std::vector<Order*>();
}

// Copy constructor - performs deep copy
OrdersList::OrdersList(const OrdersList& other) {
    orders = new std::vector<Order*>();
    for (Order* order : *other.orders) {
        orders->push_back(order->clone());  // Deep copy using clone method
    }
}

// Destructor
OrdersList::~OrdersList() {
    // Delete all order objects
    for (Order* order : *orders) {
        delete order;
    }
    delete orders;
}

// Assignment operator
OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        // Delete existing orders
        for (Order* order : *orders) {
            delete order;
        }
        orders->clear();
        
        // Deep copy from other
        for (Order* order : *other.orders) {
            orders->push_back(order->clone());
        }
    }
    return *this;
}

// Add order to the end of the list
void OrdersList::add(Order* order) {
    if (order != nullptr) {
        orders->push_back(order);
        std::cout << "Order added to list: " << order->getType() << std::endl;
    }
}

// Remove order at specified index
void OrdersList::remove(int index) {
    if (index >= 0 && index < orders->size()) {
        std::cout << "Removing order at index " << index << ": " 
                  << (*orders)[index]->getType() << std::endl;
        
        delete (*orders)[index];  // Delete the order object
        orders->erase(orders->begin() + index);  // Remove from vector
    } else {
        std::cout << "Error: Invalid index " << index << " for remove operation." << std::endl;
    }
}

// Move order from one position to another
void OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < orders->size() && 
        toIndex >= 0 && toIndex < orders->size()) {
        
        std::cout << "Moving order from index " << fromIndex 
                  << " to index " << toIndex << std::endl;
        
        Order* orderToMove = (*orders)[fromIndex];
        orders->erase(orders->begin() + fromIndex);
        orders->insert(orders->begin() + toIndex, orderToMove);
    } else {
        std::cout << "Error: Invalid indices for move operation." << std::endl;
    }
}

// Get size of orders list
int OrdersList::size() const {
    return orders->size();
}

// Get order at specific index
Order* OrdersList::getOrder(int index) const {
    if (index >= 0 && index < orders->size()) {
        return (*orders)[index];
    }
    return nullptr;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const OrdersList& list) {
    os << "OrdersList (" << list.orders->size() << " orders):" << std::endl;
    for (size_t i = 0; i < list.orders->size(); ++i) {
        os << "  [" << i << "] " << *((*list.orders)[i]) << std::endl;
    }
    return os;
}