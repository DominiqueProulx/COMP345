
#include "PlayerStrategies.h"
// ============================================================================
// PLAYER STRATEGY CLASS IMPLEMENTATION
// ============================================================================
PlayerStrategies::PlayerStrategies(Player* p) : player(p) {};

// ============================================================================
// HUMAN STRATEGY IMPLEMENTATION
// ============================================================================


HumanPlayerStrategy::HumanPlayerStrategy(Player* p) : PlayerStrategies(p) {}
//Destructor is default - player uses smart pointer
HumanPlayerStrategy::~HumanPlayerStrategy() = default;

// issueDeployOrder()
// Handles the issuance of a Deploy order by the player.
// Returns a pointer to the created Deploy order
Order* HumanPlayerStrategy::issueDeployOrder() {

    std::cout << "\nWhich territories would you like to deploy armies to? " << std::endl;
    int i = 1;

    Territory* targetTerritory = player->choseFromToDefend();
    if (targetTerritory == nullptr) {
        std::cout << "Deploy order cancelled." << std::endl;
        return nullptr;
    }
    int armiesToDeploy;
    bool validInput = false;
    int reinforcementPool = player->getReinforcementPool();
    int pendingDeployments = player->getPendingDeployments();
    while (!validInput) {
     
        std::cout << "Enter number of armies to deploy to " << targetTerritory->getName()
            << " (max " << (reinforcementPool - pendingDeployments) << "): ";
        std::cin >> armiesToDeploy;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            // Input was not an integer
            std::cin.clear(); // clear the failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        // Validate number of armies to deploy to the chosen territory
        if (armiesToDeploy < 1 || armiesToDeploy >(reinforcementPool - pendingDeployments)) {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

    // Create Deploy order using the order factory
    Order* deployOrder = player->orderFactory(Player::OrderType::DEPLOY, nullptr, targetTerritory, nullptr, armiesToDeploy);

    // Update pending deployments to match the commited armies
	player->setPendingDeployments(pendingDeployments + armiesToDeploy);
    
    std::cout << armiesToDeploy << " armies deployed to " << targetTerritory->getName() << ". Remaining reinforcement pool: " << (reinforcementPool - pendingDeployments) << std::endl;
    return deployOrder;
}

// issueAdvanceOrder()
// Handles the issuance of an Advance order by the HumanPlayerStrategy
// Returns a pointer to the created Advance order
Order* HumanPlayerStrategy::issueAdvanceOrder() {
    std::cout << "You want to advance armies to defend your own territories or to attack a neighboring territory,\n press\"d\"for defend and \"a\" for attack. press 0 to cancel." << std::endl;
    bool validInput = false;
    char choice;
    while (!validInput) {
        std::cin >> choice;

        //Handles defend choice
        if (choice == 'd' || choice == 'D') {
            Territory* fromTerritory = nullptr;
            Territory* toTerritory = nullptr;

            // Loop until a valid territory with armies is chosen to move armies from
            while (fromTerritory == nullptr || fromTerritory->getNumberOfArmies() < 1) {
                // chose the territory to move armies from
                std::cout << "chose the territory you want to move armies from: " << std::endl;
                fromTerritory = player->choseFromToDefend();
                if (fromTerritory == nullptr) {
                    std::cout << "Advance order cancelled." << std::endl;
                    return nullptr;
                }
                // Check if there are armies available to move
                if (fromTerritory->getNumberOfArmies() < 1) {
                    std::cout << "No armies available to move from " << fromTerritory->getName() << " Please chose another Territory or press 0 to skip your turn \n" << std::endl;
                    int choice;
                    std::cin >> choice;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        continue;
                    }
                    if (choice == '0') {
                        std::cout << "Advance order cancelled." << std::endl;
                        return nullptr; // Exit if user chooses to cancel
                    }

                }
            }

            //Chose the territory to move armies to
            std::cout << "chose the territory you want to move armies to: " << std::endl;
            toTerritory = player->choseFromAdjacent(fromTerritory, "defend");


            int armiestoMove;
            bool validArmiesInput = false;
            // Loop until a valid number of armies is entered
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
                // Create the Advance order using the order factory
                std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
                Order* advanceOrder = player->orderFactory(Player::OrderType::DEPLOY, fromTerritory, toTerritory, nullptr, armiestoMove);

                validInput = true;
                return advanceOrder;
            }
        }
        //Handles attack choice
        else if (choice == 'a' || choice == 'A') {
            // chose the territory to move armies from
            Territory* fromTerritory = nullptr;
            Territory* toTerritory = nullptr;
            while (fromTerritory == nullptr || fromTerritory->getNumberOfArmies() < 1) {

                std::cout << "Choose the territory you want to move armies from: " << std::endl;
                fromTerritory = player->choseFromToDefend();
                if (fromTerritory == nullptr) { return nullptr; } // Exit if user chooses to cancel
                if (fromTerritory->getNumberOfArmies() < 1) {
                    std::cout << "No armies available to move from " << fromTerritory->getName() << " Please choose another Territory " << std::endl;
                    int choice;
                    std::cin >> choice;
                    // Handle invalid input types (non-numeric)
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        continue;
                    }
                    // Exit if user chooses to cancel
                    if (choice == '0') {
                        std::cout << "Advance order cancelled." << std::endl;
                        return nullptr;
                    }
                }
            }

            //list territories to attack          
            std::cout << "chose the territory you want to move armies to: " << std::endl;
            toTerritory = player->choseFromAdjacent(fromTerritory, "attack");
            int armiestoMove;
            bool validArmiesInput = false;
            while (!validArmiesInput) {
                std::cout << "How many armies do you wish to move? " << std::endl;
                std::cin >> armiestoMove;

                // Handle invalid input types (non-numeric)
                if (std::cin.fail()) {
                    // Input was not an integer
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    continue;
                }
                // Validate number of armies to move
                if (armiestoMove < 1 || armiestoMove > fromTerritory->getNumberOfArmies()) {
                    std::cout << "Invalid number of armies. Please try again." << std::endl;
                }
                else {
                    validArmiesInput = true;
                }
                // Create the Advance order using the order factory
                std::cout << "Creating an Advance order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
                Order* advanceOrder = player->orderFactory(Player::OrderType::ADVANCE, fromTerritory, toTerritory, nullptr, armiestoMove);

                validInput = true;
                return advanceOrder;
            }
        }
        else if (choice == '0') {
            std::cout << "Advance order cancelled." << std::endl; // Exit if user chooses to cancel
            validInput = true;
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return nullptr;
}

// issueBombOrder()
// Handles the issuance of a Bomb order by the player.
// Returns a pointer to the created Bomb order
Order* HumanPlayerStrategy::issueBombOrder() {
    // choose target territory
    std::cout << "choose the territory you want to bomb: " << std::endl;
    Territory* targetTerritory = player->choseFromToAttack();
    if (targetTerritory == nullptr) {
        std::cout << "Bomb order cancelled." << std::endl;
        return nullptr;
    }
    // Create Bomb order using the order factory
    std::cout << "Creating a Bomb order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order* bombOrder = player->orderFactory(Player::OrderType::BOMB, nullptr, targetTerritory, nullptr, 0);
    return bombOrder;
}

// issueBlockadeOrder()
// Handles the issuance of a Blockade order by the player.
//// Returns a pointer to the created Blockade order
Order* HumanPlayerStrategy::issueBlockadeOrder() {
    // choose target territory
    std::cout << "choose the territory you want to blockade: " << std::endl;
    Territory* targetTerritory = player->choseFromToDefend();
    if (targetTerritory == nullptr) {
        std::cout << "Blockade order cancelled." << std::endl;
        return nullptr;
    }
    // Create Blockade order using the order factory
    std::cout << "Creating a Blockade order targeting " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order* blockadeOrder = player->orderFactory(Player::OrderType::BLOCKADE, nullptr, targetTerritory, new Player(), 0); //FLAG : need to specify neutral player after integration
    return blockadeOrder;
}

// issueAirliftOrder()
// Handles the issuance of an Airlift order by the player.
// Returns a pointer to the created Airlift order
Order* HumanPlayerStrategy::issueAirliftOrder() {
    Territory* fromTerritory;
    Territory* toTerritory;

    do {
        // choose the origin territory
        std::cout << "Choose the territory you want to airlift armies from: " << std::endl;
        fromTerritory = player->choseFromToDefend();
        if (fromTerritory == nullptr) {
            std::cout << "Airlift order cancelled." << std::endl;
            return nullptr;
        }

        // choose target territory
        std::cout << "Choose the territory you want to airlift armies to: " << std::endl;
        toTerritory = player->choseFromToDefend();
        if (toTerritory == nullptr) {
            std::cout << "Airlift order cancelled." << std::endl;
            return nullptr;
        }
        if (fromTerritory == toTerritory) {
            std::cout << "Please try again, the territories cannot be the same." << std::endl;
        }
    } while (fromTerritory == toTerritory); // this compares the pointers , the == operator is not overloaded for Territory 

    //chose number of armies
    int armiesToAirlift;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Enter number of armies to airlift from " << fromTerritory->getName()
            << " to " << toTerritory->getName() << " (max " << fromTerritory->getNumberOfArmies() << "): ";
        std::cin >> armiesToAirlift;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        if (armiesToAirlift < 1 || armiesToAirlift > fromTerritory->getNumberOfArmies()) {
            std::cout << "Invalid number of armies. Please try again." << std::endl;
        }
        else {
            validInput = true;
        }
    }

    // Create Airlift order using the order factory
    std::cout << "Creating an Airlift order from " << fromTerritory->getName() << " to " << toTerritory->getName() << " and adding it to the orders list" << std::endl;
    Order* airliftOrder = player->orderFactory(Player::OrderType::AIRLIFT, fromTerritory, toTerritory, nullptr, armiesToAirlift);
    return airliftOrder;

}

// issueNegotiateOrder()
// Handles the issuance of a Negotiate order by the player.
// Returns a pointer to the created Negotiate order
Order* HumanPlayerStrategy::issueNegotiateOrder() {
    //choose the player to negotiate with
    std::cout << "Choose the territory of the player you want to negotiate with: " << std::endl;
    //chose the owner of a territory 
    Territory* targetTerritory = player->choseFromToAttack();
    if (targetTerritory == nullptr) {
        std::cout << "Negociate order cancelled." << std::endl;
        return nullptr;
    }
    // Get the owner of the chosen territory - Negocation is between players not territories
    Player* owner = targetTerritory->getOwner();
    std::cout << "Creating a Negotiate order with player owning " << targetTerritory->getName() << " and adding it to the orders list" << std::endl;
    // Create Negotiate order using the order factory
    Order* negotiateOrder = player->orderFactory(Player::OrderType::NEGOTIATE, nullptr, nullptr, owner, 0);
    return negotiateOrder;
}

void HumanPlayerStrategy::issueOrder() {
            //First set both toDefend and toAttack list for the player
            std::cout << "\nPlease specify which of your territories to defend : " << std::endl;
            player->setTerritoriesToDefend(toDefend());
            std::cout << "\nPlease specify which territories you want to attack : " << std::endl;
            player->setTerritoriesToAttack(toAttack());
        

        std::cout << "\nHere are the possible orders you can issue" << std::endl;
        // Only deploy order is available if the reinforcement pool is not empty
		int reinforcementPool = player->getReinforcementPool();
		int pendingDeployments = player->getPendingDeployments();
        if ((reinforcementPool - pendingDeployments) != 0) {
            std::cout << "\nYour reinforcement pool is not empty, please Deploy your armies first" << std::endl;
            std::cout << (reinforcementPool - pendingDeployments) << " armies need to be deployed." << std::endl;
            // Deploy order to assign reinforcements to owned territories
            Order* deployOrder = player->issueDeployOrder();
         
            //Add to order list 

            if (deployOrder != nullptr) { player->addOrderToOrderlist(deployOrder); }
    

        }
        else {

            // Other orders are available once the reinforcement pool is empty
            std::cout << "\nHere are the possible orders" << std::endl;
            std::cout << "0. End turn" << std::endl;
            std::cout << "1. Advance" << std::endl;
            std::cout << "From the cards available in your hand : \n" << std::endl;
			Hand* playerHand = player->getHand();
            for (int i = 0; i < (*playerHand).getSize(); i++) {
                std::cout << (i + 2) << ". " << playerHand->getCard(i)->getName() << std::endl;
            }

            bool validInput = false;
            int choice;
            while (!validInput) {
                std::cout << "Please choose an order number" << std::endl;
                std::cin >> choice;
                // Handle invalid input types (non-numeric)
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    continue;
                }

                if (choice < 0 || choice >(1 + (*playerHand).getSize())) {
                    std::cout << "Invalid choice. Please try again." << std::endl;
                }
                else {
                    validInput = true;
                }
            }
            if (choice == 0) {

                return;
            }
            else if (choice == 1) {
                // Advance order (to defend or attack)
                Order* advanceOrder = issueAdvanceOrder();
                if (advanceOrder != nullptr) { player->addOrderToOrderlist(advanceOrder); }
                else { std::cout << "There was a problem with the advance order, it was not added to the orderList" << std::endl; };
            }
            else {
                // Issue Order from hand
                std::string cardName = playerHand->getCard(choice - 2)->getName();
                std::cout << "Playing the card" << cardName << ", it will create the order and add it to the orders list" << std::endl;
				Deck* deck = (player->getDeck());

                Order* cardOrder = playerHand->getCard(choice - 2)->play(*playerHand, *deck, getPlayer());
                if (cardOrder != nullptr) { player->addOrderToOrderlist(cardOrder); }
                else { std::cout << "There was a problem with the card order, it was not added to the orderList" << std::endl; };
            }
        }
    }

// toDefend()
// Player selects the territories they wish to defend this turn, in priority order.
// Returns a vector of Territory pointers representing the defend list.
std::vector<Territory*>* HumanPlayerStrategy::toDefend() {
   
   
    const std::vector<Territory*>* territoriesOwned = player->getTerritories();
    int numberOfTerritories = territoriesOwned->size();

    // If player has no territories, allow to exit
    if (numberOfTerritories == 0) {
        std::cout << "You currently have no territories to defend. Returning to main menu..." << std::endl;
        return nullptr;
    }

    std::vector<Territory*>* territoriesToDefendThisturn = new std::vector<Territory*>();// Create a new vector to hold territories to defend
    bool doneAdding = false;

    std::cout << "\nPlease add territories to defend in priority order." << std::endl;
    std::cout << "(Enter 0 to finish or -1 to cancel)\n" << std::endl;

    // Display owned territories
    for (int i = 0; i < numberOfTerritories; i++) {
        std::cout << i + 1 << ". " << (*territoriesOwned)[i]->getName() << " - " << (*territoriesOwned)[i]->getNumberOfArmies() << std::endl;
    }

    //Capture user input
    while (!doneAdding) {
        std::cout << "\nEnter the number of the territory to add to the defend list: ";
        int choice;
        std::cin >> choice;

        // Handle invalid input types (non-numeric)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        //returns the empty list if user cancels, allows a way out if there are no territories to defend. 
        if (choice == -1) {
            std::cout << "You cancelled territory selection. Returning..." << std::endl;
            return territoriesToDefendThisturn;

        }
        //marks the end of the selection
        if (choice == 0) {
            if (territoriesToDefendThisturn->empty()) {
                std::cout << "You must add at least one territory before finishing, or type -1 to cancel." << std::endl;
            }
            else {
                doneAdding = true;
            }
        }
        else if (choice < 1 || choice > numberOfTerritories) {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            //add the chosen territory to the defend list if not already added
            Territory* territoryToAdd = (*territoriesOwned)[choice - 1];
            if (std::find(territoriesToDefendThisturn->begin(), territoriesToDefendThisturn->end(), territoryToAdd)
                == territoriesToDefendThisturn->end()) {
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

// ============================================================================
// AGGRESSIVE PLAYER STRATEGY IMPLEMENTATION
// ============================================================================

// Constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p) : PlayerStrategies(p) {
    std::cout << "Aggressive mode created for Player " << *(player->getID()) << std::endl;
}

// Destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy() {
    std::cout << "Aggressive player destroyed" << std::endl;
}

// printi g operator
std::ostream& operator<<(std::ostream& os, const AggressivePlayerStrategy& strategy) {
    os << "Aggressive {Player " << *(strategy.player->getID())
    << ", Focus: Attack with strongest territory]";
    return os;
}

//Get strongest territory
Territory* AggressivePlayerStrategy::getStrongestTerritory() {
    Territory* strongest = nullptr;
    int maxArmies = -1;
    
    for (Territory* t : *player->getTerritories()) {
        if (t->getNumberOfArmies() > maxArmies) {
            strongest = t;
            maxArmies = t->getNumberOfArmies();
        }
    }
    return strongest;
}

// toDefend: Return all owned territories
std::vector<Territory*>* AggressivePlayerStrategy::toDefend() {
    auto* list = new std::vector<Territory*>();
    for (Territory* t : *player->getTerritories()) {
        list->push_back(t);
    }
    return list;
}

// toAttack: Return all adjacent enemy territories from strongest territory
std::vector<Territory*>* AggressivePlayerStrategy::toAttack() {
    auto* attackList = new std::vector<Territory*>();
    Territory* strong = getStrongestTerritory();
    
    if (!strong) return attackList;
    
    for (Territory* adj : strong->getAdjacentTerritories()) {
        if (adj->getOwner() != player) {
            attackList->push_back(adj);
        }
    }
    
    return attackList;
}

// issueOrder: Aggressive strategy implementation
void AggressivePlayerStrategy::issueOrder() {
    
    
    // Set defend and attack lists
    player->setTerritoriesToDefend(toDefend());
    player->setTerritoriesToAttack(toAttack());
    
    Territory* strongest = getStrongestTerritory();
    if (!strongest) {
        std::cout << "[Aggressive] No territories owned." << std::endl;
        return;
    }
    
    int reinforcement = player->getReinforcementPool();
    int pending = player->getPendingDeployments();
    
    // Deploy all armies to strongest territory
    if (reinforcement - pending > 0) {
        int deployAmount = reinforcement - pending;
        std::cout << "[Aggressive} Deploying " << deployAmount 
                << " armies to strongest: " << strongest->getName() << std::endl;
        
        Order* deployOrder = player->orderFactory(
            Player::OrderType::DEPLOY, nullptr, strongest, nullptr, deployAmount);
        
        player->setPendingDeployments(pending + deployAmount);
        player->addOrderToOrderlist(deployOrder);
        return;
    }
    
    // Attack from strongest territory
    const std::vector<Territory*>* targets = player->getTerritoriesToAttack();
    if (!targets->empty() && strongest->getNumberOfArmies() > 1) {
        Territory* target = (*targets)[0];
        int armies = strongest->getNumberOfArmies() - 1; // Keep 1 army
        
        std::cout << "[Aggressive] Attacking " << target->getName()
                << " from " << strongest->getName()
                << " with " << armies << " armies" << std::endl;
        
        Order* advanceOrder = player->orderFactory(
            Player::OrderType::ADVANCE, strongest, target, nullptr, armies);
        
        player->addOrderToOrderlist(advanceOrder);
    }
}

// Constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p) : PlayerStrategies(p) {
    std::cout << "Neutral mode created for Player " << *(player->getID()) << std::endl;
}

// Destructor
NeutralPlayerStrategy::~NeutralPlayerStrategy() {
    std::cout << "Neutral Player destroyed" << std::endl;
}

// Do nothing
void NeutralPlayerStrategy::issueOrder() {
    std::cout << "[Neutral] Player " << *(player->getID()) << " takes no action." << std::endl;
}

// Return empty lists
std::vector<Territory*>* NeutralPlayerStrategy::toAttack() {
    return new std::vector<Territory*>();
}

std::vector<Territory*>* NeutralPlayerStrategy::toDefend() {
    return new std::vector<Territory*>();
}

// Copy and assignment
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& other) : PlayerStrategies(other.player) {}
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

// print method
std::ostream& operator<<(std::ostream& os, const NeutralPlayerStrategy& strategy) {
    os << "NeutralPlayerStrategy[Player " << *(strategy.player->getID()) << ", Behavior: Passive]";
    return os;
}
