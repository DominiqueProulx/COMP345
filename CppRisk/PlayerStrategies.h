// COMP 345 - Fall 2025
// Risk Game Project
// //Assignment 3
// Part 1 : PlayerStrategies 
// File: PlayerStrategies.h
#ifndef PLAYERSTRATEGIES
#define PLAYERSTRATEGIES   
#include <vector>
#include "Player.h"


class PlayerStrategies {
protected:
    std::unique_ptr<Player> player;
public:
    PlayerStrategies(Player* p);
    virtual ~PlayerStrategies() = default; // virtual destructor
    virtual void issueOrder() = 0;
    virtual std::vector<Territory*>* toAttack() = 0;
    virtual std::vector<Territory*>* toDefend() = 0;
	Player* getPlayer() const { return player.get(); }
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
		HumanPlayerStrategy& operator=(const HumanPlayerStrategy& other);
		friend std::ostream& operator<<(std::ostream& os, const HumanPlayerStrategy& strategy);

};

class BenevolentPlayerStrategy : public PlayerStrategies {

public:
    BenevolentPlayerStrategy(Player* p);
    ~BenevolentPlayerStrategy();
    void issueOrder() override;
    std::vector<Territory*>* toAttack() override;
    std::vector<Territory*>* toDefend() override;
    //correct copy constructor, assignment operator, and stream insertion operator.
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);
    friend std::ostream& operator<<(std::ostream& os, const BenevolentPlayerStrategy& strategy);

};

class cheaterPlayerStrategy : public PlayerStrategies {
private:
    Player* player;
public:
    cheaterPlayerStrategy(Player* p);
    ~cheaterPlayerStrategy();
    void issueOrder() override;
    std::vector<Territory*>* toAttack() override;
    std::vector<Territory*>* toDefend() override;
    //correct copy constructor, assignment operator, and stream insertion operator.
    cheaterPlayerStrategy(const cheaterPlayerStrategy& other);
    cheaterPlayerStrategy& operator=(const cheaterPlayerStrategy& other);
    friend std::ostream& operator<<(std::ostream& os, const cheaterPlayerStrategy& strategy);

};

#endif // !PLAYERSTRATEGIES