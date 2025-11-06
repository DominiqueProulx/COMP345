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
Territory* Player::choseFromToDefend() {
    int i = 1;
    for (Territory* t : *territoriesToDefend) {
        std::cout << i << ". " << t->getName() << ":" << t->getNumberOfArmies() << std::endl;
        i++;
    }

    int choice;
    bool validInput = false;
    while (!validInput) {
        std::cin >> choice;
        if (choice < 1 || choice > territoriesToDefend->size()) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

    Territory* targetTerritory = (*territoriesToDefend)[choice - 1];
	return targetTerritory;
}
Territory* Player::choseFromToAttack() {
    int index = 1;
    std::cout << "chose the territory you want to move armies to: " << std::endl;
    for (Territory* t : *territoriesToAttack) {
        std::cout << index << "." << t->getName() << " : " << t->getNumberOfArmies() << " armies." << std::endl;
        index++;
    }
    int choice;
    bool validInputto = false;
    while (!validInputto) {
        std::cin >> choice;
        if (choice < 1 || choice > territoriesToAttack->size()) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            validInputto = true;
        }
    }
    return (*territoriesToAttack)[choice - 1];
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
    if (territoriesToDefend) territoriesToDefend->clear();
    if (territoriesToAttack) territoriesToAttack->clear();
}
Order* Player::issueAdvanceOrder() {
	std::cout << "You want to advance armies to defend your own territories or to attack a neighboring territory, press d for defend and a for attack" << std::endl;
	bool validInput = false;
	char choice;
	while (!validInput) {
		std::cin >> choice;
		if (choice == 'd' || choice == 'D') {
            Territory* fromTerritory = nullptr;
            Territory* toTerritory = nullptr;
			while (fromTerritory == nullptr || fromTerritory->getNumberOfArmies() < 1) {
				// chose the territory to move armies from
				std::cout << "chose the territory you want to move armies from: " << std::endl;
				fromTerritory = choseFromToDefend();
				if (fromTerritory->getNumberOfArmies() < 1) {
					std::cout << "No armies available to move from " << fromTerritory->getName() << " Please chose another Territory " << std::endl;
				}
			}

			std::cout << "chose the territory you want to move armies to: " << std::endl;
			toTerritory = choseFromToDefend();


			int armiestoMove;
			bool validArmiesInput = false;
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

				std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
				Order* advanceOrder = orderFactory(Player::OrderType::DEPLOY, fromTerritory, toTerritory, nullptr, armiestoMove);
				

				validInput = true;
                return advanceOrder;
			}
		}
		else if (choice == 'a' || choice == 'A') {
			// chose the territory to move armies from
            Territory* fromTerritory = nullptr;
            Territory* toTerritory = nullptr;
			while (fromTerritory == nullptr && fromTerritory->getNumberOfArmies() < 1) {

				std::cout << "Choose the territory you want to move armies from: " << std::endl;
				fromTerritory = choseFromToDefend();
				if (fromTerritory->getNumberOfArmies() < 1) {
					std::cout << "No armies available to move from " << fromTerritory->getName() << " Please choose another Territory " << std::endl;
				}
			}

			//list territories to attack          
			std::cout << "chose the territory you want to move armies to: " << std::endl;
			 toTerritory = choseFromToAttack();
			int armiestoMove;
			bool validArmiesInput = false;
			while (!validArmiesInput) {
				std::cout << "How many armies do you wish to move? " << std::endl;
				std::cin >> armiestoMove;
				if (armiestoMove < 1 || armiestoMove > fromTerritory->getNumberOfArmies()) {
					std::cout << "Invalid number of armies. Please try again." << std::endl;
				}
				else {
					validArmiesInput = true;
				}

				std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
				Order* advanceOrder = orderFactory(Player::OrderType::ADVANCE, fromTerritory, toTerritory, nullptr, armiestoMove);
				
				validInput = true;
                return advanceOrder;
			}
		}
		else {
			std::cout << "Invalid choice. Please try again." << std::endl;
		}
	}
    return nullptr;
}
Order* Player::issueDeployOrder() {

    std::cout << "Which territories would you like to deploy armies to? " << std::endl;
    std::cout << "----- Territories to Defend ----- " << std::endl;
    int i = 1;
    
	Territory* targetTerritory = choseFromToDefend();

    int armiesToDeploy;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Enter number of armies to deploy to " << targetTerritory->getName()
            << " (max " << *reinforcementPool << "): ";
        std::cin >> armiesToDeploy;
        if (armiesToDeploy < 1 || armiesToDeploy > *reinforcementPool) {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

    // Create Deploy order
    Order* deployOrder = orderFactory(Player::OrderType::DEPLOY,nullptr, targetTerritory,nullptr, armiesToDeploy);
    
    *reinforcementPool -= armiesToDeploy;
    std::cout << armiesToDeploy << " armies deployed to " << targetTerritory->getName()
        << ". Remaining reinforcement pool: " << *reinforcementPool << std::endl;
	return deployOrder;
}
Order* Player::issueBombOrder() {
    // choose target territory
	std::cout << "choose the territory you want to bomb: " << std::endl;
    Territory* targetTerritory = choseFromToAttack();
    std::cout << "Creating a Bomb order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order* bombOrder = orderFactory(Player::OrderType::BOMB, nullptr, targetTerritory, nullptr, 0);
    return bombOrder;
}

Order* Player::issueBlockadeOrder() {
    // choose target territory
    std::cout << "choose the territory you want to blockade: " << std::endl;
    Territory* targetTerritory = choseFromToDefend();
	std::cout << "Creating a Blockade order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
	Order* blockadeOrder = orderFactory(Player::OrderType::BLOCKADE, nullptr, targetTerritory, new Player(), 0); //FLAG : need to specify neutral player after integration
	return blockadeOrder;
}
Order* Player::issueAirliftOrder() {
    Territory* fromTerritory;
    Territory* toTerritory;
   
    do {
        // choose the origin territory
        std::cout << "Choose the territory you want to airlift armies from: " << std::endl;
        fromTerritory = choseFromToDefend();

        // choose target territory
        std::cout << "Choose the territory you want to airlift armies to: " << std::endl;
        toTerritory = choseFromToDefend();

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
        if (armiesToAirlift < 1 || armiesToAirlift > fromTerritory->getNumberOfArmies()) {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }
    std::cout << "Creating an Airlift order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order* airliftOrder = orderFactory(Player::OrderType::AIRLIFT, fromTerritory, toTerritory, nullptr, armiesToAirlift);
	return airliftOrder;

}
Order* Player::issueNegotiateOrder() {
	//choose the player to negotiate with
    std::cout << "Choose the territory of the player you want to negotiate with: " << std::endl;
    //chose the owner of a territory 
	Territory* targetTerritory = choseFromToAttack();
	Player* owner = targetTerritory->getOwner();
	std::cout << "Creating a Negotiate order with player owning " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
	Order* negotiateOrder = orderFactory(Player::OrderType::NEGOTIATE, nullptr, nullptr, owner, 0);
	return negotiateOrder;
}
Order* Player::orderFactory(Player::OrderType type,
    Territory* sourceTerritory,
    Territory* targetTerritory ,
    Player* targetOrNeutral ,
    int armies) {

    switch (type) {
    case OrderType::ADVANCE:
        return new Advance(this, sourceTerritory, targetTerritory, armies);
    case OrderType::DEPLOY:
        return new Deploy(this, targetTerritory, armies);
    case OrderType::BLOCKADE:
        return new Blockade(this, targetTerritory, targetOrNeutral);
    case OrderType::AIRLIFT:
        return new Airlift(this, sourceTerritory, targetTerritory, armies);
    case OrderType::BOMB:
        return new Bomb(this, targetTerritory);
    case OrderType::NEGOTIATE:
        return new Negotiate(this,  targetOrNeutral );
    default:
        return nullptr;
    }
}

    // issueOrder()
void Player::issueOrder() {
    // ToDefend and ToAttack list are chosen only once per issueOrderPhase and are cleared at the beginning of a new issueOrderPhase
    if (territoriesToDefend == nullptr || territoriesToAttack == nullptr) {
        territoriesToDefend = new std::vector<Territory*>();
        territoriesToAttack = new std::vector<Territory*>();

        std::cout << "\nPlease specify which of your territories to defend : " << std::endl;
        territoriesToDefend = toDefend();
        std::cout << "\nPlease specify which territories you want to attack : " << std::endl;
        territoriesToAttack = toAttack();
    }

    std::cout << "\nHere are the possible orders you can issue" << std::endl;
    // Only deploy order is available if the reinforcement pool is not empty
    if (*reinforcementPool != 0) {
        std::cout << "\nYour reinforcement pool is not empty, please Deploy your armies first" << std::endl;
        std::cout << *reinforcementPool << " armies need to be deployed." << std::endl;
        // Deploy order to assign reinforcements to owned territories
        Order* deployOrder =  issueDeployOrder();
        if (deployOrder != nullptr) { (*orderslist).add(deployOrder); }
       
    } else {
       
        
        std::cout << "\nHere are the possible orders" << std::endl;
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
            if (choice < 1 || choice > (1 + (*playerHand).getSize())) {
                std::cout << "Invalid choice. Please try again." << std::endl;
            } else {
                validInput = true;
            }
        }

        if (choice == 1) {
			// Advance order (to defend or attack)
           Order* advanceOrder =  issueAdvanceOrder();
           if (advanceOrder != nullptr) (*orderslist).add(advanceOrder);

		}
		else {
			// Issue Order from hand
			std::string cardName = playerHand->getCard(choice - 2)->getName();
			std::cout << "Playing the card" << cardName << ", it will create the order and add it to the orders list" << std::endl;
			Order* cardOrder = playerHand->getCard(choice - 2)->play(*playerHand, *deck, this);
			if (cardOrder != nullptr) { (*orderslist).add(cardOrder); }
		}
	}
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