// -------------------------------
// COMP 345 - Fall 2025
// Risk Game Project
// Assignment 2 - Part 4: Order Execution
// File: Orders.cpp
// -------------------------------

#include "Orders.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// NOTE: Territory and Player implementations are now in Map.cpp and Player.cpp

// ============================================================================
// ORDER BASE CLASS IMPLEMENTATION
// ============================================================================

Order::Order(const std::string& orderType) {
    this->orderType = new std::string(orderType);
    this->effect = nullptr;
    this->executed = new bool(false);
}

Order::Order(const Order& other) {
    orderType = new std::string(*other.orderType);
    effect = other.effect ? new std::string(*other.effect) : nullptr;
    executed = new bool(*other.executed);
}

Order::~Order() {
    delete orderType;
    delete effect;
    delete executed;
}

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

std::string Order::getType() const {
    return *orderType;
}

std::string Order::getEffect() const {
    return effect ? *effect : "Not yet executed";
}

bool Order::isExecuted() const {
    return *executed;
}

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

// Default constructor for Assignment 1 compatibility
Deploy::Deploy() : Order("Deploy") {
    this->player = nullptr;
    this->targetTerritory = nullptr;
    this->armiesToDeploy = new int(0);
}

Deploy::Deploy(Player* player, Territory* target, int numArmies) 
    : Order("Deploy") {
    this->player = player;
    this->targetTerritory = target;
    this->armiesToDeploy = new int(numArmies);
}

Deploy::Deploy(const Deploy& other) : Order(other) {
    player = other.player;
    targetTerritory = other.targetTerritory;
    armiesToDeploy = new int(*other.armiesToDeploy);
}

Deploy::~Deploy() {
    delete armiesToDeploy;
}

Deploy& Deploy::operator=(const Deploy& other) {
    if (this != &other) {
        Order::operator=(other);
        player = other.player;
        targetTerritory = other.targetTerritory;
        delete armiesToDeploy;
        armiesToDeploy = new int(*other.armiesToDeploy);
    }
    return *this;
}

bool Deploy::validate() {
    // Handle default constructor case (Assignment 1 mode)
    if (!player || !targetTerritory) {
        std::cout << "Validating Deploy order (Assignment 1 mode)..." << std::endl;
        return true;
    }
    
    // Assignment 2 validation
    if (!player->ownsTerritory(targetTerritory)) {
        std::cout << "Deploy INVALID: " << player->getName() 
                  << " does not own " << targetTerritory->getName() << std::endl;
        return false;
    }
    
    if (player->getReinforcementPool() < *armiesToDeploy) {
        std::cout << "Deploy INVALID: Not enough reinforcements (have " 
                  << player->getReinforcementPool() << ", need " 
                  << *armiesToDeploy << ")" << std::endl;
        return false;
    }
    
    std::cout << "Deploy VALID: Deploying " << *armiesToDeploy 
              << " armies to " << targetTerritory->getName() << std::endl;
    return true;
}

void Deploy::execute() {
    if (validate()) {
        // Handle Assignment 1 mode
        if (!player || !targetTerritory) {
            std::cout << "Executing Deploy order (Assignment 1 mode)..." << std::endl;
            delete effect;
            effect = new std::string("Deploy order executed (placeholder)");
            *executed = true;
            std::cout << "Deploy order executed successfully." << std::endl;
            return;
        }
        
        // Assignment 2 execution
        player->setReinforcementPool(player->getReinforcementPool() - *armiesToDeploy);
        targetTerritory->setNumberOfArmies(
            targetTerritory->getNumberOfArmies() + *armiesToDeploy
        );
        
        delete effect;
        effect = new std::string("Deployed " + std::to_string(*armiesToDeploy) + 
                                " armies to " + targetTerritory->getName());
        *executed = true;
        
        std::cout << "Deploy EXECUTED: " << *effect << std::endl;
    } else {
        delete effect;
        effect = new std::string("Deploy order was invalid and not executed");
        std::cout << "Deploy NOT EXECUTED" << std::endl;
    }
}

Order* Deploy::clone() const {
    return new Deploy(*this);
}

// ============================================================================
// ADVANCE ORDER IMPLEMENTATION
// ============================================================================

// Default constructor for Assignment 1 compatibility
Advance::Advance() : Order("Advance") {
    this->player = nullptr;
    this->sourceTerritory = nullptr;
    this->targetTerritory = nullptr;
    this->armiesToMove = new int(0);
}

Advance::Advance(Player* player, Territory* source, Territory* target, int numArmies)
    : Order("Advance") {
    this->player = player;
    this->sourceTerritory = source;
    this->targetTerritory = target;
    this->armiesToMove = new int(numArmies);
}

Advance::Advance(const Advance& other) : Order(other) {
    player = other.player;
    sourceTerritory = other.sourceTerritory;
    targetTerritory = other.targetTerritory;
    armiesToMove = new int(*other.armiesToMove);
}

Advance::~Advance() {
    delete armiesToMove;
}

Advance& Advance::operator=(const Advance& other) {
    if (this != &other) {
        Order::operator=(other);
        player = other.player;
        sourceTerritory = other.sourceTerritory;
        targetTerritory = other.targetTerritory;
        delete armiesToMove;
        armiesToMove = new int(*other.armiesToMove);
    }
    return *this;
}

bool Advance::validate() {
    // Handle default constructor case (Assignment 1 mode)
    if (!player || !sourceTerritory || !targetTerritory) {
        std::cout << "Validating Advance order (Assignment 1 mode)..." << std::endl;
        return true;
    }
    
    // Assignment 2 validation
    if (!player->ownsTerritory(sourceTerritory)) {
        std::cout << "Advance INVALID: " << player->getName() 
                  << " does not own source " << sourceTerritory->getName() << std::endl;
        return false;
    }
    
    if (!sourceTerritory->isAdjacentTo(targetTerritory)) {
        std::cout << "Advance INVALID: " << sourceTerritory->getName() 
                  << " is not adjacent to " << targetTerritory->getName() << std::endl;
        return false;
    }
    
    if (sourceTerritory->getNumberOfArmies() < *armiesToMove) {
        std::cout << "Advance INVALID: Not enough armies on " 
                  << sourceTerritory->getName() << std::endl;
        return false;
    }
    
    if (targetTerritory->getOwner() != player && 
        targetTerritory->getOwner() != nullptr &&
        player->hasNegotiationWith(targetTerritory->getOwner())) {
        std::cout << "Advance INVALID: Cannot attack " 
                  << targetTerritory->getOwner()->getName() 
                  << " due to negotiate order" << std::endl;
        return false;
    }
    
    std::cout << "Advance VALID: Moving " << *armiesToMove 
              << " armies from " << sourceTerritory->getName() 
              << " to " << targetTerritory->getName() << std::endl;
    return true;
}

void Advance::simulateBattle(int attackers, int defenders) {
    std::cout << "\n=== BATTLE SIMULATION ===" << std::endl;
    std::cout << "Attackers: " << attackers << " | Defenders: " << defenders << std::endl;
    
    int attackersRemaining = attackers;
    int defendersRemaining = defenders;
    
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    
    for (int i = 0; i < attackers && defendersRemaining > 0; i++) {
        if (std::rand() % 100 < 60) {
            defendersRemaining--;
        }
    }
    
    for (int i = 0; i < defenders && attackersRemaining > 0; i++) {
        if (std::rand() % 100 < 70) {
            attackersRemaining--;
        }
    }
    
    std::cout << "Battle Result - Attackers: " << attackersRemaining 
              << " | Defenders: " << defendersRemaining << std::endl;
    
    sourceTerritory->setNumberOfArmies(
        sourceTerritory->getNumberOfArmies() - attackers
    );
    
    if (defendersRemaining == 0) {
        std::cout << "Territory conquered by " << player->getName() << "!" << std::endl;
        
        Player* previousOwner = targetTerritory->getOwner();
        if (previousOwner) {
            previousOwner->removeTerritory(targetTerritory);
        }
        
        player->addTerritory(targetTerritory);
        targetTerritory->setNumberOfArmies(attackersRemaining);
        player->setConqueredThisTurn(true);
        
        delete effect;
        effect = new std::string("Conquered " + targetTerritory->getName() + 
                                " with " + std::to_string(attackersRemaining) + 
                                " armies surviving");
    } else {
        targetTerritory->setNumberOfArmies(defendersRemaining);
        
        delete effect;
        effect = new std::string("Attack on " + targetTerritory->getName() + 
                                " failed. " + std::to_string(defendersRemaining) + 
                                " defenders remain");
    }
    
    std::cout << "=== BATTLE END ===" << std::endl << std::endl;
}

void Advance::execute() {
    if (validate()) {
        // Handle Assignment 1 mode
        if (!player || !sourceTerritory || !targetTerritory) {
            std::cout << "Executing Advance order (Assignment 1 mode)..." << std::endl;
            delete effect;
            effect = new std::string("Advance order executed (placeholder)");
            *executed = true;
            std::cout << "Advance order executed successfully." << std::endl;
            return;
        }
        
        // Assignment 2 execution
        if (player->ownsTerritory(targetTerritory)) {
            sourceTerritory->setNumberOfArmies(
                sourceTerritory->getNumberOfArmies() - *armiesToMove
            );
            targetTerritory->setNumberOfArmies(
                targetTerritory->getNumberOfArmies() + *armiesToMove
            );
            
            delete effect;
            effect = new std::string("Moved " + std::to_string(*armiesToMove) + 
                                    " armies from " + sourceTerritory->getName() + 
                                    " to " + targetTerritory->getName());
            *executed = true;
            
            std::cout << "Advance EXECUTED: " << *effect << std::endl;
        } else {
            simulateBattle(*armiesToMove, targetTerritory->getNumberOfArmies());
            *executed = true;
            std::cout << "Advance EXECUTED: " << *effect << std::endl;
        }
    } else {
        delete effect;
        effect = new std::string("Advance order was invalid and not executed");
        std::cout << "Advance NOT EXECUTED" << std::endl;
    }
}

Order* Advance::clone() const {
    return new Advance(*this);
}

// ============================================================================
// BOMB ORDER IMPLEMENTATION
// ============================================================================

// Default constructor for Assignment 1 compatibility
Bomb::Bomb() : Order("Bomb") {
    this->player = nullptr;
    this->targetTerritory = nullptr;
}

Bomb::Bomb(Player* player, Territory* target) : Order("Bomb") {
    this->player = player;
    this->targetTerritory = target;
}

Bomb::Bomb(const Bomb& other) : Order(other) {
    player = other.player;
    targetTerritory = other.targetTerritory;
}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& other) {
    if (this != &other) {
        Order::operator=(other);
        player = other.player;
        targetTerritory = other.targetTerritory;
    }
    return *this;
}

bool Bomb::validate() {
    // Handle default constructor case
    if (!player || !targetTerritory) {
        std::cout << "Validating Bomb order (Assignment 1 mode)..." << std::endl;
        return true;
    }
        // Assignment 2 validation
    if (player->ownsTerritory(targetTerritory)) {
        std::cout << "Bomb INVALID: Cannot bomb own territory" << std::endl;
        return false;
    }

    
    bool isAdjacent = false;
    const std::vector<Territory*>* territories = player->getTerritories();
    for (Territory* t : *territories) {
        if (t->isAdjacentTo(targetTerritory)) {
            isAdjacent = true;
            break;
        }
    }
    
    if (!isAdjacent) {
        std::cout << "Bomb INVALID: " << targetTerritory->getName() 
                  << " is not adjacent to any owned territory" << std::endl;
        return false;
    }
    
    std::cout << "Bomb VALID: Bombing " << targetTerritory->getName() << std::endl;
    return true;
}

void Bomb::execute() {
    if (validate()) {
        // Handle Assignment 1 mode
        if (!player || !targetTerritory) {
            std::cout << "Executing Bomb order (Assignment 1 mode)..." << std::endl;
            delete effect;
            effect = new std::string("Bomb order executed (placeholder)");
            *executed = true;
            return;
        }

        int currentArmies = targetTerritory->getNumberOfArmies();
        int armiesDestroyed = currentArmies / 2;
        int remainingArmies = currentArmies - armiesDestroyed;
        
        targetTerritory->setNumberOfArmies(remainingArmies);
        
        delete effect;
        effect = new std::string("Bombed " + targetTerritory->getName() + 
                                ": destroyed " + std::to_string(armiesDestroyed) + 
                                " armies, " + std::to_string(remainingArmies) + " remain");
        *executed = true;
        
        std::cout << "Bomb EXECUTED: " << *effect << std::endl;
    } else {
        delete effect;
        effect = new std::string("Bomb order was invalid and not executed");
        std::cout << "Bomb NOT EXECUTED" << std::endl;
    }
}

Order* Bomb::clone() const {
    return new Bomb(*this);
}

// ============================================================================
// BLOCKADE ORDER IMPLEMENTATION
// ============================================================================

// Default constructor for Assignment 1 compatibility
Blockade::Blockade() : Order("Blockade") {
    this->player = nullptr;
    this->targetTerritory = nullptr;
    this->neutralPlayer = nullptr;
}

Blockade::Blockade(Player* player, Territory* target, Player* neutral) 
    : Order("Blockade") {
    this->player = player;
    this->targetTerritory = target;
    this->neutralPlayer = neutral;
}

Blockade::Blockade(const Blockade& other) : Order(other) {
    player = other.player;
    targetTerritory = other.targetTerritory;
    neutralPlayer = other.neutralPlayer;
}

Blockade::~Blockade() {}

Blockade& Blockade::operator=(const Blockade& other) {
    if (this != &other) {
        Order::operator=(other);
        player = other.player;
        targetTerritory = other.targetTerritory;
        neutralPlayer = other.neutralPlayer;
    }
    return *this;
}

bool Blockade::validate() {

     // Handle default constructor case
    if (!player || !targetTerritory || !neutralPlayer) {
        std::cout << "Validating Blockade order (Assignment 1 mode)..." << std::endl;
        return true;
    }

    // Assignment 2 validation
    if (!player->ownsTerritory(targetTerritory)) {
        std::cout << "Blockade INVALID: " << player->getName() 
                  << " does not own " << targetTerritory->getName() << std::endl;
        return false;
    }
    
    std::cout << "Blockade VALID: Blockading " << targetTerritory->getName() << std::endl;
    return true;
}

void Blockade::execute() {
    if (validate()) {
        int currentArmies = targetTerritory->getNumberOfArmies();
        targetTerritory->setNumberOfArmies(currentArmies * 2);
        
        player->removeTerritory(targetTerritory);
        neutralPlayer->addTerritory(targetTerritory);
        
        delete effect;
        effect = new std::string("Blockaded " + targetTerritory->getName() + 
                                ": doubled armies to " + 
                                std::to_string(currentArmies * 2) + 
                                " and transferred to Neutral player");
        *executed = true;
        
        std::cout << "Blockade EXECUTED: " << *effect << std::endl;
    } else {
        delete effect;
        effect = new std::string("Blockade order was invalid and not executed");
        std::cout << "Blockade NOT EXECUTED" << std::endl;
    }
}

Order* Blockade::clone() const {
    return new Blockade(*this);
}

// ============================================================================
// AIRLIFT ORDER IMPLEMENTATION
// ============================================================================

// Default constructor for Assignment 1 compatibility
Airlift::Airlift() : Order("Airlift") {
    this->player = nullptr;
    this->sourceTerritory = nullptr;
    this->targetTerritory = nullptr;
    this->armiesToMove = new int(0);
}

Airlift::Airlift(Player* player, Territory* source, Territory* target, int numArmies)
    : Order("Airlift") {
    this->player = player;
    this->sourceTerritory = source;
    this->targetTerritory = target;
    this->armiesToMove = new int(numArmies);
}

Airlift::Airlift(const Airlift& other) : Order(other) {
    player = other.player;
    sourceTerritory = other.sourceTerritory;
    targetTerritory = other.targetTerritory;
    armiesToMove = new int(*other.armiesToMove);
}

Airlift::~Airlift() {
    delete armiesToMove;
}

Airlift& Airlift::operator=(const Airlift& other) {
    if (this != &other) {
        Order::operator=(other);
        player = other.player;
        sourceTerritory = other.sourceTerritory;
        targetTerritory = other.targetTerritory;
        delete armiesToMove;
        armiesToMove = new int(*other.armiesToMove);
    }
    return *this;
}

bool Airlift::validate() {

    // Handle default constructor case
    if (!player || !sourceTerritory || !targetTerritory) {
        std::cout << "Validating Airlift order (Assignment 1 mode)..." << std::endl;
        return true;
    }
    // Assignment 2 validation
    if (!player->ownsTerritory(sourceTerritory)) {
        std::cout << "Airlift INVALID: " << player->getName() 
                  << " does not own source " << sourceTerritory->getName() << std::endl;
        return false;
    }
    
    
    if (sourceTerritory->getNumberOfArmies() < *armiesToMove) {
        std::cout << "Airlift INVALID: Not enough armies on " 
                  << sourceTerritory->getName() << std::endl;
        return false;
    }
    
    std::cout << "Airlift VALID: Airlifting " << *armiesToMove 
              << " armies from " << sourceTerritory->getName() 
              << " to " << targetTerritory->getName() << std::endl;
    return true;
}

void Airlift::execute() {
    if (validate()) {
        sourceTerritory->setNumberOfArmies(
            sourceTerritory->getNumberOfArmies() - *armiesToMove
        );
        targetTerritory->setNumberOfArmies(
            targetTerritory->getNumberOfArmies() + *armiesToMove
        );
        
        delete effect;
        effect = new std::string("Airlifted " + std::to_string(*armiesToMove) + 
                                " armies from " + sourceTerritory->getName() + 
                                " to " + targetTerritory->getName());
        *executed = true;
        
        std::cout << "Airlift EXECUTED: " << *effect << std::endl;
    } else {
        delete effect;
        effect = new std::string("Airlift order was invalid and not executed");
        std::cout << "Airlift NOT EXECUTED" << std::endl;
    }
}

Order* Airlift::clone() const {
    return new Airlift(*this);
}

// ============================================================================
// NEGOTIATE ORDER IMPLEMENTATION
// ============================================================================

// Default constructor for Assignment 1 compatibility
Negotiate::Negotiate() : Order("Negotiate") {
    this->player = nullptr;
    this->targetPlayer = nullptr;
}

Negotiate::Negotiate(Player* player, Player* targetPlayer) : Order("Negotiate") {
    this->player = player;
    this->targetPlayer = targetPlayer;
}

Negotiate::Negotiate(const Negotiate& other) : Order(other) {
    player = other.player;
    targetPlayer = other.targetPlayer;
}

Negotiate::~Negotiate() {}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    if (this != &other) {
        Order::operator=(other);
        player = other.player;
        targetPlayer = other.targetPlayer;
    }
    return *this;
}

bool Negotiate::validate() {
    // Handle default constructor case
    if (!player || !targetPlayer) {
        std::cout << "Validating Negotiate order (Assignment 1 mode)..." << std::endl;
        return true;
    }

    if (player == targetPlayer) {
        std::cout << "Negotiate INVALID: Cannot negotiate with self" << std::endl;
        return false;
    }
    
    std::cout << "Negotiate VALID: " << player->getName() 
              << " negotiating with " << targetPlayer->getName() << std::endl;
    return true;
}

void Negotiate::execute() {
    if (validate()) {
        player->addNegotiatedPlayer(targetPlayer);
        targetPlayer->addNegotiatedPlayer(player);
        
        delete effect;
        effect = new std::string("Negotiated with " + targetPlayer->getName() + 
                                ": no attacks possible this turn");
        *executed = true;
        
        std::cout << "Negotiate EXECUTED: " << *effect << std::endl;
    } else {
        delete effect;
        effect = new std::string("Negotiate order was invalid and not executed");
        std::cout << "Negotiate NOT EXECUTED" << std::endl;
    }
}

Order* Negotiate::clone() const {
    return new Negotiate(*this);
}

// ============================================================================
// ORDERSLIST CLASS IMPLEMENTATION
// ============================================================================

OrdersList::OrdersList() {
    orders = new std::vector<Order*>();
}

OrdersList::OrdersList(const OrdersList& other) {
    orders = new std::vector<Order*>();
    for (Order* order : *other.orders) {
        orders->push_back(order->clone());
    }
}

OrdersList::~OrdersList() {
    for (Order* order : *orders) {
        delete order;
    }
    delete orders;
}

OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        for (Order* order : *orders) {
            delete order;
        }
        orders->clear();
        
        for (Order* order : *other.orders) {
            orders->push_back(order->clone());
        }
    }
    return *this;
}

void OrdersList::add(Order* order) {
    if (order != nullptr) {
        orders->push_back(order);
    }
}

void OrdersList::remove(int index) {
    if (index >= 0 && index < orders->size()) {
        delete (*orders)[index];
        orders->erase(orders->begin() + index);
    }
}

void OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < orders->size() && 
        toIndex >= 0 && toIndex < orders->size()) {
        Order* orderToMove = (*orders)[fromIndex];
        orders->erase(orders->begin() + fromIndex);
        orders->insert(orders->begin() + toIndex, orderToMove);
    }
}

int OrdersList::size() const {
    return orders->size();
}

Order* OrdersList::getOrder(int index) const {
    if (index >= 0 && index < orders->size()) {
        return (*orders)[index];
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const OrdersList& list) {
    os << "OrdersList (" << list.orders->size() << " orders):" << std::endl;
    for (size_t i = 0; i < list.orders->size(); ++i) {
        os << "  [" << i << "] " << *((*list.orders)[i]) << std::endl;
    }
    return os;
}