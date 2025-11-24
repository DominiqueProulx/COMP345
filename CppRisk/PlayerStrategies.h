// COMP 345 - Fall 2025
// Risk Game Project
// //Assignment 3
// Part 1 : PlayerStrategies
// File: PlayerStrategies.h
#ifndef PLAYERSTRATEGIES
#define PLAYERSTRATEGIES

#include <vector>
#include "Player.h"
#include <iostream>
#include <memory>
#include <string>
#include <memory>

class PlayerStrategies
{
protected:
    Player*  player;
    std::string* strategyType;

public:
    PlayerStrategies(Player *p);
    virtual ~PlayerStrategies();
    virtual void issueOrder() = 0;
    virtual std::vector<Territory*> toAttack() = 0;
    virtual std::vector<Territory*> toDefend() = 0;
	Player* getPlayer() const { return player; }
    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const PlayerStrategies &strategey);

    virtual Order *issueDeployOrder() { return nullptr; }
    virtual Order *issueAdvanceOrder() { return nullptr; }
    virtual Order *issueBombOrder() { return nullptr; }
    virtual Order *issueBlockadeOrder() { return nullptr; }
    virtual Order *issueAirliftOrder() { return nullptr; }
    virtual Order *issueNegotiateOrder() { return nullptr; }
};

class HumanPlayerStrategy : public PlayerStrategies
{

public:
    HumanPlayerStrategy(Player *p);
    ~HumanPlayerStrategy();
    void issueOrder() override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;
    // correct copy constructor, assignment operator, and stream insertion operator.
    HumanPlayerStrategy(const HumanPlayerStrategy &other);
    friend std::ostream &operator<<(std::ostream &os, const HumanPlayerStrategy &strategy);
    Order *issueDeployOrder();
    Order *issueAdvanceOrder();
    Order *issueBombOrder();
    Order *issueBlockadeOrder();
    Order *issueAirliftOrder();
    Order *issueNegotiateOrder();
};

class BenevolentPlayerStrategy : public PlayerStrategies
{

public:
    BenevolentPlayerStrategy(Player *p);
    ~BenevolentPlayerStrategy();

    // copy constructor, assignment operator, and stream insertion operator.
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy &other);
    friend std::ostream &operator<<(std::ostream &os, const BenevolentPlayerStrategy &strategy);

    void issueOrder() override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;

    Order *issueDeployOrder();
    Order *issueAdvanceOrder();
    Order *issueBlockadeOrder();
    Order *issueAirliftOrder();
    Order *issueNegotiateOrder();
};

 class cheaterPlayerStrategy : public PlayerStrategies
{

public:
    cheaterPlayerStrategy(Player *p);
    ~cheaterPlayerStrategy();
    // copy constructor, assignment operator, and stream insertion operator.
    cheaterPlayerStrategy(const cheaterPlayerStrategy &other);
     friend std::ostream &operator<<(std::ostream &os, const cheaterPlayerStrategy &strategy);

     void issueOrder() override;
     std::vector<Territory*> toAttack() override;
     std::vector<Territory*> toDefend() override;
     Order* issueDeployOrder();
     Order* issueAdvanceOrder();
     void cheat();
 };

class AggressivePlayerStrategy : public PlayerStrategies
{

public:
    AggressivePlayerStrategy(Player *p);
    ~AggressivePlayerStrategy();

    // copy constructor, assignment operator, and stream insertion operator.
    AggressivePlayerStrategy(const AggressivePlayerStrategy &other);
    friend std::ostream &operator<<(std::ostream &os, const AggressivePlayerStrategy &strategy);

    void issueOrder() override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;

    Order *issueDeployOrder();
    Order *issueAdvanceOrder();
    Order *issueBombOrder();

    Territory* getStrongestTerritory();
};


class NeutralPlayerStrategy : public PlayerStrategies
{

public:
    NeutralPlayerStrategy(Player p);
    NeutralPlayerStrategy(Player *p);
    ~NeutralPlayerStrategy() override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;

    NeutralPlayerStrategy(const NeutralPlayerStrategy& other);
    friend std::ostream& operator<<(std::ostream& os, const NeutralPlayerStrategy& strategy);
};

#endif // !PLAYERSTRATEGIES