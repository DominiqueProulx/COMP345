// COMP 345 - Fall 2025
// Risk Game Project
// //Assignment 3
// Part 1 : PlayerStrategies 
// File: PlayerStrategies.h
#ifndef PLAYERSTRATEGIES
#define PLAYERSTRATEGIES   

#include <vector>
#include <memory>
#include "Player.h"
#include <iostream>
#include <string>
#include <memory>


class PlayerStrategies {
protected:
    std::unique_ptr<Player> player;
    std::string* strategyType;

public:
    PlayerStrategies(Player* p);
    virtual ~PlayerStrategies(); 
    virtual void issueOrder() = 0;
    virtual std::vector<Territory*>* toAttack() = 0;
    virtual std::vector<Territory*>* toDefend() = 0;
	Player* getPlayer() const { return player.get(); }
    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const  PlayerStrategies& strategey);

  
    virtual Order* issueDeployOrder() { return nullptr; }
    virtual Order* issueAdvanceOrder() { return nullptr; }
    virtual Order* issueBombOrder() { return nullptr; }
    virtual Order* issueBlockadeOrder() { return nullptr; }
    virtual Order* issueAirliftOrder() { return nullptr; }
    virtual Order* issueNegotiateOrder() { return nullptr; }

};

class HumanPlayerStrategy : public PlayerStrategies {
  
        public:
        HumanPlayerStrategy(Player* p);
        ~HumanPlayerStrategy();
        void issueOrder() override;
        std::vector<Territory*>* toAttack() override;
		std::vector<Territory*>* toDefend() override;
        //correct copy constructor, assignment operator, and stream insertion operator.
		HumanPlayerStrategy(const HumanPlayerStrategy& other);
		friend std::ostream& operator<<(std::ostream& os, const HumanPlayerStrategy& strategy);
        Order* issueDeployOrder();
        Order* issueAdvanceOrder();
        Order* issueBombOrder();
        Order* issueBlockadeOrder();
        Order* issueAirliftOrder();
        Order* issueNegotiateOrder();

};

class BenevolentPlayerStrategy : public PlayerStrategies {

public:
    BenevolentPlayerStrategy(Player* p);
    ~BenevolentPlayerStrategy();

    //copy constructor, assignment operator, and stream insertion operator.
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
    friend std::ostream& operator<<(std::ostream& os, const BenevolentPlayerStrategy& strategy);

    void issueOrder() override;
    std::vector<Territory*>* toAttack() override;
    std::vector<Territory*>* toDefend() override;

    Order* issueDeployOrder();
    Order* issueAdvanceOrder();
    Order* issueBlockadeOrder();
    Order* issueAirliftOrder();
    Order* issueNegotiateOrder();

};

class cheaterPlayerStrategy : public PlayerStrategies {

public:
    cheaterPlayerStrategy(Player* p);
    ~cheaterPlayerStrategy();
    //copy constructor, assignment operator, and stream insertion operator.
    cheaterPlayerStrategy(const cheaterPlayerStrategy& other);
    cheaterPlayerStrategy& operator=(const cheaterPlayerStrategy& other);
    friend std::ostream& operator<<(std::ostream& os, const cheaterPlayerStrategy& strategy);

};

// ============================================================================
// Aggressive Player Strategy
// ============================================================================
class AggressivePlayerStrategy : public PlayerStrategies {
public:
    // Constructor and destructor
    AggressivePlayerStrategy(Player* p);
    ~AggressivePlayerStrategy();

    
    void issueOrder() override;
    std::vector<Territory*>* toAttack() override;
    std::vector<Territory*>* toDefend() override;

    // Copy constructor and assignment operator
    AggressivePlayerStrategy(const AggressivePlayerStrategy& other);
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);
    
    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const AggressivePlayerStrategy& strategy);

private:
    // method to fidn strongest territory
    Territory* getStrongestTerritory();
};

class NeutralPlayerStrategy : public PlayerStrategies {
public:
    NeutralPlayerStrategy(Player* p);
    ~NeutralPlayerStrategy();
    void issueOrder() override;
    std::vector<Territory*>* toAttack() override;
    std::vector<Territory*>* toDefend() override;

    // assignment, copy..
    NeutralPlayerStrategy(const NeutralPlayerStrategy& other);
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& other);
    friend std::ostream& operator<<(std::ostream& os, const NeutralPlayerStrategy& strategy);
};
#endif // !PLAYERSTRATEGIES