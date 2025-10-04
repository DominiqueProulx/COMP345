#ifndef ORDERS_H
#define ORDERS_H

#include <string>
#include <vector>
#include <iostream>

// -------- DUMMY CLASSES FOR INTEGRATION --------
// Placeholder for Territory until integration with Map component
// Based on the Territory interface from a1-map branch
class Territory {
public:
    std::string* name;
    int* numberOfArmies;
    
    Territory(const std::string& territoryName);
    Territory(const Territory& other);
    Territory& operator=(const Territory& other);
    ~Territory();
    
    std::string getName() const;
    int getNumberOfArmies() const;
    void setNumberOfArmies(int armies);
};

// Placeholder for Player until integration with Player component
class Player {
public:
    std::string* playerName;
    
    Player(const std::string& name);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();
    
    std::string getName() const;
};

// -------- ORDER BASE CLASS --------
// Abstract base class for all order types
class Order {
public:
    // Constructors and destructor
    Order(const std::string& orderType);
    Order(const Order& other);
    virtual ~Order();
    
    // Assignment operator
    Order& operator=(const Order& other);
    
    // Pure virtual methods - must be implemented by subclasses
    virtual bool validate() = 0;
    virtual void execute() = 0;
    
    // Virtual clone method for deep copying in OrdersList
    virtual Order* clone() const = 0;
    
    // Getters
    std::string getType() const;
    std::string getEffect() const;
    bool isExecuted() const;
    
    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    
protected:
    std::string* orderType;      // Type of order (deploy, advance, etc.)
    std::string* effect;         // Effect description after execution (null until executed)
    bool* executed;              // Execution status
};

// -------- ORDER SUBCLASSES --------

// Deploy order: place armies on a territory
class Deploy : public Order {
public:
    Deploy();
    Deploy(const Deploy& other);
    ~Deploy();
    Deploy& operator=(const Deploy& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
};

// Advance order: move armies from one territory to another
class Advance : public Order {
public:
    Advance();
    Advance(const Advance& other);
    ~Advance();
    Advance& operator=(const Advance& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
};

// Bomb order: destroy half the armies on a territory
class Bomb : public Order {
public:
    Bomb();
    Bomb(const Bomb& other);
    ~Bomb();
    Bomb& operator=(const Bomb& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
};

// Blockade order: triple armies and make territory neutral
class Blockade : public Order {
public:
    Blockade();
    Blockade(const Blockade& other);
    ~Blockade();
    Blockade& operator=(const Blockade& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
};

// Airlift order: move armies to any territory
class Airlift : public Order {
public:
    Airlift();
    Airlift(const Airlift& other);
    ~Airlift();
    Airlift& operator=(const Airlift& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
};

// Negotiate order: prevent attacks between two players
class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const Negotiate& other);
    ~Negotiate();
    Negotiate& operator=(const Negotiate& other);
    
    bool validate() override;
    void execute() override;
    Order* clone() const override;
};

// -------- ORDERSLIST CLASS --------
// Manages a collection of orders with operations to manipulate the list
class OrdersList {
public:
    // Constructors and destructor
    OrdersList();
    OrdersList(const OrdersList& other);
    ~OrdersList();
    
    // Assignment operator
    OrdersList& operator=(const OrdersList& other);
    
    // Methods
    void add(Order* order);                    // Add order to end of list
    void remove(int index);                     // Remove order at index
    void move(int fromIndex, int toIndex);      // Move order from one position to another
    
    // Getters
    int size() const;
    Order* getOrder(int index) const;
    
    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const OrdersList& list);
    
private:
    std::vector<Order*>* orders;  // List of order pointers
};

#endif // ORDERS_H